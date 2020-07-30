type month_ =
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

type weekday_ =
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

type time_ = {
  t: int64,
  tz: timezone,
};
let compare = (t1: time_, t2: time_) => Int64.compare(t1.t, t2.t);

let z = Fixed(0);
let local = Location("local");
let at = (tz: timezone, t: time_) => {
  {...t, tz};
};
let inUTC = at(z);
let inLocal = at(local);
let zone = (t: time_) => {
  t.tz;
};
let zero = {t: 0L, tz: z};
let _zero = zero;

type timeunit_ =
  | Year
  | Month
  | Day
  | Hour
  | Minute
  | Second
  | Millisecond;

module Duration = Tajm_Duration;

let stringOfWeekday = (weekday: weekday_): string => {
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

let stringOfWeekdayShort = (weekday: weekday_): string => {
  // TODO apply timezone adjustment
  switch (weekday) {
  | Monday => "Mon"
  | Tuesday => "Tue"
  | Wednesday => "Wed"
  | Thursday => "Thu"
  | Friday => "Fri"
  | Saturday => "Sat"
  | Sunday => "Sun"
  };
};

let intOfWeekday = (weekday: weekday_): int => {
  switch (weekday) {
  | Monday => 0
  | Tuesday => 1
  | Wednesday => 2
  | Thursday => 3
  | Friday => 4
  | Saturday => 5
  | Sunday => 6
  };
};
let weekdayOfInt = (i: int): weekday_ => {
  switch (abs(i) mod 7) {
  | 0 => Monday
  | 1 => Tuesday
  | 2 => Wednesday
  | 3 => Thursday
  | 4 => Friday
  | 5 => Saturday
  | 6 => Sunday
  | _ => raise(Failure("math is broken"))
  };
};

let intOfMonth = (month: month_): int => {
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
let monthOfInt = (i: int): month_ => {
  // TODO apply timezone adjustment
  switch (abs(i) mod 12) {
  | 1 => January
  | 2 => February
  | 3 => March
  | 4 => April
  | 5 => May
  | 6 => June
  | 7 => July
  | 8 => August
  | 9 => September
  | 10 => October
  | 11 => November
  | 0 => December
  | _ => raise(Failure("math is broken"))
  };
};
let stringOfMonth = (month: month_): string => {
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
let stringOfMonthShort = (month: month_): string => {
  // TODO apply timezone adjustment
  switch (month) {
  | January => "Jan"
  | February => "Feb"
  | March => "Mar"
  | April => "Apr"
  | May => "May"
  | June => "Jun"
  | July => "Jul"
  | August => "Aug"
  | September => "Sep"
  | October => "Oct"
  | November => "Nov"
  | December => "Dec"
  };
};

module Days = {
  let ofYear = (year: int): int => {
    let leap = year mod 400 === 0 || year mod 4 === 0 && year mod 100 !== 0;
    leap ? 366 : 365;
  };
  let ofMonth = (year: int, month: month_): int => {
    switch (month) {
    | January => 31
    | February => ofYear(year) == 366 ? 29 : 28
    | March => 31
    | April => 30
    | May => 31
    | June => 30
    | July => 31
    | August => 31
    | September => 30
    | October => 31
    | November => 30
    | December => 31
    };
  };
};

let unix = (t: time_): float => {
  t.t |> Int64.to_float;
};
let ofUnix = (f: float): time_ => {
  {t: f |> Int64.of_float, tz: z};
};

let string = (t: time_) => {
  t |> unix |> Js.Date.fromFloat |> Js.Date.toISOString;
};
let js = (t: time_) => {
  t |> unix |> Js.Date.fromFloat;
};

let make =
    (
      ~y as year: option(int)=?,
      ~m as month: option(month_)=?,
      ~d as day: option(int)=?,
      ~hour: option(int)=?,
      ~min: option(int)=?,
      ~sec: option(int)=?,
      ~ms: option(int)=?,
      tz: timezone,
    )
    : time_ => {
  let d =
    Js.Date.utcWithYMDHMS(
      ~year=float_of_int(year |> Kit.Option.default(0)),
      ~month=
        float_of_int(intOfMonth(month |> Kit.Option.default(January)) - 1),
      ~date=float_of_int(day |> Kit.Option.default(1)),
      ~hours=float_of_int(hour |> Kit.Option.default(0)),
      ~minutes=float_of_int(min |> Kit.Option.default(0)),
      ~seconds=float_of_int(sec |> Kit.Option.default(0)),
      (),
    );

  let msf = ms |> Kit.Option.default(0) |> Int64.of_int;

  let newTime = d |> Int64.of_float |> Int64.add(msf);

  {t: newTime, tz};
};

let now = (): time_ => {
  let ms = Js.Date.now();
  {t: ms |> Int64.of_float, tz: z};
};

let add = (dur: Duration.d, t: time_): time_ => {
  {...t, t: t.t |> Int64.add(dur |> Int64.of_float)};
};

let sub = (t1: time_, t2: time_): Duration.d => {
  Int64.sub(t1.t, t2.t) |> Int64.to_float;
};

let since = (t: time_): Duration.d => {
  t |> sub(now());
};

let until = (t: time_): Duration.d => {
  now() |> sub(t);
};

let truncate = (_m: Duration.d, t: time_): time_ => {
  {...t, t: t.t |> Int64.sub(t.t |> Int64.rem(_m |> Int64.of_float))};
};

// y2000 |> before(y3000) // true
// y3000 |> before(y2000) // flase
let before = (t2: time_, t1: time_): bool => {
  t1.t < t2.t;
};

// y2000 |> after(y3000) // false
// y3000 |> after(y2000) // true
let after = (t2: time_, t1: time_): bool => {
  t1.t > t2.t;
};

let future = (t: time_): bool => {
  now().t < t.t;
};
let past = (t: time_): bool => {
  t.t < now().t;
};

let weekday = (t: time_): weekday_ => {
  // TODO apply timezone adjustment
  let d = (t |> js |> Js.Date.getUTCDay |> int_of_float) - 1;
  (d == (-1) ? 6 : d) |> weekdayOfInt;
};
let _weekday = weekday;

let year = (t: time_): int => {
  // TODO apply timezone adjustment
  t |> js |> Js.Date.getUTCFullYear |> int_of_float;
};
let _year = year;

let month = (t: time_): month_ => {
  // TODO apply timezone adjustment
  (t |> js |> Js.Date.getUTCMonth |> int_of_float) + 1 |> monthOfInt;
};
let _month = month;

// Day of month
let day = (t: time_): int => {
  // TODO apply timezone adjustment
  t |> js |> Js.Date.getUTCDate |> int_of_float;
};
let _day = day;

let hour = (t: time_): int => {
  // TODO apply timezone adjustment
  t |> js |> Js.Date.getUTCHours |> int_of_float;
};
let _hour = hour;

let minute = (t: time_): int => {
  // TODO apply timezone adjustment
  t |> js |> Js.Date.getUTCMinutes |> int_of_float;
};
let _minute = minute;

let second = (t: time_): int => {
  // TODO apply timezone adjustment
  t |> js |> Js.Date.getUTCSeconds |> int_of_float;
};
let _second = second;

let millisecond = (t: time_): int => {
  // TODO apply timezone adjustment
  t |> js |> Js.Date.getUTCMilliseconds |> int_of_float;
};
let _millisecond = millisecond;

let yearDay = (t: time_): int => {
  // TODO apply timezone adjustment
  let year = year(t);
  let rec calc = (acc, month) => {
    month < 1
      ? acc
      : {
        calc(acc + Days.ofMonth(year, month |> monthOfInt), month - 1);
      };
  };
  let month = month(t) |> intOfMonth;
  let d = day(t);
  calc(d, month - 1);
};

let week = (t: time_): int => {
  let doy = yearDay(t);
  let dow = weekday(t) |> intOfWeekday;
  let firstDow = make(~y=year(t), t.tz) |> weekday |> intOfWeekday;
  let week = (doy + 6) / 7;
  dow < firstDow ? week + 1 : week;
};

let clock = (t: time_): (int, int, int) => {
  (hour(t), minute(t), second(t));
};
let date = (t: time_): (int, month_, int) => {
  (year(t), month(t), day(t));
};

let set =
    (
      ~y: option(int)=?,
      ~m: option(month_)=?,
      ~d: option(int)=?,
      ~h: option(int)=?,
      ~min: option(int)=?,
      ~sec: option(int)=?,
      ~ms: option(int)=?,
      t: time_,
    )
    : time_ => {
  make(
    ~y=y |> Kit.Option.default(year(t)),
    ~m=m |> Kit.Option.default(month(t)),
    ~d=d |> Kit.Option.default(day(t)),
    ~hour=h |> Kit.Option.default(hour(t)),
    ~min=min |> Kit.Option.default(minute(t)),
    ~sec=sec |> Kit.Option.default(second(t)),
    ~ms=ms |> Kit.Option.default(millisecond(t)),
    t.tz,
  );
};

let setDate = (y: int, m: month_, d: int, t: time_) => set(~y, ~m, ~d, t);
let setClock = (h: int, min: int, sec: int, t: time_) =>
  set(~h, ~min, ~sec, t);

let addDate =
    (
      ~y as years: option(int)=?,
      ~m as months: option(int)=?,
      ~d as days: option(int)=?,
      t: time_,
    )
    : time_ => {
  let _t = t |> js;
  let y = t |> year;
  let m = _t |> Js.Date.getUTCMonth |> int_of_float;
  let d = t |> day;

  let _n =
    Js.Date.setUTCFullYearMD(
      _t,
      ~year=float_of_int(y + (years |> Kit.Option.default(0))),
      ~month=float_of_int(m + (months |> Kit.Option.default(0))),
      ~date=float_of_int(d + (days |> Kit.Option.default(0))),
      (),
    )
    |> ofUnix;

  {...t, t: _n.t};
};

let rec startOf = (u: timeunit_, t: time_) => {
  switch (u) {
  | Year => t |> set(~m=January) |> startOf(Month)
  | Month => t |> set(~d=1) |> startOf(Day)
  | Day => t |> set(~h=0) |> startOf(Hour)
  | Hour => t |> set(~min=0) |> startOf(Minute)
  | Minute => t |> set(~sec=0) |> startOf(Second)
  | Second => t |> set(~ms=0) |> startOf(Millisecond)
  | Millisecond => t
  };
};

let rec endOf = (u: timeunit_, t: time_) => {
  switch (u) {
  | Year => t |> set(~m=December) |> endOf(Month)
  | Month => t |> set(~d=Days.ofMonth(year(t), month(t))) |> endOf(Day)
  | Day => t |> set(~h=23) |> endOf(Hour)
  | Hour => t |> set(~min=59) |> endOf(Minute)
  | Minute => t |> set(~sec=59) |> endOf(Second)
  | Second => t |> set(~ms=999) |> endOf(Millisecond)
  | Millisecond => t
  };
};

module Is = {
  let equal = (t1: time_, t2: time_): bool => {
    Int64.equal(t1.t, t2.t);
  };
  let zero = (t: time_) => equal(t, _zero);

  let year = (y: int, t: time_) => {
    t |> year == y;
  };
  let month = (m: month_, t: time_) => {
    t |> month == m;
  };
  let day = (d: int, t: time_) => {
    t |> _day == d;
  };
  let hour = (h: int, t: time_) => {
    t |> _hour == h;
  };
  let minute = (m: int, t: time_) => {
    t |> _minute == m;
  };
  let secound = (s: int, t: time_) => {
    t |> _second == s;
  };
  let millisecond = (ms: int, t: time_) => {
    t |> _millisecond == ms;
  };

  let weekday = (w: weekday_, t: time_) => {
    t |> weekday == w;
  };
  let monday = weekday(Monday);
  let tuesday = weekday(Tuesday);
  let wednesday = weekday(Wednesday);
  let thursday = weekday(Thursday);
  let friday = weekday(Friday);
  let saturday = weekday(Saturday);
  let sunday = weekday(Sunday);

  let weekend = (t: time_) => {
    let d = t |> _weekday;
    d == Saturday || d == Sunday;
  };
  let leapYear = (t: time_) => {
    let y = t |> _year;
    y mod 400 === 0 || y mod 4 === 0 && y mod 100 !== 0;
  };

  let rec same = (u: timeunit_, t1: time_, t2: time_): bool => {
    switch (u) {
    | Millisecond => t1 |> equal(t2)
    | Second => _second(t1) == _second(t2) && same(Minute, t1, t2)
    | Minute => _minute(t1) == _minute(t2) && same(Hour, t1, t2)
    | Hour => _hour(t1) == _hour(t2) && same(Day, t1, t2)
    | Day => _day(t1) == _day(t2) && same(Month, t1, t2)
    | Month => _month(t1) == _month(t2) && same(Year, t1, t2)
    | Year => _year(t1) == _year(t2)
    };
  };

  let sameYear = same(Year);
  let sameMonth = same(Month);
  let sameHour = same(Hour);
  let sameMinute = same(Minute);
  let sameSecound = same(Second);
  let sameMillisecond = same(Millisecond);
};

module Fmt = {
  module Common = {
    let ansic = "Mon Jan _2 15:04:05 2006";
    let unixDate = "Mon Jan _2 15:04:05 MST 2006";
    let rubyDate = "Mon Jan 02 15:04:05 -0700 2006";
    let rfc822 = "02 Jan 06 15:04 MST";
    let rfc822Z = "02 Jan 06 15:04 -0700"; // RFC822 with numeric zone
    let rfc850 = "Monday, 02-Jan-06 15:04:05 MST";
    let rfc1123 = "Mon, 02 Jan 2006 15:04:05 MST";
    let rfc1123Z = "Mon, 02 Jan 2006 15:04:05 -0700"; // RFC1123 with numeric zone
    let rfc3339 = "2006-01-02T15:04:05Z07:00";
    let rfc3339ms = "2006-01-02T15:04:05.999Z07:00";
    let kitchen = "3:04PM";
    // Handy time stamps.
    let stamp = "Jan _2 15:04:05";
    let stampMilli = "Jan _2 15:04:05.000";
  };

  type token =
    | LongMonth // = "January";
    | Month // = "Jan";
    | NumMonth // = "1";
    | ZeroMonth // = "01";
    | LongWeekDay // = "Monday";
    | WeekDay // = "Mon";
    | Day // = "2";
    | UnderDay // = "_2";
    | ZeroDay // = "02";
    | UnderYearDay // = "__2";
    | ZeroYearDay // = "002";
    | Hour // = "15";
    | Hour12 // = "3";
    | ZeroHour12 // = "03";
    | Minute // = "4";
    | ZeroMinute // = "04";
    | Second // = "5";
    | ZeroSecond // = "05";
    | LongYear // = "2006";
    | Year // = "06";
    | PM(bool) // = "PM" | "pm";
    | TZ // = "MST";
    | ISO8601TZ // = "Z0700";  prints Z for UTC
    | ISO8601SecondsTZ // = "Z070000";
    | ISO8601ShortTZ // = "Z07";
    | ISO8601ColonTZ // = "Z07:00";  prints Z for UTC
    | ISO8601ColonSecondsTZ // = "Z07:00:00";
    | NumTZ // = "-0700";  always numeric
    | NumSecondsTz // = "-070000";
    | NumShortTZ // = "-07"; always numeric
    | NumColonTZ // = "-07:00";
    | NumColonSecondsTZ // = "-07:00:00";
    | FracSecond0(int) // = ".0"; ".00", ".000", ... , trailing zeros included
    | FracSecond9(int) // = ".9"; ".99", ".999" ... , trailing zeros omited
    | Unkown(string);

  let escape = (c: string) => {
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
    | "_2" => UnderDay
    | "__2" => UnderYearDay
    | "01" => ZeroMonth
    | "02" => ZeroDay
    | "03" => ZeroHour12
    | "04" => ZeroMinute
    | "05" => ZeroSecond
    | "06" => Year
    | "002" => ZeroYearDay
    | "1" => NumMonth
    | "15" => Hour
    | "2" => Day
    | "2006" => LongYear
    | "3" => Hour12
    | "4" => Minute
    | "5" => Second
    | "PM" => PM(true)
    | "pm" => PM(false)
    | "TZ" => TZ
    | "Z07" => ISO8601ShortTZ
    | "Z0700" => ISO8601TZ
    | "Z070000" => ISO8601SecondsTZ
    | "Z07:00" => ISO8601ColonTZ
    | "Z07:00:00" => ISO8601ColonSecondsTZ
    | "-07" => NumShortTZ
    | "-0700" => NumTZ
    | "-070000" => NumSecondsTz
    | "-07:00" => NumColonTZ
    | "-07:00:00" => NumColonSecondsTZ
    | ".0" => FracSecond0(1)
    | ".00" => FracSecond0(2)
    | ".000" => FracSecond0(3)
    | ".9" => FracSecond9(1)
    | ".99" => FracSecond9(2)
    | ".999" => FracSecond9(3)
    | _ => Unkown(s)
    };
  };

  let len = String.length;
  let min = (i: int, j: int) => i < j ? i : j;
  let leftPad = (l: int, padding: char, str: string) => {
    String.init(l - (str |> len), _ => padding) ++ str;
  };

  let peek = (i: int, s: string): string => {
    String.sub(s, 0, min(i, s |> len));
  };
  let strip = (i: int, s: string): string => {
    let l = s |> len;
    String.sub(s, i, l - i);
  };
  let poll = (i: int, s: string): (string, string) => {
    (peek(i, s), strip(i, s));
  };

  let rec unk = (acc: string, rem: string): (string, string) =>
    if (rem |> len == 0) {
      (acc, rem);
    } else if (peek(1, rem) |> escape) {
      (acc, rem);
    } else {
      let (h, t) = poll(1, rem);
      unk(acc ++ h, t);
    };

  let listOfString = (str: string) => List.init(str |> len, i => str.[i]);
  let stringOfList = (l: list(char)) =>
    Kit.List.reduce((acc, c) => acc ++ String.make(1, c), "", l);

  let rec tokenize = (acc: list(token), s: string): list(token) => {
    let parseUnknown = (i, s) => {
      let (t, h) = poll(i, s);
      let (u, rem) = unk(t, h);
      tokenize(acc @ [Unkown(u)], rem);
    };

    s |> len == 0
      ? acc
      : (
        switch (peek(1, s)) {
        | "J" =>
          let i = "January" |> len;
          let ii = "Jan" |> len;
          if (peek(i, s) == "January") {
            tokenize(acc @ [LongMonth], strip(i, s));
          } else if (peek(ii, s) == "Jan") {
            tokenize(acc @ [Month], strip(ii, s));
          } else {
            parseUnknown(1, s);
          };
        | "M" =>
          let i = "Monday" |> len;
          let ii = "Mon" |> len;
          if (peek(i, s) == "Monday") {
            tokenize(acc @ [LongWeekDay], strip(i, s));
          } else if (peek(ii, s) == "Mon") {
            tokenize(acc @ [WeekDay], strip(ii, s));
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
        | "4" => tokenize(acc @ [Minute], strip(1, s))
        | "5" => tokenize(acc @ [Second], strip(1, s))
        | "0" =>
          switch (peek(2, s)) {
          | "01" => tokenize(acc @ [ZeroMonth], strip(2, s))
          | "02" => tokenize(acc @ [ZeroDay], strip(2, s))
          | "03" => tokenize(acc @ [ZeroHour12], strip(2, s))
          | "04" => tokenize(acc @ [ZeroMinute], strip(2, s))
          | "05" => tokenize(acc @ [ZeroSecond], strip(2, s))
          | "06" => tokenize(acc @ [Year], strip(2, s))
          | "00" =>
            peek(3, s) == "002"
              ? tokenize(acc @ [Year], strip(3, s)) : parseUnknown(1, s)
          | _ => parseUnknown(1, s)
          }
        | "_" =>
          if (peek(2, s) == "_2") {
            tokenize(acc @ [UnderDay], strip(2, s));
          } else if (peek(3, s) == "__2") {
            tokenize(acc @ [UnderYearDay], strip(3, s));
          } else {
            parseUnknown(1, s);
          }
        | "." =>
          switch (peek(2, s)) {
          | ".0" =>
            let l =
              strip(1, s)
              |> listOfString
              |> Kit.List.take_while(c => c == '0')
              |> List.length
              |> min(3);
            tokenize(acc @ [FracSecond0(l)], strip(l + 1, s));
          | ".9" =>
            let l =
              strip(1, s)
              |> listOfString
              |> Kit.List.take_while(c => c == '9')
              |> List.length
              |> min(3);
            tokenize(acc @ [FracSecond9(l)], strip(l + 1, s));
          | _ => parseUnknown(1, s)
          }
        | _ => parseUnknown(1, s)
        }
      );
  };

  let printer = (t: time_, tokens: list(token)) => {
    // Js.log("[Debug]: Running Printer");
    tokens
    |> List.fold_left(
         (acc, tk) => {
           acc
           ++ (
             switch (tk) {
             | LongMonth => t |> month |> stringOfMonth
             | Month => t |> month |> stringOfMonthShort
             | NumMonth => t |> month |> intOfMonth |> string_of_int
             | ZeroMonth =>
               t |> month |> intOfMonth |> string_of_int |> leftPad(2, '0')
             | LongWeekDay => t |> weekday |> stringOfWeekday
             | WeekDay => t |> weekday |> stringOfWeekdayShort
             | Day => t |> day |> string_of_int
             | UnderDay => t |> day |> string_of_int |> leftPad(2, ' ')
             | ZeroDay => t |> day |> string_of_int |> leftPad(2, '0')
             | UnderYearDay =>
               t |> yearDay |> string_of_int |> leftPad(3, ' ')
             | ZeroYearDay => t |> yearDay |> string_of_int |> leftPad(3, '0')
             | Hour => t |> hour |> string_of_int
             | Hour12 =>
               let h = t |> hour;
               h / 13 + h mod 13 |> string_of_int;
             | ZeroHour12 => t |> hour |> string_of_int |> leftPad(2, '0')
             | Minute => t |> minute |> string_of_int
             | ZeroMinute => t |> minute |> string_of_int |> leftPad(2, '0')
             | Second => t |> second |> string_of_int
             | ZeroSecond => t |> second |> string_of_int |> leftPad(2, '0')
             | LongYear => t |> year |> string_of_int
             | Year =>
               let y = t |> year |> string_of_int |> leftPad(4, '0');
               String.sub(y, 2, 2);
             | PM(upper) => upper ? "PM" : "pm"
             | FracSecond0(i) =>
               "." ++ (t |> millisecond |> string_of_int |> leftPad(i, '0'))
             | FracSecond9(i) =>
               "."
               ++ (
                 t
                 |> millisecond
                 |> string_of_int
                 |> leftPad(i, '0')
                 |> listOfString
                 |> Kit.List.drop_right_while(c => c == '0')
                 |> stringOfList
               )
             // TZ stuff to do ...
             | TZ => "MST"
             | ISO8601TZ => "Z0700"
             | ISO8601SecondsTZ => "Z070000"
             | ISO8601ShortTZ => "Z07"
             | ISO8601ColonTZ => "Z07:00"
             | ISO8601ColonSecondsTZ => "Z07:00:00"
             | NumTZ => "-0700"
             | NumSecondsTz => "-070000"
             | NumShortTZ => "-07"
             | NumColonTZ => "-07:00"
             | NumColonSecondsTZ => "-07:00:00"
             | Unkown(s) => s
             }
           )
         },
         "",
       );
  };

  let string = (s: string, t: time_) => {
    tokenize([], s) |> printer(t);
  };
};
