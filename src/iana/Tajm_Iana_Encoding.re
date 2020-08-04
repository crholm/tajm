let to_string = (arr: array(char)): string => {
  String.init(Array.length(arr), i => arr[i]);
};
let from_string = (str: string): array(char) => {
  Array.init(String.length(str), i => str.[i]);
};

let split_string = (c: char, str: string) => {
  let rec split = (str: string, acc: list(string)) =>
    if (str == "") {
      acc;
    } else {
      let len = String.length(str);
      let i =
        try(String.index(str, c)) {
        | _ => len
        };

      let next = i == len ? "" : String.sub(str, i + 1, len - i - 1);
      split(next, acc @ [String.sub(str, 0, i)]);
    };
  split(str, []) |> Array.of_list;
};

let poll = (i, arr) => {
  let len = Array.length(arr);
  (Array.sub(arr, 0, i), Array.sub(arr, i, len - i));
};

let int_of_big_endian_bytes = arr => {
  Array.fold_left((acc, b) => {acc lsl 8 lor int_of_char(b)}, 0, arr);
};
let int_of_little_endian_bytes = arr => {
  Array.fold_right((b, acc) => {acc lsl 8 lor int_of_char(b)}, arr, 0);
};

let float_of_big_endian_bytes = arr => {
  let len = Array.length(arr);

  if (len == 8) {
    Array.fold_left(
      (acc, b) => {
        let a = Int64.shift_left(acc, 8);
        let bb = Int64.of_int(int_of_char(b));
        Int64.logor(a, bb);
      },
      0L,
      arr,
    )
    |> Int64.to_float;
  } else {
    int_of_big_endian_bytes(arr) |> float_of_int;
  };
};

let read_untils = (arr: array(char), num: int, i64) => {
  let rec read = (acc: array(float), arr: array(char), num: int) =>
    if (num < 1) {
      (acc, arr);
    } else {
      let ilen = i64 ? 8 : 4;
      let (i, arr) = poll(ilen, arr);
      let until = float_of_big_endian_bytes(i) *. 1000.;
      acc[Array.length(acc) - num] = until;
      read(acc, arr, num - 1);
    };
  read(Array.make(num, 0.0), arr, num);
};
let read_zone_idxs = (arr: array(char), num: int) => {
  let rec read = (acc: array(int), arr: array(char), num: int) =>
    if (num < 1) {
      (acc, arr);
    } else {
      let (i, arr) = poll(1, arr);
      acc[Array.length(acc) - num] = int_of_char(i[0]);
      read(acc, arr, num - 1);
    };
  read(Array.make(num, 0), arr, num);
};

let read_zone_infos = (arr: array(char), num: int) => {
  let rec read = (acc, arr, num) =>
    if (num < 1) {
      (acc, arr);
    } else {
      let (off, arr) = poll(4, arr);
      let (dst, arr) = poll(1, arr);
      let (desig, arr) = poll(1, arr);
      acc[Array.length(acc) - num] =
        Tajm_Iana_Tz.{
          offset: int_of_big_endian_bytes(off),
          dst: dst[0] == '\001',
          abbrev_idx: int_of_char(desig[0]),
        };
      read(acc, arr, num - 1);
    };
  read(
    Array.make(num, Tajm_Iana_Tz.{offset: 0, dst: false, abbrev_idx: 0}),
    arr,
    num,
  );
};

let read_leap_seconds = (arr: array(char), num: int, is64) => {
  let rec read = (acc: array((float, int)), arr: array(char), num: int) => {
    let len = is64 ? 8 : 4;
    if (num < 1) {
      (acc, arr);
    } else {
      let (at, arr) = poll(len, arr);
      let (seconds, arr) = poll(4, arr);
      acc[Array.length(acc) - num] = (
        float_of_big_endian_bytes(at),
        int_of_big_endian_bytes(seconds),
      );
      read(acc, arr, num - 1);
    };
  };
  read(Array.make(num, (0.0, 0)), arr, num);
};

let read_bools = (arr: array(char), num: int) => {
  let rec read = (acc: array(bool), arr: array(char), num: int) =>
    if (num < 1) {
      (acc, arr);
    } else {
      let (b, arr) = poll(1, arr);
      acc[Array.length(acc) - num] = b[0] == '\001';
      read(acc, arr, num - 1);
    };
  read(Array.make(num, false), arr, num);
};

type header = {
  version: int,
  n_utc_local: int,
  n_std_wall: int,
  n_leap: int,
  n_time: int,
  n_zone: int,
  n_char: int,
};

let read_headers = (data: array(char)): (header, array(char)) => {
  //    * The magic four-byte ASCII sequence “TZif” identifies the file as a
  //      timezone information file.
  let (magicheader, data) = poll(4, data);
  if (magicheader |> to_string != "TZif") {
    raise(Failure("dose not start with TZif"));
  };

  //    * A byte identifying the version of the file's format (as of 2017,
  //      either an ASCII NUL, or “2”, or “3”).
  let (vstr, data) = poll(1, data);
  let version =
    switch (vstr[0]) {
    | '\000' => 1
    | '2' => 2
    | '3' => 3
    | _ => raise(Failure("bad version byte"))
    };

  //    * Fifteen bytes containing zeros reserved for future use.
  let (_, data) = poll(15, data);

  //    * Six four-byte integer values, in the following order:
  //    tzh_ttisutcnt
  //         The number of UT/local indicators stored in the file.  (UT
  //         is Universal Time.)
  let (i, data) = poll(4, data);
  let n_utc_local = int_of_big_endian_bytes(i);

  //    tzh_ttisstdcnt
  //         The number of standard/wall indicators stored in the file.

  let (i, data) = poll(4, data);
  let n_std_wall = int_of_big_endian_bytes(i);

  //    tzh_leapcnt
  //         The number of leap seconds for which data entries are stored in the file.
  let (i, data) = poll(4, data);
  let n_leap = int_of_big_endian_bytes(i);

  //     tzh_timecnt
  //         The number of transition times for which data entries are stored in the file.

  let (i, data) = poll(4, data);
  let n_time = int_of_big_endian_bytes(i);

  //     tzh_typecnt
  //         The number of local time types for which data entries are stored in the file (must not be zero).
  let (i, data) = poll(4, data);
  let n_zone = int_of_big_endian_bytes(i);

  //     tzh_charcnt
  //         The number of bytes of timezone abbreviation strings stored in the file.
  let (i, data) = poll(4, data);
  let n_char = int_of_big_endian_bytes(i);
  ({version, n_utc_local, n_std_wall, n_leap, n_time, n_zone, n_char}, data);
};

let marshal = (tz: Tajm_Iana_Tz.tz) => {
  let acc = tz.name ++ "|";
  let acc =
    acc
    ++ (
      Array.fold_left((acc, z) => acc ++ " " ++ z, "", tz.abbrev)
      |> String.trim
    );
  acc;
};

let unmarshal_binary = (name: string, data: array(char)) => {
  let (head, data) = read_headers(data);

  let is64 = head.version > 1;
  // Js.log2("Use64", is64);
  // Js.log2("Version", head.version);
  // Js.log2("tzh_ttisutcnt: ", head.n_utc_local);
  // Js.log2("tzh_ttisstdcnt: ", head.n_std_wall);
  // Js.log2("tzh_leapcnt: ", head.n_leap);
  // Js.log2("tzh_timecnt: ", head.n_time);
  // Js.log2("tzh_typecnt: ", head.n_zone);
  // Js.log2("tzh_charcnt: ", head.n_char);

  let (head, data) =
    // If not version 1, skip and re read headers.
    if (is64) {
      let skip =
        4
        * head.n_time
        + head.n_time
        + 6
        * head.n_zone
        + head.n_char
        + 8
        * head.n_leap
        + head.n_zone
        + head.n_utc_local;
      let (_, data) = poll(skip, data);
      read_headers(data);
    } else {
      (head, data);
    };

  // Js.log2("2 Version", head.version);
  // Js.log2("2 tzh_ttisutcnt: ", head.n_utc_local);
  // Js.log2("2 tzh_ttisstdcnt: ", head.n_std_wall);
  // Js.log2("2 tzh_leapcnt: ", head.n_leap);
  // Js.log2("2 tzh_timecnt: ", head.n_time);
  // Js.log2("2 tzh_typecnt: ", head.n_zone);
  // Js.log2("2 tzh_charcnt: ", head.n_char);

  //     tzh_timecnt * four-byte signed integer values sorted in ascending
  //          order.  These values are written in network byte order.  Each is
  //          used as a transition time (as returned by time(2)) at which the
  //          rules for computing local time change.
  let (transitions, data) = read_untils(data, head.n_time, is64);
  // Js.log2("transitions", transitions);
  //     tzh_timecnt * one-byte unsigned integer values; each one but the last
  //         tells which of the different types of local time types described in
  //         the file is associated with the time period starting with the same-
  //         indexed transition time and continuing up to but not including the
  //         next transition time.  (The last time type is present only for
  //         consistency checking with the POSIX-style TZ string described
  //         below.)  These values serve as indices into the next field.
  let (zone_idxs, data) = read_zone_idxs(data, head.n_time);

  // Js.log2("zone_idxs", zone_idxs);

  //     tzh_typecnt * ttinfo entries, each defined as follows:
  //           struct ttinfo {
  //                int32_t        tt_utoff;
  //                unsigned char  tt_isdst;
  //                unsigned char  tt_desigidx;
  //           };
  //      Each structure is written as a four-byte signed integer value for
  //      tt_utoff, in network byte order, followed by a one-byte boolean for
  //      tt_isdst and a one-byte value for tt_desigidx.  In each structure,
  //      tt_utoff gives the number of seconds to be added to UT, tt_isdst
  //      tells whether tm_isdst should be set by localtime(3) and
  //      tt_desigidx serves as an index into the array of time zone
  //      abbreviation bytes that follow the ttinfo structure(s) in the file.
  //      The tt_utoff value is never equal to -2**31, to let 32-bit clients
  //      negate it without overflow.  Also, in realistic applications
  //      tt_utoff is in the range [-89999, 93599] (i.e., more than -25 hours
  //      and less than 26 hours); this allows easy support by
  //      implementations that already support the POSIX-required range
  //      [-24:59:59, 25:59:59].
  let (zones, data) = read_zone_infos(data, head.n_zone);
  // Js.log2("zone_infos", zones);

  let (abbrev_arr, data) = poll(head.n_char, data);
  let abbrev = split_string('\000', to_string(abbrev_arr));
  // Js.log2("abbrev", abbrev);

  // Mapping to correct index
  let zones =
    Array.map(
      (z: Tajm_Iana_Tz.zone) => {
        let idx = z.abbrev_idx;
        let sub = Array.sub(abbrev_arr, 0, idx);
        let idx_ =
          Array.fold_left((acc, c) => acc + (c == '\000' ? 1 : 0), 0, sub);
        Tajm_Iana_Tz.{...z, abbrev_idx: idx_};
      },
      zones,
    );
  // Js.log2("zone_infos2", zones);

  //    tzh_leapcnt * pairs of four-byte values, written in network byte
  //      order; the first value of each pair gives the nonnegative time (as
  //      returned by time(2)) at which a leap second occurs; the second is a
  //      signed integer specifying the total number of leap seconds to be
  //      applied during the time period starting at the given time.  The
  //      pairs of values are sorted in ascending order by time.  Each
  //      transition is for one leap second, either positive or negative;
  //      transitions always separated by at least 28 days minus 1 second.
  let (leap_seconds, data) = read_leap_seconds(data, head.n_leap, is64);
  // Js.log2("leap_seconds", leap_seconds);

  //   tzh_ttisstdcnt * standard/wall indicators, each stored as a one-byte
  //      boolean; they tell whether the transition times associated with
  //      local time types were specified as standard time or local (wall
  //      clock) time.
  let (isstds, data) = read_bools(data, head.n_std_wall);
  // Js.log2("isstds:", isstds);

  //   tzh_ttisutcnt * UT/local indicators, each stored as a one-byte
  //      boolean; they tell whether the transition times associated with
  //      local time types were specified as UT or local time.  If a UT/local
  //      indicator is set, the corresponding standard/wall indicator must
  //      also be set.
  let (isuts, _) = read_bools(data, head.n_utc_local);
  // Js.log2("isuts:", isuts);
  // Js.log2("rem:", Array.length(data));
  Tajm_Iana_Tz.{
    name,
    transitions,
    zone_idxs,
    zones,
    leap_seconds,
    isstds,
    isuts,
    abbrev,
  };
};
