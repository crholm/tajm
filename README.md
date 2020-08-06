# Tajm [![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg?style=flat-square)](https://github.com/crholm/tajm/blob/master/LICENSE)

> A quite large lib for handling time in Reason

## API Reference
The api is inspierd by Golang time lib

Tajm does not differentiate between a date and a time (wall clock). Instead there is only one type, time_, which can be used for both dependent on your need

### Tajm


#### `let compare: (t1: time_, t2: time_) => int`
#### `let at = (loc: location_, t: time_) => time_`

#### `let atLocal = (t: time_) => time_`
#### `let atUTC = (t: time_) => time_`
#### `let atFixed = (t: time_) => time_`
#### `let zone = (t: time_) => location_`
#### `let location: (name: string) => option(location_)`

#### `let zero = () => time_`
#### `let now = () => time_`
#### `let unix: (t: time_) => float`
#### `let ofUnix: (f: float) => time_`
#### `let make: (~y, ~m, ~d, ~hour, ~min, ~sec, ~ms, loc) => time_`


#### `let add: (dur: duration_, t: time_) => time_`
#### `let sub: (t1: time_, t2: time_) => duration_`

#### `let since: (t: time_) => duration_`
#### `let until: (t: time_) => duration_`
#### `let truncate: (_m: duration_, t: time_) => time_`

#### `let before: (t2: time_, t1: time_) => bool`
#### `let after: (t2: time_, t1: time_) => bool`

#### `let future: (t: time_) => bool`
#### `let past: (t: time_) => bool`

#### `let weekday: (t: time_) => weekday_`
#### `let year: (t: time_) => int`
#### `let month: (t: time_) => month_`
#### `let day: (t: time_) => int`
#### `let hour: (t: time_) => int`
#### `let minute: (t: time_) => int`
#### `let second: (t: time_) => int`
#### `let millisecond: (t: time_) => int`

#### `let yearDay: (t: time_) => int`
#### `let week: (t: time_) => int`
#### `let clock: (t: time_) => (int, int, int)`
#### `let date: (t: time_) => (int, month_, int)`

#### `let set: (~y, ~m, ~d, ~hour, ~min, ~sec, ~ms, _time) => time_`
#### `let setDate: (y: int, m: month_, d: int, t: time_) => time_`
#### `let setClock: (hour: int, min: int, sec: int, t: time_) => time_`

#### `let addDate: (~y, ~m, ~d, t: time_) => time_`
#### `let startOf = (u: timeunit_, t: time_) => time_`
#### `let endOf = (u: timeunit_, t: time_) => time_`

#### `let format: (format: string, t: time_) => string`
#### `let parse: (format: string, t: string) => time_`
#### `let string: (t: time_) => string`
#### `let toJs: (t: time_) => Js.Date.t`

### Tajm.Duration

#### `let millisecond: duration_`
#### `let secound: duration_`
#### `let minute: duration_`
#### `let hour: duration_`

#### `let truncate: (m: duration_, d: duration_) => duration_`
#### `let string: (d: duration_) => string`
#### `let parse: (d: string) => duration_`

### Tajm.Is
#### `let  equal: (t1: time_, t2: time_) => bool`
#### `let  zero: (t: time_) => bool`

#### `let  year: (y: int, t: time_) => bool`
#### `let  month: (m: month_, t: time_) => bool`
#### `let  day: (d: int, t: time_) => bool`
#### `let  hour: (h: int, t: time_) => bool`
#### `let  minute: (m: int, t: time_) => bool`
#### `let  second: (s: int, t: time_) => bool`
#### `let  millisecond: (ms: int, t: time_) => bool`

#### `let  weekday: (w: weekday_, t: time_) => bool`
#### `let  monday: (t: time_) => bool`
#### `let  tuesday: (t: time_) => bool`
#### `let  wednesday: (t: time_) => bool`
#### `let  thursday: (t: time_) => bool`
#### `let  friday: (t: time_) => bool`
#### `let  saturday: (t: time_) => bool`
#### `let  sunday: (t: time_) => bool`

#### `let  weekend: (t: time_) => bool`
#### `let  leapYear: (t: time_) => bool`

#### `let  sameYear: (t: time_) => bool`
#### `let  sameMonth: (t: time_) => bool`
#### `let  sameDay: (t: time_) => bool`
#### `let  sameHour: (t: time_) => bool`
#### `let  sameMinute: (t: time_) => bool`
#### `let  sameSecond: (t: time_) => bool`
#### `let  sameMillisecond: (t: time_) => bool`


### Tajm.Conv
#### `let stringOfWeekday: (weekday: weekday_) => string`
#### `let stringOfWeekdayShort: (weekday: weekday_) => string`
#### `let intOfWeekday: (weekday: weekday_) => int`
#### `let weekdayOfInt: (i: int) => weekday_`
#### `let intOfMonth: (month: month_) => int`
#### `let monthOfInt: (i: int) => month_`
#### `let monthOfStringShort: (m: string) => month_`
#### `let stringOfMonth: (month: month_) => string`
#### `let stringShortOfMonth: (month: month_) => string`
#### `let daysInYear: (year: int) => int`
#### `let daysInMonth: (year: int, month: month_) => int`


### Types
```reason
type time_ = {
  t: int64,
  loc: location_,
};
```


```reason
type duration_ = float;
```


```reason
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
```


```reason
type weekday_ =
  | Monday
  | Tuesday
  | Wednesday
  | Thursday
  | Friday
  | Saturday
  | Sunday;
  ```

```reason
type location_ =
  | Fixed(string, int)     // (name, offset) in secound, Fixed("CET", 3600)
  | Local                  // Uses the local location defined by the environment
  | IANA(Tajm_Iana_Tz.tz); // A IANA location, eg Europe/London
```
