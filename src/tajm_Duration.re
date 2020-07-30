type d = float;

let millisecond: d = 1.0;
let secound: d = 1000. *. millisecond;
let minute: d = 60. *. secound;
let hour: d = 60. *. minute;

let string = (a: d) => {
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
                Kit_List.init(String.length(s1), i => String.sub(s1, i, 1))
                |> Kit_List.drop_right_while(c => c == "0");

              let s = Kit.List.reduce((acc, c) => acc ++ c, "", l);

              let divisor =
                l
                |> Kit.List.take_while(c => c == "0")
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
