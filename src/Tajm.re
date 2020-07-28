type month =
  | January
  | February
  | March
  | April
  | May
  | June
  | July
  | August
  | September
  | October
  | November
  | December;

type weekday =
  | Monday
  | Tuesday
  | Wednesday
  | Thursday
  | Friday
  | Saturday
  | Sunday;

type timezone =
  | Fixed(int)
  | Location(string);

type time = {
  t: float,
  tz: timezone,
};

let tzUTC = Fixed(0);

module Duration = {
  type d = float;

  let millisecond: d = 1.0;
  let secound: d = 1000. *. millisecond;
  let minute: d = 60. *. secound;
  let hour: d = 60. *. minute;

  let toString = (a: d) => {
    let s = mod_float(a, minute) /. secound;
    let m = (mod_float(a, hour) -. s) /. minute;
    let h = a /. hour;

    (h > 0. ? string_of_int(int_of_float(h)) ++ "h" : "")
    ++ (h > 0. || m > 0. ? string_of_int(int_of_float(m)) ++ "m" : "")
    ++ Js.Float.toString(s)
    ++ "s";
  };

  let truncate = (_m: d, _d: d): d => {
    _d -. mod_float(_d, _m);
  };

  let parse = (_d: string): d => {
    let hi =
      try(String.index(_d, 'h')) {
      | Not_found => (-1)
      };
    let mi =
      try(String.index(_d, 'm')) {
      | Not_found => (-1)
      };
    let si =
      try(String.index(_d, '.')) {
      | Not_found =>
        try(String.index(_d, 's')) {
        | Not_found => (-1)
        }
      };
    let msi =
      try(String.index(_d, 's')) {
      | Not_found => (-1)
      };

    if (msi == (-1)) {
      raise(Invalid_argument("no_secounds"));
    };

    let ms =
      msi == si
        ? 0.
        : (
          try(
            String.sub(_d, si + 1, msi - si - 1)
            |> (
              s1 => {
                let l =
                  Kit.List.init(String.length(s1), i => String.sub(s1, i, 1))
                  |> Kit.List.drop_right_while(c => c == "0");

                let s = Kit.List.fold_left((acc, c) => acc ++ c, "", l);

                let divisor =
                  l
                  |> Kit.List.take_left_while(c => c == "0")
                  |> List.length
                  |> float_of_int;

                s
                |> float_of_string
                |> (
                  f => f *. 100. /. Js.Math.pow_float(~base=10., ~exp=divisor)
                );
              }
            )
          ) {
          | Failure(_) => raise(Invalid_argument("ms_not_a_number"))
          | Invalid_argument(_) =>
            raise(Invalid_argument("bad_index_of_string"))
          }
        );

    let s =
      try(String.sub(_d, mi + 1, si - mi - 1) |> float_of_string) {
      | Invalid_argument(_) => raise(Invalid_argument("bad_index_of_string"))
      | Failure(_) => raise(Invalid_argument("s_not_a_number"))
      };

    let m =
      switch (mi) {
      | (-1) => 0.
      | _ =>
        try(String.sub(_d, hi + 1, mi - hi - 1) |> float_of_string) {
        | Invalid_argument(_) => 0.
        | Failure(_) => raise(Invalid_argument("m_not_a_number"))
        }
      };

    let h =
      switch (hi) {
      | (-1) => 0.
      | _ =>
        try(String.sub(_d, 0, hi) |> float_of_string) {
        | Invalid_argument(_) => 0.
        | Failure(_) => raise(Invalid_argument("h_not_a_number"))
        }
      };
    h *. hour +. m *. minute +. s *. secound +. ms *. millisecond;
  };
};

let toString = (t: time) => {
  t.t |> Js.Date.fromFloat |> Js.Date.toISOString;
};

let now = (): time => {
  let ms = Js.Date.now();
  {t: ms, tz: tzUTC};
};

// let toTime(year: int, month: month, day: int, hour: int, min: int, sec: float, tz:timezone ): time => {
//   utcWithYMDHMS: (~year: float_of_int(year), ~month: float_of_int(month), ~date: float, ~hours: float, ~minutes: float, ~seconds: float, unit)

// }

let add = (dur: Duration.d, t: time): time => {
  {...t, t: t.t +. dur};
};

let sub = (t1: time, t2: time): Duration.d => {
  t1.t -. t2.t;
};

let since = (t: time): Duration.d => {
  t |> sub(now());
};

let until = (t: time): Duration.d => {
  now() |> sub(t);
};

let unix = (t: time): float => {
  t.t;
};

let truncate = (_m: Duration.d, t: time): time => {
  {...t, t: t.t -. mod_float(t.t, _m)};
};

let utc = (t: time): time => {
  {...t, tz: Fixed(0)};
};

let equal = (t1: time, t2: time): bool => {
  t1.t == t2.t;
};

// y2000 |> before(y3000) // true
// y3000 |> before(y2000) // flase
let before = (t2: time, t1: time): bool => {
  t1.t < t2.t;
};

// y2000 |> after(y3000) // false
// y3000 |> after(y2000) // true
let after = (t2: time, t1: time): bool => {
  t1.t > t2.t;
};

let getWeekday = (t: time): weekday => {
  // TODO apply timezone adjustment
  switch (Js.Date.fromFloat(t.t) |> Js.Date.getUTCDay) {
  | 1. => Monday
  | 2. => Tuesday
  | 3. => Wednesday
  | 4. => Thursday
  | 5. => Friday
  | 6. => Saturday
  | 0. => Sunday
  | _ => raise(Failure("seems like the js api is broken"))
  };
};

let getWeekdayName = (weekday: weekday): string => {
  // TODO apply timezone adjustment
  switch (weekday) {
  | Monday => "Monday"
  | Tuesday => "Tuesday"
  | Wednesday => "Wednesday"
  | Thursday => "Thursday"
  | Friday => "Friday"
  | Saturday => "Saturday"
  | Sunday => "Sunday"
  };
};

let getWeekdayShortName = (weekday: weekday): string => {
  // TODO apply timezone adjustment
  switch (weekday) {
  | Monday => "Mon."
  | Tuesday => "Tue."
  | Wednesday => "Wed."
  | Thursday => "Thu."
  | Friday => "Fri."
  | Saturday => "Sat."
  | Sunday => "Sun."
  };
};

let getYear = (t: time): int => {
  // TODO apply timezone adjustment
  Js.Date.fromFloat(t.t) |> Js.Date.getUTCFullYear |> int_of_float;
};

let getMonth = (t: time): month => {
  // TODO apply timezone adjustment
  switch (Js.Date.fromFloat(t.t) |> Js.Date.getUTCMonth) {
  | 0. => January
  | 1. => February
  | 2. => March
  | 3. => April
  | 4. => May
  | 5. => June
  | 6. => July
  | 7. => August
  | 8. => September
  | 9. => October
  | 10. => November
  | 11. => December
  | _ => raise(Failure("seems like the js api is broken"))
  };
};

let getMonthNumber = (month: month): int => {
  // TODO apply timezone adjustment
  switch (month) {
  | January => 1
  | February => 2
  | March => 3
  | April => 4
  | May => 5
  | June => 6
  | July => 7
  | August => 8
  | September => 9
  | October => 10
  | November => 11
  | December => 12
  };
};
let getMonthName = (month: month): string => {
  // TODO apply timezone adjustment
  switch (month) {
  | January => "January"
  | February => "February"
  | March => "March"
  | April => "April"
  | May => "May"
  | June => "June"
  | July => "July"
  | August => "August"
  | September => "September"
  | October => "October"
  | November => "November"
  | December => "December"
  };
};

let getDay = (t: time): int => {
  // TODO apply timezone adjustment
  Js.Date.fromFloat(t.t) |> Js.Date.getUTCDate |> int_of_float;
};

let getHour = (t: time): int => {
  // TODO apply timezone adjustment
  Js.Date.fromFloat(t.t) |> Js.Date.getUTCHours |> int_of_float;
};

let getMinute = (t: time): int => {
  // TODO apply timezone adjustment
  Js.Date.fromFloat(t.t) |> Js.Date.getUTCMinutes |> int_of_float;
};

let getSecound = (t: time): int => {
  // TODO apply timezone adjustment
  Js.Date.fromFloat(t.t) |> Js.Date.getUTCSeconds |> int_of_float;
};

let clock = (t: time): (int, int, int) => {
  (getHour(t), getMinute(t), getSecound(t));
};
let date = (t: time): (int, month, int) => {
  (getYear(t), getMonth(t), getDay(t));
};

let addDate = (years: int, months: int, days: int, t: time): time => {
  let _t = Js.Date.fromFloat(t.t);
  let y = t |> getYear;
  let m = _t |> Js.Date.getUTCMonth |> int_of_float;
  let d = t |> getDay;

  let _n =
    Js.Date.setUTCFullYearMD(
      _t,
      ~year=float_of_int(y + years),
      ~month=float_of_int(m + months),
      ~date=float_of_int(d + days),
      (),
    );

  {...t, t: _n};
};
