open Tajm_Type;

let equal = (t1: time_, t2: time_): bool => {
  Int64.equal(t1.t, t2.t);
};
let zero = (t: time_) => equal(t, Tajm_Fun.zero);

let year = (y: int, t: time_) => {
  t |> Tajm_Fun.year == y;
};
let month = (m: month_, t: time_) => {
  t |> Tajm_Fun.month == m;
};
let day = (d: int, t: time_) => {
  t |> Tajm_Fun.day == d;
};
let hour = (h: int, t: time_) => {
  t |> Tajm_Fun.hour == h;
};
let minute = (m: int, t: time_) => {
  t |> Tajm_Fun.minute == m;
};
let secound = (s: int, t: time_) => {
  t |> Tajm_Fun.second == s;
};
let millisecond = (ms: int, t: time_) => {
  t |> Tajm_Fun.millisecond == ms;
};

let weekday = (w: weekday_, t: time_) => {
  t |> Tajm_Fun.weekday == w;
};
let monday = weekday(Monday);
let tuesday = weekday(Tuesday);
let wednesday = weekday(Wednesday);
let thursday = weekday(Thursday);
let friday = weekday(Friday);
let saturday = weekday(Saturday);
let sunday = weekday(Sunday);

let weekend = (t: time_) => {
  let d = t |> Tajm_Fun.weekday;
  d == Saturday || d == Sunday;
};
let leapYear = (t: time_) => {
  let y = t |> Tajm_Fun.year;
  y mod 400 === 0 || y mod 4 === 0 && y mod 100 !== 0;
};

let rec same = (u: timeunit_, t1: time_, t2: time_): bool => {
  switch (u) {
  | Millisecond => t1 |> equal(t2)
  | Second =>
    Tajm_Fun.second(t1) == Tajm_Fun.second(t2) && same(Minute, t1, t2)
  | Minute =>
    Tajm_Fun.minute(t1) == Tajm_Fun.minute(t2) && same(Hour, t1, t2)
  | Hour => Tajm_Fun.hour(t1) == Tajm_Fun.hour(t2) && same(Day, t1, t2)
  | Day => Tajm_Fun.day(t1) == Tajm_Fun.day(t2) && same(Month, t1, t2)
  | Month => Tajm_Fun.month(t1) == Tajm_Fun.month(t2) && same(Year, t1, t2)
  | Year => Tajm_Fun.year(t1) == Tajm_Fun.year(t2)
  };
};

let sameYear = same(Year);
let sameMonth = same(Month);
let sameHour = same(Hour);
let sameMinute = same(Minute);
let sameSecound = same(Second);
let sameMillisecond = same(Millisecond);
