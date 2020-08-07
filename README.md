# Tajm [![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg?style=flat-square)](https://github.com/crholm/tajm/blob/master/LICENSE)

> A quite large lib for handling time in Reason

## API Reference
The api is inspierd by Golang time lib

Tajm does not differentiate between a date and a time (wall clock). Instead there is only one type, `time_`, which can be used for both dependent on your need

<details>
<summary><strong>Tajm</strong></summary>

#### `let zero = () => time_`
```reason 
Tajm.zero() |> Tajm.string |> Js.log
// 1970-01-01T00:00:00.000Z
```


#### `let now = () => time_`
```reason
Tajm.now() |> Tajm.string |> Js.log
// 2020-08-07T07:38:57.613Z
```
#### `let ofUnix: (f: float) => time_`
```reason 
Tajm.ofUnix(1585443600000.)  |> Tajm.string |> Js.log
// 2020-03-29T01:00:00.000Z
```

#### `let make: (~y, ~m, ~d, ~hour, ~min, ~sec, ~ms, loc) => time_`

```reason 
Tajm.make(
  ~y=2020,
  ~m=February,
  ~d=29,
  ~hour=15,
  ~min=32,
  ~sec=42,
  ~ms=123,
  Tajm.z,
)
|> Tajm.string
|> Js.log;
// 2020-02-29T15:32:42.123Z

```

#### `let compare: (t1: time_, t2: time_) => int`
```reason 
Tajm.zero |> Tajm.compare(Tajm.now()) |> Js.log
// 1
```

#### `let unix: (t: time_) => float`
```reason 
Tajm.now() |> Tajm.unix |> Js.log
// 1596788909886
```
#### `let toJs: (t: time_) => Js.Date.t`
```reason 
Tajm.now() |> Tajm.toJs |> Js.log
// 2020-08-07T08:29:14.241Z
```

#### `let add: (dur: duration_, t: time_) => time_`
```reason
Tajm.now()
|> tee2(Tajm.string, Js.log)
|> Tajm.add(Tajm.Duration.hour)
|> Tajm.string
|> Js.log
// 2020-08-07T08:35:18.066Z
// 2020-08-07T09:35:18.066Z
```

#### `let sub: (t1: time_, t2: time_) => duration_`
```reason 
let t1 = Tajm.now();
let t2 =
  t1 |> Tajm.add(Tajm.Duration.hour) |> Tajm.add(15. *. Tajm.Duration.minute);
Tajm.sub(t1, t2) |> Tajm.Duration.string |> Js.log
// -1h15m0s
```

#### `let since: (t: time_) => duration_`
```reason 
Tajm.now()
|> Tajm.add((-35.) *. Tajm.Duration.secound)
|> Tajm.since
|> Tajm.Duration.string
|> Js.log;
// 0h0m35s
```
#### `let until: (t: time_) => duration_`
```reason 
Tajm.now()
|> Tajm.add(2. *. Tajm.Duration.minute)
|> Tajm.until
|> Tajm.Duration.string
|> Js.log;
// 0h2m0s
```

#### `let truncate: (_m: duration_, t: time_) => time_`
```reason 
Tajm.now() |> Tajm.truncate(Tajm.Duration.minute) |> Tajm.string |> Js.log
// 2020-08-07T08:52:00.000Z
```


#### `let before: (t2: time_, t1: time_) => bool`
```reason 
Tajm.now()->Tajm.before(Tajm.zero) |> Js.log;
// false
```
#### `let after: (t2: time_, t1: time_) => bool`
```reason 
Tajm.now()->Tajm.after(Tajm.zero) |> Js.log;
// true
```

#### `let future: (t: time_) => bool`
```reason
Tajm.zero |> Tajm.future |> Js.log;
// false
```

#### `let past: (t: time_) => bool`
```reason
Tajm.zero |> Tajm.past |> Js.log;
// true
```


#### `let atUTC = (t: time_) => time_`
```reason 
Tajm.now()
|> Tajm.atUTC
|> Tajm.format("2006-01-02 15:04:05 Z07:00")
|> Js.log
// 2020-08-07 09:05:38 Z
```
#### `let atLocal = (t: time_) => time_`
```reason 
Tajm.now()
|> Tajm.atLocal
|> Tajm.format("2006-01-02 15:04:05 Z07:00")
|> Js.log
// 2020-08-07 11:05:38 +02:00
```

#### `let atFixed = (name: string, sec: int) => time_`
```reason
Tajm.now()
|> Tajm.atFixed("CEST", 2 * 60 * 60)
|> Tajm.format("2006-01-02 15:04:05 MST")
|> Js.log
// 2020-08-07 11:07:46 CEST
```

#### `let at = (loc: location_, t: time_) => time_`
```reason
Tajm.now()
|> Tajm.at(Tajm.local)
|> Tajm.format("2006-01-02 15:04:05 MST")
|> Js.log;
// 2020-08-07 11:10:21 Central European Summer Time
```


#### `let zone = (t: time_) => location_`
```reason
let zone = Tajm.now() |> Tajm.at(Tajm.local) |> Tajm.zone;
Tajm.zero
|> Tajm.at(zone)
|> Tajm.format("2006-01-02 15:04:05 Z07:00")
|> Js.log;
// 1970-01-01 01:00:00 +01:00
```

#### `let location: (name: string) => option(location_)`


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

</details>


<details>
<summary><strong>Tajm.Duration</strong></summary>

#### `let millisecond: duration_`
#### `let secound: duration_`
#### `let minute: duration_`
#### `let hour: duration_`

#### `let truncate: (m: duration_, d: duration_) => duration_`
#### `let string: (d: duration_) => string`
#### `let parse: (d: string) => duration_`

</details>

<details>
<summary><strong>Tajm.Is</strong></summary>

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

</details>

<details>
<summary><strong>Tajm.Conv</strong></summary>

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

</details>



<details>
<summary><strong>Types</strong></summary>

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

</details>
