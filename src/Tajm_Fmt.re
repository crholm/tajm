open Tajm_Util;
open Tajm_Type;
open Tajm_Conv;
open Tajm_Func;

type token =
  | LongMonth // = "January";
  | Month // = "Jan";
  | NumMonth // = "1";
  | ZeroMonth // = "01";
  | RightMonth // = "_1";
  | LongWeekDay // = "Monday";
  | WeekDay // = "Mon";
  | NumWeekDay // = "7";
  | Day // = "2";
  | ZeroDay // = "02";
  | RightDay // = "_2";
  | YearDay // = "8"
  | ZeroYearDay // = "008";
  | RightYearDay // = "__8";
  | Hour // = "15";
  | Hour12 // = "3";
  | ZeroHour12 // = "03";
  | RightHour12 // = "_3";
  | Minute // = "4";
  | ZeroMinute // = "04";
  | RightMinute // = "_4";
  | YearWeek // = "49";
  | RightYearWeek // = "_9"
  | ZeroYearWeek // = "09"
  | Second // = "5";
  | ZeroSecond // = "05";
  | RightSecond // = "_5";
  | LongYear // = "2006";
  | Year // = "06";
  | PM(bool) // = "PM" | "pm";
  | TZ // = "MST";
  | ISO8601TZ // = "Z0700";  prints Z for UTC
  | ISO8601ShortTZ // = "Z07";
  | ISO8601ColonTZ // = "Z07:00";  prints Z for UTC
  | NumTZ // = "-0700";  always numeric
  | NumShortTZ // = "-07"; always numeric
  | NumColonTZ // = "-07:00";
  | FracSecond0(int) // = ".0"; ".00", ".000", ... , trailing zeros included
  | FracSecond9(int) // = ".9"; ".99", ".999" ... , trailing zeros omited
  | Unkown(string);

let crtl = (c: string) => {
  switch (c) {
  | "J" => true
  | "M" => true
  | "P" => true
  | "p" => true
  | "T" => true
  | "Z" => true
  | "1" => true
  | "0" => true
  | "2" => true
  | "3" => true
  | "4" => true
  | "5" => true
  | "7" => true
  | "8" => true
  | "9" => true
  | "_" => true
  | "-" => true
  | "." => true
  | _ => false
  };
};

let tokenOfString = (s: string): token => {
  switch (s) {
  | "January" => LongMonth
  | "Jan" => Month
  | "Monday" => LongWeekDay
  | "Mon" => WeekDay
  | "_1" => RightMonth
  | "_2" => RightDay
  | "_3" => RightHour12
  | "_4" => RightMinute
  | "_5" => RightSecond
  | "_9" => RightYearWeek
  | "__8" => RightYearDay
  | "01" => ZeroMonth
  | "02" => ZeroDay
  | "03" => ZeroHour12
  | "04" => ZeroMinute
  | "05" => ZeroSecond
  | "06" => Year
  | "07" => NumWeekDay
  | "008" => ZeroYearDay
  | "09" => ZeroYearWeek
  | "1" => NumMonth
  | "15" => Hour
  | "2" => Day
  | "2006" => LongYear
  | "3" => Hour12
  | "4" => Minute
  | "49" => YearWeek
  | "5" => Second
  | "7" => NumWeekDay
  | "8" => YearDay
  | "PM" => PM(true)
  | "pm" => PM(false)
  | "TZ" => TZ
  | "Z07" => ISO8601ShortTZ
  | "Z0700" => ISO8601TZ
  | "Z07:00" => ISO8601ColonTZ
  | "-07" => NumShortTZ
  | "-0700" => NumTZ
  | "-07:00" => NumColonTZ
  | ".0" => FracSecond0(1)
  | ".00" => FracSecond0(2)
  | ".000" => FracSecond0(3)
  | ".9" => FracSecond9(1)
  | ".99" => FracSecond9(2)
  | ".999" => FracSecond9(3)
  | _ => Unkown(s)
  };
};

let peek = (i: int, s: string): string => {
  String.sub(s, 0, min(i, s |> _len));
};
let strip = (i: int, s: string): string => {
  let l = s |> _len;
  String.sub(s, i, l - i);
};
let poll = (i: int, s: string): (string, string) => {
  (peek(i, s), strip(i, s));
};

let rec unk = (acc: string, rem: string): (string, string) =>
  if (rem |> _len == 0) {
    (acc, rem);
  } else if (peek(1, rem) |> crtl) {
    (acc, rem);
  } else {
    let (h, t) = poll(1, rem);
    unk(acc ++ h, t);
  };

let rec tokenize = (acc: list(token), s: string): list(token) => {
  let parseUnknown = (i, s) => {
    let (t, h) = poll(i, s);
    let (u, rem) = unk(t, h);
    tokenize(acc @ [Unkown(u)], rem);
  };

  s |> _len == 0
    ? acc
    : (
      switch (peek(1, s)) {
      | "J" =>
        let i = "January" |> _len;
        let ii = "Jan" |> _len;
        if (peek(i, s) == "January") {
          tokenize(acc @ [LongMonth], strip(i, s));
        } else if (peek(ii, s) == "Jan") {
          tokenize(acc @ [Month], strip(ii, s));
        } else {
          parseUnknown(1, s);
        };
      | "M" =>
        let i = "Monday" |> _len;
        let ii = "Mon" |> _len;
        if (peek(i, s) == "Monday") {
          tokenize(acc @ [LongWeekDay], strip(i, s));
        } else if (peek(ii, s) == "Mon") {
          tokenize(acc @ [WeekDay], strip(ii, s));
        } else if (peek(3, s) == "MST") {
          tokenize(acc @ [TZ], strip(3, s));
        } else {
          parseUnknown(1, s);
        };
      | "P" =>
        if (peek(2, s) == "PM") {
          tokenize(acc @ [PM(true)], strip(2, s));
        } else {
          parseUnknown(1, s);
        }
      | "p" =>
        if (peek(2, s) == "pm") {
          tokenize(acc @ [PM(false)], strip(2, s));
        } else {
          parseUnknown(1, s);
        }
      | "1" =>
        if (peek(2, s) == "15") {
          tokenize(acc @ [Hour], strip(2, s));
        } else {
          tokenize(acc @ [NumMonth], strip(1, s));
        }
      | "2" =>
        if (peek(4, s) == "2006") {
          tokenize(acc @ [LongYear], strip(4, s));
        } else {
          tokenize(acc @ [Day], strip(1, s));
        }
      | "3" => tokenize(acc @ [Hour12], strip(1, s))
      | "4" =>
        if (peek(2, s) == "49") {
          tokenize(acc @ [YearWeek], strip(2, s));
        } else {
          tokenize(acc @ [Minute], strip(1, s));
        }
      | "5" => tokenize(acc @ [Second], strip(1, s))
      | "7" => tokenize(acc @ [NumWeekDay], strip(1, s))
      | "8" => tokenize(acc @ [YearDay], strip(1, s))
      | "0" =>
        switch (peek(2, s)) {
        | "01" => tokenize(acc @ [ZeroMonth], strip(2, s))
        | "02" => tokenize(acc @ [ZeroDay], strip(2, s))
        | "03" => tokenize(acc @ [ZeroHour12], strip(2, s))
        | "04" => tokenize(acc @ [ZeroMinute], strip(2, s))
        | "05" => tokenize(acc @ [ZeroSecond], strip(2, s))
        | "06" => tokenize(acc @ [Year], strip(2, s))
        | "00" =>
          peek(3, s) == "008"
            ? tokenize(acc @ [ZeroYearDay], strip(3, s))
            : parseUnknown(1, s)
        | _ => parseUnknown(1, s)
        }
      | "_" =>
        switch (peek(2, s)) {
        | "_1" => tokenize(acc @ [RightMonth], strip(2, s))
        | "_2" => tokenize(acc @ [RightDay], strip(2, s))
        | "_3" => tokenize(acc @ [RightHour12], strip(2, s))
        | "_4" => tokenize(acc @ [RightMinute], strip(2, s))
        | "_5" => tokenize(acc @ [RightSecond], strip(2, s))
        | "__" =>
          peek(3, s) == "__8"
            ? tokenize(acc @ [RightYearDay], strip(3, s))
            : parseUnknown(1, s)
        | _ => parseUnknown(1, s)
        }
      | "." =>
        switch (peek(2, s)) {
        | ".0" =>
          let l =
            strip(1, s)
            |> _listOfString
            |> _takeWhile(c => c == '0')
            |> List.length
            |> _min(3);
          tokenize(acc @ [FracSecond0(l)], strip(l + 1, s));
        | ".9" =>
          let l =
            strip(1, s)
            |> _listOfString
            |> _takeWhile(c => c == '9')
            |> List.length
            |> _min(3);
          tokenize(acc @ [FracSecond9(l)], strip(l + 1, s));
        | _ => parseUnknown(1, s)
        }
      | "Z" =>
        if (peek(6, s) == "Z07:00") {
          tokenize(acc @ [ISO8601ColonTZ], strip(6, s));
        } else if (peek(5, s) == "Z0700") {
          tokenize(acc @ [ISO8601TZ], strip(5, s));
        } else if (peek(3, s) == "Z07") {
          tokenize(acc @ [ISO8601ShortTZ], strip(3, s));
        } else {
          parseUnknown(1, s);
        }
      | "-" =>
        if (peek(6, s) == "-07:00") {
          tokenize(acc @ [NumColonTZ], strip(6, s));
        } else if (peek(5, s) == "-0700") {
          tokenize(acc @ [NumTZ], strip(5, s));
        } else if (peek(3, s) == "-07") {
          tokenize(acc @ [NumShortTZ], strip(3, s));
        } else {
          parseUnknown(1, s);
        }
      | _ => parseUnknown(1, s)
      }
    );
};

let printer = (t: time_, tokens: list(token)) => {
  let tzOffset = () => - (t |> Tajm_Kernel.tzAdjustment) / 1000 / 60;

  tokens
  |> List.fold_left(
       (acc, tk) => {
         acc
         ++ (
           switch (tk) {
           | LongMonth => t |> month |> stringOfMonth
           | Month => t |> month |> stringShortOfMonth
           | NumMonth => t |> month |> intOfMonth |> string_of_int
           | ZeroMonth =>
             t |> month |> intOfMonth |> string_of_int |> _leftPad(2, '0')
           | RightMonth =>
             t |> month |> intOfMonth |> string_of_int |> _leftPad(2, ' ')
           | LongWeekDay => t |> weekday |> stringOfWeekday
           | WeekDay => t |> weekday |> stringOfWeekdayShort
           | Day => t |> day |> string_of_int
           | RightDay => t |> day |> string_of_int |> _leftPad(2, ' ')
           | ZeroDay => t |> day |> string_of_int |> _leftPad(2, '0')
           | YearDay => t |> yearDay |> string_of_int
           | RightYearDay => t |> yearDay |> string_of_int |> _leftPad(3, ' ')
           | ZeroYearDay => t |> yearDay |> string_of_int |> _leftPad(3, '0')
           | Hour => t |> hour |> string_of_int |> _leftPad(2, '0')
           | NumWeekDay => (t |> weekday |> intOfWeekday) + 1 |> string_of_int
           | ZeroYearWeek => t |> week |> string_of_int |> _leftPad(2, '0')
           | RightYearWeek => t |> week |> string_of_int |> _leftPad(2, ' ')
           | YearWeek => t |> week |> string_of_int
           | Hour12 =>
             let h = t |> hour;
             (h == 0 ? 12 : h / 13 + h mod 13) |> string_of_int;
           | ZeroHour12 =>
             let h = t |> hour;
             (h == 0 ? 12 : h / 13 + h mod 13)
             |> string_of_int
             |> _leftPad(2, '0');
           | RightHour12 =>
             let h = t |> hour;
             (h == 0 ? 12 : h / 13 + h mod 13)
             |> string_of_int
             |> _leftPad(2, ' ');
           | Minute => t |> minute |> string_of_int
           | ZeroMinute => t |> minute |> string_of_int |> _leftPad(2, '0')
           | RightMinute => t |> minute |> string_of_int |> _leftPad(2, ' ')
           | Second => t |> second |> string_of_int
           | ZeroSecond => t |> second |> string_of_int |> _leftPad(2, '0')
           | RightSecond => t |> second |> string_of_int |> _leftPad(2, ' ')
           | LongYear => t |> year |> string_of_int
           | Year =>
             let y = t |> year |> string_of_int |> _leftPad(4, '0');
             String.sub(y, 2, 2);
           | PM(upper) =>
             t |> hour < 12 ? upper ? "AM" : "am" : upper ? "PM" : "pm"
           | FracSecond0(i) =>
             "." ++ (t |> millisecond |> string_of_int |> _leftPad(i, '0'))
           | FracSecond9(i) =>
             let str =
               t
               |> millisecond
               |> string_of_int
               |> _leftPad(i, '0')
               |> _listOfString
               |> _dropRightWhile(c => c == '0')
               |> _stringOfList;
             String.length(str) == 0 ? "" : "." ++ str;
           // TZ stuff to do ...
           | TZ => t |> Tajm_Kernel.getTZName
           | ISO8601ShortTZ =>
             let off = tzOffset();
             off == 0
               ? "Z"
               : (off < 0 ? "-" : "+")
                 ++ (abs(off) / 60 |> string_of_int |> _leftPad(2, '0'));
           | ISO8601TZ =>
             let off = tzOffset();
             off == 0
               ? "Z"
               : (off < 0 ? "-" : "+")
                 ++ (off / 60 |> string_of_int |> _leftPad(2, '0'))
                 ++ (off mod 60 |> string_of_int |> _leftPad(2, '0'));
           | ISO8601ColonTZ =>
             let off = tzOffset();
             off == 0
               ? "Z"
               : (off < 0 ? "-" : "+")
                 ++ (abs(off) / 60 |> string_of_int |> _leftPad(2, '0'))
                 ++ ":"
                 ++ (abs(off) mod 60 |> string_of_int |> _leftPad(2, '0'));
           | NumShortTZ =>
             let off = tzOffset();
             (off < 0 ? "-" : "+")
             ++ (abs(off) / 60 |> string_of_int |> _leftPad(2, '0'));
           | NumTZ =>
             let off = tzOffset();
             (off < 0 ? "-" : "+")
             ++ (abs(off) / 60 |> string_of_int |> _leftPad(2, '0'))
             ++ (abs(off) mod 60 |> string_of_int |> _leftPad(2, '0'));
           | NumColonTZ =>
             let off = tzOffset();
             (off < 0 ? "-" : "+")
             ++ (abs(off) / 60 |> string_of_int |> _leftPad(2, '0'))
             ++ ":"
             ++ (abs(off) mod 60 |> string_of_int |> _leftPad(2, '0'));
           | Unkown(s) => s
           }
         )
       },
       "",
     );
};

let parse = (s: string, tokens: list(token)): option(time_) => {
  let par = (): option(time_) => {
    let (_, t) =
      tokens
      |> List.fold_left(
           (acc, token) => {
             let (str, t) = acc;
             switch (token) {
             | LongMonth =>
               // = "January";
               let hd = peek(3, str);
               let m = hd |> monthOfStringShort;
               let name = m |> stringOfMonth;
               if (peek(name |> String.length, str) != name) {
                 raise(Failure("Not a valid month"));
               };
               let tl = strip(name |> String.length, str);
               (tl, t |> set(~m));
             | Month =>
               // = "Jan";
               let (hd, tl) = poll(3, str);
               let m = hd |> monthOfStringShort;
               (tl, t |> set(~m));
             | NumMonth =>
               // = "1";
               let (hd, tl) = poll(1, str);
               let next = tl |> peek(1);
               if (hd == "1" && "0" <= next && next <= "2") {
                 let m = hd ++ next |> int_of_string |> monthOfInt;
                 (strip(1, str), t |> set(~m));
               } else {
                 let m = hd |> int_of_string |> monthOfInt;
                 (tl, t |> set(~m));
               };

             | ZeroMonth =>
               // = "01";
               let (hd, tl) = poll(2, str);
               let m = hd |> int_of_string |> monthOfInt;
               (tl, t |> set(~m));
             | RightMonth =>
               // = "_1";
               let (hd, tl) = poll(2, str);
               let m = hd |> String.trim |> int_of_string |> monthOfInt;
               (tl, t |> set(~m));
             | Day =>
               // = "2";
               let (hd, tl) = poll(1, str);
               let next = peek(1, str);
               if ("1" <= hd && hd <= "3" && "0" <= next && next <= "9") {
                 let d = hd ++ next |> int_of_string;
                 (tl |> strip(1), t |> set(~d));
               } else {
                 let d = hd |> int_of_string;
                 (tl, t |> set(~d));
               };
             | ZeroDay =>
               // = "02";
               let (hd, tl) = poll(2, str);
               let d = hd |> int_of_string;
               (tl, t |> set(~d));
             | RightDay =>
               // = "_2";
               let (hd, tl) = poll(2, str);
               let d = hd |> String.trim |> int_of_string;
               (tl, t |> set(~d));

             | Hour =>
               // = "15";
               let (hd, tl) = poll(2, str);
               let hour = hd |> int_of_string;
               (tl, t |> set(~hour));
             | Hour12 =>
               // = "3";
               let (hd, tl) = poll(1, str);
               let next = peek(1, tl);
               if (hd == "1" && "0" <= next && next <= "2") {
                 let hour = hd ++ next |> int_of_string;
                 (tl |> strip(1), t |> set(~hour));
               } else {
                 let hour = hd |> int_of_string;
                 (tl, t |> set(~hour));
               };

             | ZeroHour12 =>
               // = "03";
               let (hd, tl) = poll(2, str);
               let hour = hd |> int_of_string;
               (tl, t |> set(~hour));
             | RightHour12 =>
               // = "_3";
               let (hd, tl) = poll(2, str);
               let hour = hd |> String.trim |> int_of_string;
               (tl, t |> set(~hour));
             | Minute =>
               // = "4";
               let (hd, tl) = poll(1, str);
               let next = peek(1, tl);
               if ("1" <= hd && hd <= "6" && "0" <= next && next <= "9") {
                 let min = hd ++ next |> int_of_string;
                 (tl |> strip(1), t |> set(~min));
               } else {
                 let min = hd |> int_of_string;
                 (tl, t |> set(~min));
               };

             | ZeroMinute =>
               // = "04";
               let (hd, tl) = poll(2, str);
               let min = hd |> int_of_string;
               (tl, t |> set(~min));
             | RightMinute =>
               // = "_4";
               let (hd, tl) = poll(2, str);
               let min = hd |> String.trim |> int_of_string;
               (tl, t |> set(~min));
             | Second =>
               // = "5";
               let (hd, tl) = poll(1, str);
               let next = peek(1, tl);
               if ("1" <= hd && hd <= "6" && "0" <= next && next <= "9") {
                 let sec = hd ++ next |> int_of_string;
                 (tl |> strip(1), t |> set(~sec));
               } else {
                 let sec = hd |> int_of_string;
                 (tl, t |> set(~sec));
               };
             | ZeroSecond =>
               // = "05";
               let (hd, tl) = poll(2, str);
               let sec = hd |> int_of_string;
               (tl, t |> set(~sec));
             | RightSecond =>
               // = "_5";
               let (hd, tl) = poll(2, str);
               let sec = hd |> String.trim |> int_of_string;
               (tl, t |> set(~sec));
             | LongYear =>
               // = "2006";
               let (hd, tl) = poll(4, str);
               let y = hd |> int_of_string;
               (tl, t |> set(~y));
             | Year =>
               // = "06";
               let (hd, tl) = poll(2, str);
               let yy = hd |> int_of_string;
               let y = yy > 50 ? 1900 : 2000;
               (tl, t |> set(~y));

             | PM(_) =>
               // = "PM" | "pm";
               // Todo if "pm" apply adjustment once parsing is done
               let (hd, tl) = poll(2, str);
               if (hd |> String.lowercase_ascii == "pm") {
                 let adjust = t |> hour;
                 let hour = adjust < 12 ? adjust + 12 : adjust;
                 (tl, t |> set(~hour));
               } else {
                 (tl, t);
               };

             | TZ => (str, t) // = "MST";
             | ISO8601TZ => (str, t) // = "Z0700";  prints Z for UTC
             | ISO8601ShortTZ => (str, t) // = "Z07";
             | ISO8601ColonTZ => (str, t) // = "Z07:00";  prints Z for UTC
             | NumTZ => (str, t) // = "-0700";  always numeric
             | NumShortTZ => (str, t) // = "-07"; always numeric
             | NumColonTZ => (str, t) // = "-07:00";
             | FracSecond0(_) => (str, t) // = ".0"; ".00", ".000", ... , trailing zeros included
             | FracSecond9(_) => (str, t) // = ".9"; ".99", ".999" ... , trailing zeros omited

             | YearWeek => (str, t) // = "49";
             | RightYearWeek => (str, t) // = "_9"
             | ZeroYearWeek => (str, t) // = "09"
             | LongWeekDay => (str, t) // = "Monday";
             | WeekDay => (str, t) // = "Mon";
             | NumWeekDay => (str, t) // = "7";
             | YearDay => (str, t) // = "8"
             | ZeroYearDay => (str, t) // = "008";
             | RightYearDay => (str, t) // = "__8";
             | Unkown(ss) => (strip(ss |> String.length, str), t)
             };
           },
           (s, zero),
         );

    Some(t);
  };

  switch (par()) {
  | Some(t) => Some(t)
  | None => None
  | exception _ => None
  };
};
