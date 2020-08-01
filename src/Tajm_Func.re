open Tajm_Type;
open Tajm_Const;

open Tajm_Conv;
open Tajm_Util;

let compare = (t1: time_, t2: time_) => Int64.compare(t1.t, t2.t);
let at = (loc: location_, t: time_) => {
  {...t, loc};
};

let inLocal = at(local);
let inUTC = at(z);
let inFixed = (name: string, sec: int) => Fixed(name, sec) |> at;
let zone = (t: time_) => {
  t.loc;
};
let zero = {t: 0L, loc: z};

let unix = (t: time_): float => {
  t.t |> Int64.to_float;
};
let ofUnix = (f: float): time_ => {
  {t: f |> Int64.of_float, loc: z};
};

// let js = (t: time_) => {
//   t |> unix |> Tajm_Kernel.fromFloat;
// };

let make =
    (
      ~y as year: option(int)=?,
      ~m as month: option(month_)=?,
      ~d as day: option(int)=?,
      ~hour: option(int)=?,
      ~min: option(int)=?,
      ~sec: option(int)=?,
      ~ms: option(int)=?,
      loc: location_,
    )
    : time_ => {
  let d =
    Tajm_Kernel.withYMDHMS(
      ~year=float_of_int(year |> _valueOr(0)),
      ~month=float_of_int(intOfMonth(month |> _valueOr(January)) - 1),
      ~date=float_of_int(day |> _valueOr(1)),
      ~hours=float_of_int(hour |> _valueOr(0)),
      ~minutes=float_of_int(min |> _valueOr(0)),
      ~seconds=float_of_int(sec |> _valueOr(0)),
      loc,
    );

  let msf = ms |> _valueOr(0) |> Int64.of_int;

  {...d, t: Int64.add(d.t, msf)};
};

let now = (): time_ => {
  let ms = Tajm_Kernel.now();
  {t: ms |> Int64.of_float, loc: z};
};

let add = (dur: duration_, t: time_): time_ => {
  {...t, t: t.t |> Int64.add(dur |> Int64.of_float)};
};

let sub = (t1: time_, t2: time_): duration_ => {
  Int64.sub(t1.t, t2.t) |> Int64.to_float;
};

let since = (t: time_): duration_ => {
  t |> sub(now());
};

let until = (t: time_): duration_ => {
  now() |> sub(t);
};

let truncate = (_m: duration_, t: time_): time_ => {
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
  let d = (t |> Tajm_Kernel.getDay |> int_of_float) - 1;
  (d == (-1) ? 6 : d) |> weekdayOfInt;
};
let _weekday = weekday;

let year = (t: time_): int => {
  // TODO apply timezone adjustment
  t |> Tajm_Kernel.getFullYear |> int_of_float;
};
let _year = year;

let month = (t: time_): month_ => {
  // TODO apply timezone adjustment
  (t |> Tajm_Kernel.getMonth |> int_of_float) + 1 |> monthOfInt;
};
let _month = month;

// Day of month
let day = (t: time_): int => {
  // TODO apply timezone adjustment
  t |> Tajm_Kernel.getDate |> int_of_float;
};
let _day = day;

let hour = (t: time_): int => {
  // TODO apply timezone adjustment
  t |> Tajm_Kernel.getHours |> int_of_float;
};
let _hour = hour;

let minute = (t: time_): int => {
  // TODO apply timezone adjustment
  t |> Tajm_Kernel.getMinutes |> int_of_float;
};
let _minute = minute;

let second = (t: time_): int => {
  // TODO apply timezone adjustment
  t |> Tajm_Kernel.getSeconds |> int_of_float;
};
let _second = second;

let millisecond = (t: time_): int => {
  // TODO apply timezone adjustment
  t |> Tajm_Kernel.getMilliseconds |> int_of_float;
};
let _millisecond = millisecond;

let yearDay = (t: time_): int => {
  // TODO apply timezone adjustment
  let year = year(t);
  let rec calc = (acc, month) => {
    month < 1
      ? acc
      : {
        calc(acc + daysInMonth(year, month |> monthOfInt), month - 1);
      };
  };
  let month = month(t) |> intOfMonth;
  let d = day(t);
  calc(d, month - 1);
};

let week = (t: time_): int => {
  let doy = yearDay(t);
  let dow = weekday(t) |> intOfWeekday;
  let firstDow = make(~y=year(t), t.loc) |> weekday |> intOfWeekday;
  let week = (doy + 6) / 7;
  dow < firstDow ? week + 1 : week;
};

let clock = (t: time_): (int, int, int) => {
  (
    //TODO, maybe adjust for location
    hour(t),
    minute(t),
    second(t),
  );
};
let date = (t: time_): (int, month_, int) => {
  (year(t), month(t), day(t));
};

let set =
    (
      ~y: option(int)=?,
      ~m: option(month_)=?,
      ~d: option(int)=?,
      ~hour as h: option(int)=?,
      ~min: option(int)=?,
      ~sec: option(int)=?,
      ~ms: option(int)=?,
      t: time_,
    )
    : time_ => {
  make(
    ~y=y |> _valueOr(year(t)),
    ~m=m |> _valueOr(month(t)),
    ~d=d |> _valueOr(day(t)),
    ~hour=h |> _valueOr(hour(t)),
    ~min=min |> _valueOr(minute(t)),
    ~sec=sec |> _valueOr(second(t)),
    ~ms=ms |> _valueOr(millisecond(t)),
    t.loc,
  );
};

let setDate = (y: int, m: month_, d: int, t: time_) => set(~y, ~m, ~d, t);
let setClock = (hour: int, min: int, sec: int, t: time_) =>
  set(~hour, ~min, ~sec, t);

let addDate =
    (
      ~y as years: option(int)=?,
      ~m as months: option(int)=?,
      ~d as days: option(int)=?,
      t: time_,
    )
    : time_ => {
  let y = t |> year;
  let m = t |> Tajm_Kernel.getMonth |> int_of_float;
  let d = t |> day;

  let _n =
    Tajm_Kernel.setUTCFullYearMD(
      t |> Tajm_Kernel.toJs,
      ~year=float_of_int(y + (years |> _valueOr(0))),
      ~month=float_of_int(m + (months |> _valueOr(0))),
      ~date=float_of_int(d + (days |> _valueOr(0))),
      (),
    )
    |> ofUnix;
  // TODO: adjust for location
  {...t, t: _n.t};
};

let rec startOf = (u: timeunit_, t: time_) => {
  switch (u) {
  | Year => t |> set(~m=January) |> startOf(Month)
  | Month => t |> set(~d=1) |> startOf(Day)
  | Day => t |> set(~hour=0) |> startOf(Hour)
  | Hour => t |> set(~min=0) |> startOf(Minute)
  | Minute => t |> set(~sec=0) |> startOf(Second)
  | Second => t |> set(~ms=0) |> startOf(Millisecond)
  | Millisecond => t
  };
};

let rec endOf = (u: timeunit_, t: time_) => {
  switch (u) {
  | Year => t |> set(~m=December) |> endOf(Month)
  | Month => t |> set(~d=daysInMonth(year(t), month(t))) |> endOf(Day)
  | Day => t |> set(~hour=23) |> endOf(Hour)
  | Hour => t |> set(~min=59) |> endOf(Minute)
  | Minute => t |> set(~sec=59) |> endOf(Second)
  | Second => t |> set(~ms=999) |> endOf(Millisecond)
  | Millisecond => t
  };
};
