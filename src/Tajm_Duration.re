open Tajm_Type;
open Tajm_Util;

let millisecond: duration_ = 1.0;
let secound: duration_ = 1000. *. millisecond;
let minute: duration_ = 60. *. secound;
let hour: duration_ = 60. *. minute;

let truncate = (_m: duration_, _d: duration_): duration_ => {
  _d -. mod_float(_d, _m);
};

let string = (a: duration_) => {
  let s = mod_float(a, minute) /. secound;
  let m = (mod_float(a, hour) -. s) /. minute;
  let h = a /. hour;

  let parts = Tajm_Kernel.string_of_float(s) |> String.split_on_char('.');
  let ss = parts |> List.hd;
  let ms =
    parts |> List.length < 2
      ? ""
      : "."
        ++ {
          let mms = parts |> List.tl |> List.hd;
          mms |> String.length < 4 ? mms : String.sub(mms, 0, 3);
        };

  (h > 0. ? string_of_int(int_of_float(h)) ++ "h" : "")
  ++ (h > 0. || m > 0. ? string_of_int(int_of_float(m)) ++ "m" : "")
  ++ ss
  ++ ms
  ++ "s";
};

// Todo: replace rais with option
let parse = (_d: string): duration_ => {
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
            s => {
              let l = s |> _listOfString |> _dropRightWhile(c => c == '0');

              let divisor =
                l |> _takeWhile(c => c == '0') |> List.length |> float_of_int;

              l
              |> _stringOfList
              |> float_of_string
              |> (f => f *. 100. /. 10. ** divisor);
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
