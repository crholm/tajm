open Tajm_Type;

let equal = (t1: time_, t2: time_): bool => {
  Int64.equal(t1.t, t2.t);
};
let zero = (t: time_) => equal(t, Tajm_Func.zero);

let year = (y: int, t: time_) => {
  t |> Tajm_Func.year == y;
};
let month = (m: month_, t: time_) => {
  t |> Tajm_Func.month == m;
};
let day = (d: int, t: time_) => {
  t |> Tajm_Func.day == d;
};
let hour = (h: int, t: time_) => {
  t |> Tajm_Func.hour == h;
};
let minute = (m: int, t: time_) => {
  t |> Tajm_Func.minute == m;
};
let secound = (s: int, t: time_) => {
  t |> Tajm_Func.second == s;
};
let millisecond = (ms: int, t: time_) => {
  t |> Tajm_Func.millisecond == ms;
};

let weekday = (w: weekday_, t: time_) => {
  t |> Tajm_Func.weekday == w;
};
let monday = weekday(Monday);
let tuesday = weekday(Tuesday);
let wednesday = weekday(Wednesday);
let thursday = weekday(Thursday);
let friday = weekday(Friday);
let saturday = weekday(Saturday);
let sunday = weekday(Sunday);

let weekend = (t: time_) => {
  let d = t |> Tajm_Func.weekday;
  d == Saturday || d == Sunday;
};
let leapYear = (t: time_) => {
  let y = t |> Tajm_Func.year;
  y mod 400 === 0 || y mod 4 === 0 && y mod 100 !== 0;
};

let rec same = (u: timeunit_, t1: time_, t2: time_): bool => {
  switch (u) {
  | Millisecond => equal(t1, t2)
  | Second =>
    Tajm_Func.second(t1) == Tajm_Func.second(t2) && same(Minute, t1, t2)
  | Minute =>
    Tajm_Func.minute(t1) == Tajm_Func.minute(t2) && same(Hour, t1, t2)
  | Hour => Tajm_Func.hour(t1) == Tajm_Func.hour(t2) && same(Day, t1, t2)
  | Day => Tajm_Func.day(t1) == Tajm_Func.day(t2) && same(Month, t1, t2)
  | Month =>
    Tajm_Func.month(t1) == Tajm_Func.month(t2) && same(Year, t1, t2)
  | Year => Tajm_Func.year(t1) == Tajm_Func.year(t2)
  };
};

let sameYear = same(Year);
let sameMonth = same(Month);
let sameDay = same(Day);
let sameHour = same(Hour);
let sameMinute = same(Minute);
let sameSecound = same(Second);
let sameMillisecond = same(Millisecond);
