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

#### `let unix: (t: time_) => float`
#### `let ofUnix: (f: float) => time_`
#### `let make: (~y,~m, ~d, ~hour, ~min, ~sec, ~ms, loc) => time_`



#### `let format: (format: string, t: time_) => string`
#### `let parse: (format: string, t: string) => time_`
#### `let string: (t: time_) => string`
#### `let toJs: (t: time_) => Js.Date.t`

### Tajm.Duration


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
