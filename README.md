# Tajm [![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg?style=flat-square)](https://github.com/crholm/tajm/blob/master/LICENSE) [![npm](https://img.shields.io/npm/v/tajm.svg?style=flat-square&logo=npm)](https://www.npmjs.com/package/tajm) [![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat-square)](http://makeapullrequest.com)  

> Tajm, pronounced time, is ReasonML library to handle most needs when time and dates are needed.
>
The purpose of the library is to have a tool for handling times and date that is as 
uncoupled from `Js.Date` and provide the most common functionalities generally needed. Tajm supports time zone locations, such as `Europe/London`, when parsing and formatting dates. The IANA data needed for this and is compiled into the library (unused timezone data can be tree shooken out for production).   

## State 
>⚠ Please try it, but use with care. tajm is still a work in progress  
>

## Install 
```bash
npm install tajm --save
```
or 
```bash
yarn add tajm
```

Add `tajm` to `bs-dependencies` in your `bsconfig.json`:
```json 
{
  "bs-dependencies": [
    "tajm"
  ]
}
```

## API Reference
The api is inspierd by Golang time lib

Tajm does not differentiate between a date and a time (wall clock). Instead there is only one type, `time_`, which can be used for both dependent on your need

<details open>
<summary><strong>Tajm</strong></summary>

#### `let zero: () => time_`
```reason 
Tajm.zero() |> Tajm.string |> Js.log
// 1970-01-01T00:00:00.000Z
```


#### `let now = () => time_`
Creates a time of current time in UTC
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
Adds a duration to a time and returns the new time.

> ⚠ Do not use longer periods, days, months and so on. This since eg. hour * 24 * 365 is not nessasary a year,
 due to leapyears and leap secounds. Insead use `addDate`

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
Subtracts t1 from t2 and returns the difference as a duration (ms)

```reason 
let t1 = Tajm.now();
let t2 =
  t1 |> Tajm.add(Tajm.Duration.hour) |> Tajm.add(15. *. Tajm.Duration.minute);
Tajm.sub(t1, t2) |> Tajm.Duration.string |> Js.log
// -1h15m0s
```

#### `let since: (t: time_) => duration_`
Returns the duration elapsed from t
```reason 
Tajm.now()
|> Tajm.add((-35.) *. Tajm.Duration.secound)
|> Tajm.since
|> Tajm.Duration.string
|> Js.log;
// 0h0m35s
```
#### `let until: (t: time_) => duration_`
Returns the dutaion from now until t 
```reason 
Tajm.now()
|> Tajm.add(2. *. Tajm.Duration.minute)
|> Tajm.until
|> Tajm.Duration.string
|> Js.log;
// 0h2m0s
```

#### `let truncate: (_m: duration_, t: time_) => time_`
Truncate the time a with the specified duration,
   eg. if duration second, seconds and milliseconds is truncated from time 
```reason 
Tajm.now() |> Tajm.truncate(Tajm.Duration.minute) |> Tajm.string |> Js.log
// 2020-08-07T08:52:00.000Z
```


#### `let before: (t1: time_, t2: time_) => bool`
Is time1 before time2 
```reason 
Tajm.now()->Tajm.before(Tajm.zero) |> Js.log;
// false
```
#### `let after: (t1: time_, t2: time_) => bool`
Is time1 after time2 
```reason 
Tajm.now()->Tajm.after(Tajm.zero) |> Js.log;
// true
```

#### `let future: (t: time_) => bool`
Is time in the future
```reason
Tajm.zero |> Tajm.future |> Js.log;
// false
```

#### `let past: (t: time_) => bool`
Is time in the past
```reason
Tajm.zero |> Tajm.past |> Js.log;
// true
```


#### `let atUTC = (t: time_) => time_`
Sets the location to UTC
```reason 
Tajm.now()
|> Tajm.atUTC
|> Tajm.format("2006-01-02 15:04:05 Z07:00")
|> Js.log
// 2020-08-07 09:05:38 Z
```
#### `let atLocal = (t: time_) => time_`
Sets the location to the Local time zone
```reason 
Tajm.now()
|> Tajm.atLocal
|> Tajm.format("2006-01-02 15:04:05 Z07:00")
|> Js.log
// 2020-08-07 11:05:38 +02:00
```

#### `let atFixed = (name: string, sec: int) => time_`
Sets the location to a fixed offset
```reason
Tajm.now()
|> Tajm.atFixed("CEST", 2 * 60 * 60)
|> Tajm.format("2006-01-02 15:04:05 MST")
|> Js.log
// 2020-08-07 11:07:46 CEST
```

#### `let at = (loc: location_, t: time_) => time_`
Sets the location to the time
```reason
Tajm.now()
|> Tajm.at(Tajm.local)
|> Tajm.format("2006-01-02 15:04:05 MST")
|> Js.log;
// 2020-08-07 11:10:21 Central European Summer Time
```


#### `let zone = (t: time_) => location_`
Returns the location of the time
```reason
let zone = Tajm.now() |> Tajm.at(Tajm.local) |> Tajm.zone;
Tajm.zero
|> Tajm.at(zone)
|> Tajm.format("2006-01-02 15:04:05 Z07:00")
|> Js.log;
// 1970-01-01 01:00:00 +01:00
```

#### `let location: (name: string) => option(location_)`
Tries to load retrieve the location by name, eg UTC, Local and Europe/Stockholm

For IANA locations, such as Europe/Stockholm, a time zone database must be loaded, view section on IANA

#### `let weekday: (t: time_) => weekday_`
Returns the weekday of the provided time
```reason
Tajm.now() |> Tajm.weekday |> Tajm.Conv.stringOfWeekday |> Js.log
// Thursday
```

#### `let year: (t: time_) => int`
Returns the year of the provided time
```reason
Tajm.zero |> Tajm.year |> Js.log;
// 1970
```

Returns the month of the provided time
#### `let month: (t: time_) => month_`
```reason
Tajm.zero |> Tajm.month |> Tajm.Conv.stringOfMonth |> Js.log
// January
```

#### `let day: (t: time_) => int`
Returns the day in the month of the provided time 
```reason 
Tajm.zero |> Tajm.day |> Js.log
// 1
```

#### `let hour: (t: time_) => int`
Returns the hour of the provided time
```reason
Tajm.zero |> Tajm.hour |> Js.log
// 0
```
#### `let minute: (t: time_) => int`
Returns the minute of the provided time
```reason
Tajm.zero |> Tajm.minute |> Js.log
// 0
```
#### `let second: (t: time_) => int`
Returns the second of the provided time
```reason
Tajm.zero |> Tajm.second |> Js.log
// 0
```
#### `let millisecond: (t: time_) => int`
Returns the millisecond of the provided time
```reason
Tajm.zero |> Tajm.millisecond |> Js.log
// 0
```

#### `let yearDay: (t: time_) => int`
Returns day of the year, where 01/jan is 1 and 31/dec is 365
```reason
Tajm.now() |> Tajm.yearDay |> Js.log
// 220
```

#### `let week: (t: time_) => int`
Returns the week of the year, 1-53
```Tajm.now() |> Tajm.week |> Js.log
   32
```

#### `let clock: (t: time_) => (int, int, int)`
Returns the wall clock as three ints, (hh mm ss)
```reason
Tajm.now() |> Tajm.clock |> Js.log
// [ 9, 29, 43 ]
```

#### `let date: (t: time_) => (int, month_, int)`
Returns the calender date, (year month day)
```
Tajm.now() |> Tajm.date |> Js.log
// [ 2020, 7, 7 ]
```

#### `let set: (~y, ~m, ~d, ~hour, ~min, ~sec, ~ms, _time) => time_`
Sets any specific parameters to a time and defaults to current
```reason
Tajm.now() |> Tajm.set(~d=1) |> Tajm.string |> Js.log
// 1999-08-01T09:32:06.946Z
```

#### `let setDate: (y: int, m: month_, d: int, t: time_) => time_`
Sets any specific date to a time and keep hour, min, sec, ms from original
```reason
Tajm.now() |> Tajm.setDate(1999, August, 1) |> Tajm.string |> Js.log
// 1999-08-01T09:33:27.010Z
```

#### `let setClock: (hour: int, min: int, sec: int, t: time_) => time_`
Sets any specific wall clock to a time and keep year, month and day from original
```reason
Tajm.now() |> Tajm.setClock(10, 10, 10) |> Tajm.string |> Js.log
// 2020-08-07T10:10:10.552Z
```

#### `let addDate: (~y, ~m, ~d, t: time_) => time_`
Add lager time units, year, month and day, to a time
```reason
Tajm.zero |> Tajm.addDate(~y=1, ~m=2, ~d=3) |> Tajm.string |> Js.log
// 1971-03-04T00:00:00.000Z
```

#### `let startOf = (u: timeunit_, t: time_) => time_`
```reason
Tajm.now() |> Tajm.startOf(Day) |> Tajm.string |> Js.log
// 2020-08-07T00:00:00.000Z
```
#### `let endOf = (u: timeunit_, t: time_) => time_`
```reason
Tajm.now() |> Tajm.endOf(Day) |> Tajm.string |> Js.log
// 2020-08-07T23:59:59.999Z
```

#### `let format: (layout: string, t: time_) => string`
Formats a time to a string in accordance to the layout string. The layout is borrowed from [golang time library](https://golang.org/src/time/format.go?s=16029:16071#L92) with some addition and changes. It provides a human friendly way for developers to see what the expected output will be.

```reason
Tajm.now() |> Tajm.format(Tajm.fmtRFC850) |> Tajm.string |> Js.log
// Friday, 07-Aug-20 09:40:55 UTC
Tajm.now() |> Tajm.format("2006-01-02T15:04:05Z07:00") |> Tajm.string |> Js.log
// 2020-08-07T09:40:55Z
```

<details open>
<summary><strong>Pre defined Layouts</strong></summary>

```reason
Tajm.fmtANSIC           = "Mon Jan _2 15:04:05 2006";
Tajm.fmtUnixDate        = "Mon Jan _2 15:04:05 MST 2006";
Tajm.fmtRubyDate        = "Mon Jan 02 15:04:05 -0700 2006";
Tajm.fmtRFC822          = "02 Jan 06 15:04 MST";
Tajm.fmtRFC822Z         = "02 Jan 06 15:04 -0700"; // RFC822 with numeric zone
Tajm.fmtRFC850          = "Monday, 02-Jan-06 15:04:05 MST";
Tajm.fmtRFC1123         = "Mon, 02 Jan 2006 15:04:05 MST";
Tajm.fmtRFC1123Z        = "Mon, 02 Jan 2006 15:04:05 -0700"; // RFC1123 with numeric zone
Tajm.fmtRFC3339         = "2006-01-02T15:04:05Z07:00";
Tajm.fmtRFC3339ms       = "2006-01-02T15:04:05.999Z07:00";
Tajm.fmtISOTimestamp    = "2006-01-02T15:04:05Z07:00"; // ISO 8601
Tajm.fmtISOTimestampMs  = "2006-01-02T15:04:05.000Z07:00"; // ISO 8601
Tajm.fmtISOWeek         = "2006-W49"; // ISO 8601
Tajm.fmtISOWeekDay      = "2006-W49-7"; // ISO 8601
Tajm.fmtKitchen         = "3:04PM";
```
</details>

<details open>
<summary><strong>Layout Reference</strong></summary>

```reason
| LongMonth         // = "January";
| Month             // = "Jan";
| NumMonth          // = "1";
| ZeroMonth         // = "01"; Padd left with 0
| RightMonth        // = "_1"; Right aligns
| LongWeekDay       // = "Monday";
| WeekDay           // = "Mon";
| NumWeekDay        // = "7";
| Day               // = "2";
| ZeroDay           // = "02"; Padd left with 0
| RightDay          // = "_2"; Right aligns
| YearDay           // = "8"
| ZeroYearDay       // = "008";
| RightYearDay      // = "__8"; Right aligns
| Hour              // = "15";
| Hour12            // = "3";
| ZeroHour12        // = "03"; Padd left with 0
| RightHour12       // = "_3"; Right aligns
| Minute            // = "4";
| ZeroMinute        // = "04"; Padd left with 0
| RightMinute       // = "_4"; Right aligns
| YearWeek          // = "49";
| RightYearWeek     // = "_9" Right aligns
| ZeroYearWeek      // = "09" Padd left with 0
| Second            // = "5";
| ZeroSecond        // = "05"; Padd left with 0
| RightSecond       // = "_5"; Right aligns
| LongYear          // = "2006";
| Year              // = "06"; 
| PM(bool)          // = "PM" | "pm";
| TZ                // = "MST"; Poor interop with local timezone due to the js Date api behavior
| ISO8601TZ         // = "Z0700";  prints Z for UTC
| ISO8601ShortTZ    // = "Z07";
| ISO8601ColonTZ    // = "Z07:00";  prints Z for UTC
| NumTZ             // = "-0700";  always numeric
| NumShortTZ        // = "-07"; always numeric
| NumColonTZ        // = "-07:00";
| FracSecond0(int)  // = ".0"; ".00", ".000", ... , trailing zeros included
| FracSecond9(int)  // = ".9"; ".99", ".999" ... , trailing zeros omited
```
</details>

#### `let parse: (layout: string, t: string) => time_`
Parses a string into a time given a specific layout
```reason
"2020-08-07 09:40:55 +02:00"
|> Tajm.parse("2006-01-02 15:04:05 Z07:00")
|> (
  fun
  | Some(t) => t |> Tajm.string
  | None => "Failed to parse"
)
|> Js.log;
// 2020-08-07T07:40:55.000Z
```

#### `let string: (t: time_) => string`
Returns a ISO Timestamp formatted string of the time
```reason
Tajm.now() |> Tajm.string |> Js.log;
// 2020-08-07T09:47:58.315Z
```

</details>


<details open>
<summary><strong>Tajm.Duration</strong></summary>

#### `let millisecond: duration_`
#### `let secound: duration_`
#### `let minute: duration_`
#### `let hour: duration_`

#### `let truncate: (m: duration_, d: duration_) => duration_`
#### `let string: (d: duration_) => string`
#### `let parse: (d: string) => duration_`

</details>

<details open>
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

<details open>
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



<details open>
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



## Contribute
```reason
git clone https://github.com/crholm/tajm.git
cd tajm
npm install
npm run build
npm run test
```

## TODO
* More tests 
* Finish Tajm.parse 
* Expose IANA properly
* Finish Documentation