open Tajm_Type;

module Func = Tajm_Func;

let equal = (t1: time_, t2: time_): bool => {
  Int64.equal(t1.t, t2.t);
};
let zero = (t: time_) => equal(t, Func.zero);

let year = (y: int, t: time_) => {
  t |> Func.year == y;
};
let month = (m: month_, t: time_) => {
  t |> Func.month == m;
};
let day = (d: int, t: time_) => {
  t |> Func.day == d;
};
let hour = (h: int, t: time_) => {
  t |> Func.hour == h;
};
let minute = (m: int, t: time_) => {
  t |> Func.minute == m;
};
let secound = (s: int, t: time_) => {
  t |> Func.second == s;
};
let millisecond = (ms: int, t: time_) => {
  t |> Func.millisecond == ms;
};

let weekday = (w: weekday_, t: time_) => {
  t |> Func.weekday == w;
};
let monday = weekday(Monday);
let tuesday = weekday(Tuesday);
let wednesday = weekday(Wednesday);
let thursday = weekday(Thursday);
let friday = weekday(Friday);
let saturday = weekday(Saturday);
let sunday = weekday(Sunday);

let weekend = (t: time_) => {
  let d = t |> Func.weekday;
  d == Saturday || d == Sunday;
};
let leapYear = (t: time_) => {
  let y = t |> Func.year;
  y mod 400 === 0 || y mod 4 === 0 && y mod 100 !== 0;
};

let rec same = (u: timeunit_, t1: time_, t2: time_): bool => {
  switch (u) {
  | Millisecond => equal(t1, t2)
  | Second =>
    Func.second(t1) == Func.second(t2) && same(Minute, t1, t2)
  | Minute =>
    Func.minute(t1) == Func.minute(t2) && same(Hour, t1, t2)
  | Hour => Func.hour(t1) == Func.hour(t2) && same(Day, t1, t2)
  | Day => Func.day(t1) == Func.day(t2) && same(Month, t1, t2)
  | Month =>
    Func.month(t1) == Func.month(t2) && same(Year, t1, t2)
  | Year => Func.year(t1) == Func.year(t2)
  };
};

let sameYear = same(Year);
let sameMonth = same(Month);
let sameDay = same(Day);
let sameHour = same(Hour);
let sameMinute = same(Minute);
let sameSecound = same(Second);
let sameMillisecond = same(Millisecond);
