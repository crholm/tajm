open Jest;
open Expect;
open Tajm;

describe("Time operations", () => {
  test("add", () => {
    let t = now();
    t
    |> add(Duration.secound)
    |> add(Duration.millisecond)
    |> unix
    |> expect
    |> toBe((t |> unix) +. Duration.secound +. Duration.millisecond);
  });
  test("since", () => {
    now()
    |> add(-. Duration.secound)
    |> since
    |> expect
    |> toBeGreaterThanOrEqual(Duration.secound)
  });
  test("until", () => {
    now()
    |> add(Duration.secound)
    |> until
    |> expect
    |> toBeLessThanOrEqual(Duration.secound)
  });

  test("print time", () => {
    now()
    |> add(Duration.secound)
    |> until
    |> expect
    |> toBe(Duration.secound)
  });

  test("print duration", () => {
    now()
    |> add(Duration.secound)
    |> until
    |> expect
    |> toBe(Duration.secound)
  });

  test("truncate ms", () => {
    let _d =
      Duration.millisecond
      +. Duration.secound
      +. Duration.minute
      +. Duration.hour
      +. 0.1;
    _d
    |> Duration.truncate(Duration.millisecond)
    |> Duration.string
    |> expect
    |> toBe("1h1m1.001s");
  });
  test("truncate s", () => {
    let _d =
      Duration.millisecond
      +. Duration.secound
      +. Duration.minute
      +. Duration.hour;
    _d
    |> Duration.truncate(Duration.secound)
    |> Duration.string
    |> expect
    |> toBe("1h1m1s");
  });
  test("truncate m", () => {
    let _d =
      Duration.millisecond
      +. Duration.secound
      +. Duration.minute
      +. Duration.hour;
    _d
    |> Duration.truncate(Duration.minute)
    |> Duration.string
    |> expect
    |> toBe("1h1m0s");
  });
  test("truncate h", () => {
    let _d =
      Duration.millisecond
      +. Duration.secound
      +. Duration.minute
      +. Duration.hour;
    _d
    |> Duration.truncate(Duration.hour)
    |> Duration.string
    |> expect
    |> toBe("1h0m0s");
  });
  test("dur parse0", () => {
    let d =
      0.9876
      +. Duration.millisecond
      +. Duration.secound
      +. Duration.minute
      +. Duration.hour;

    d |> Duration.string |> expect |> toBe("1h1m1.001s");
  });
  test("dur parse1", () => {
    let d = "1h1m1s";
    d |> Duration.parse |> Duration.string |> expect |> toBe(d);
  });
  test("dur parse2", () => {
    let d = "100h10m10.1s";
    d |> Duration.parse |> Duration.string |> expect |> toBe(d);
  });
  test("dur parse3", () => {
    let d = "100h10m10.01s";
    d |> Duration.parse |> Duration.string |> expect |> toBe(d);
  });
  test("dur parse4", () => {
    let d = "100h10m10.001s";
    d |> Duration.parse |> Duration.string |> expect |> toBe(d);
  });
  test("dur parse5", () => {
    let d = "100h10m10.00100s";
    let exp = "100h10m10.001s";

    d |> Duration.parse |> Duration.string |> expect |> toBe(exp);
  });

  test("intToWeekday", () => {
    let m1 = 0 |> Conv.weekdayOfInt;
    let m2 = 6 |> Conv.weekdayOfInt;
    (m1 |> Conv.stringOfWeekday)
    ++ (m2 |> Conv.stringOfWeekday)
    |> expect
    |> toBe("MondaySunday");
  });
  test("getWeekday", () => {
    let m1 = make(~y=2020, ~m=July, ~d=20, z) |> weekday;
    let m2 = make(~y=2020, ~m=July, ~d=26, z) |> weekday;

    (m1 |> Conv.stringOfWeekday)
    ++ (m2 |> Conv.stringOfWeekday)
    |> expect
    |> toBe("MondaySunday");
  });

  test("week1", () => {
    let t = make(~y=2020, ~m=January, ~d=1, z);

    t |> week |> expect |> toBe(1);
  });
  test("week2", () => {
    let t1 = make(~y=2020, ~m=December, ~d=31, z) |> week |> string_of_int;

    t1 |> expect |> toBe("53");
  });
  test("weekFull1", () => {
    let t1 = make(~y=2020, ~m=July, ~d=20, z) |> week |> string_of_int;
    let t2 = make(~y=2020, ~m=July, ~d=21, z) |> week |> string_of_int;
    let t3 = make(~y=2020, ~m=July, ~d=22, z) |> week |> string_of_int;
    let t4 = make(~y=2020, ~m=July, ~d=23, z) |> week |> string_of_int;
    let t5 = make(~y=2020, ~m=July, ~d=24, z) |> week |> string_of_int;
    let t6 = make(~y=2020, ~m=July, ~d=25, z) |> week |> string_of_int;
    let t7 = make(~y=2020, ~m=July, ~d=26, z) |> week |> string_of_int;
    t1
    ++ " "
    ++ t2
    ++ " "
    ++ t3
    ++ " "
    ++ t4
    ++ " "
    ++ t5
    ++ " "
    ++ t6
    ++ " "
    ++ t7
    |> expect
    |> toBe("30 30 30 30 30 30 30");
  });

  test("weekFull2", () => {
    let t1 = make(~y=2020, ~m=February, ~d=24, z) |> week |> string_of_int;
    let t2 = make(~y=2020, ~m=February, ~d=25, z) |> week |> string_of_int;
    let t3 = make(~y=2020, ~m=February, ~d=26, z) |> week |> string_of_int;
    let t4 = make(~y=2020, ~m=February, ~d=27, z) |> week |> string_of_int;
    let t5 = make(~y=2020, ~m=February, ~d=28, z) |> week |> string_of_int;
    let t6 = make(~y=2020, ~m=February, ~d=29, z) |> week |> string_of_int;
    let t7 = make(~y=2020, ~m=March, ~d=1, z) |> week |> string_of_int;

    t1
    ++ " "
    ++ t2
    ++ " "
    ++ t3
    ++ " "
    ++ t4
    ++ " "
    ++ t5
    ++ " "
    ++ t6
    ++ " "
    ++ t7
    |> expect
    |> toBe("9 9 9 9 9 9 9");
  });

  test("StartOfMs", () => {
    let t =
      make(
        ~y=2020,
        ~m=February,
        ~d=24,
        ~hour=13,
        ~min=16,
        ~sec=42,
        ~ms=932,
        z,
      );

    t
    |> startOf(Millisecond)
    |> string
    |> expect
    |> toBe("2020-02-24T13:16:42.932Z");
  });

  test("StartOfSec", () => {
    let t =
      make(
        ~y=2020,
        ~m=February,
        ~d=24,
        ~hour=13,
        ~min=16,
        ~sec=42,
        ~ms=932,
        z,
      );

    t
    |> startOf(Second)
    |> string
    |> expect
    |> toBe("2020-02-24T13:16:42.000Z");
  });
  test("StartOfMinute", () => {
    let t =
      make(
        ~y=2020,
        ~m=February,
        ~d=24,
        ~hour=13,
        ~min=16,
        ~sec=42,
        ~ms=932,
        z,
      );

    t
    |> startOf(Minute)
    |> string
    |> expect
    |> toBe("2020-02-24T13:16:00.000Z");
  });
  test("StartOfHour", () => {
    let t =
      make(
        ~y=2020,
        ~m=February,
        ~d=24,
        ~hour=13,
        ~min=16,
        ~sec=42,
        ~ms=932,
        z,
      );

    t
    |> startOf(Hour)
    |> string
    |> expect
    |> toBe("2020-02-24T13:00:00.000Z");
  });

  test("StartOfDay", () => {
    let t =
      make(
        ~y=2020,
        ~m=February,
        ~d=24,
        ~hour=13,
        ~min=16,
        ~sec=42,
        ~ms=932,
        z,
      );

    t |> startOf(Day) |> string |> expect |> toBe("2020-02-24T00:00:00.000Z");
  });

  test("StartOfMonth", () => {
    let t =
      make(
        ~y=2020,
        ~m=February,
        ~d=24,
        ~hour=13,
        ~min=16,
        ~sec=42,
        ~ms=932,
        z,
      );

    t
    |> startOf(Month)
    |> string
    |> expect
    |> toBe("2020-02-01T00:00:00.000Z");
  });
  test("StartOfYear", () => {
    let t =
      make(
        ~y=2020,
        ~m=February,
        ~d=24,
        ~hour=13,
        ~min=16,
        ~sec=42,
        ~ms=932,
        z,
      );

    t
    |> startOf(Year)
    |> string
    |> expect
    |> toBe("2020-01-01T00:00:00.000Z");
  });

  test("EndOfMs", () => {
    let t =
      make(
        ~y=2020,
        ~m=February,
        ~d=24,
        ~hour=13,
        ~min=16,
        ~sec=42,
        ~ms=932,
        z,
      );

    t
    |> endOf(Millisecond)
    |> string
    |> expect
    |> toBe("2020-02-24T13:16:42.932Z");
  });

  test("EndOfSec", () => {
    let t =
      make(
        ~y=2020,
        ~m=February,
        ~d=24,
        ~hour=13,
        ~min=16,
        ~sec=42,
        ~ms=932,
        z,
      );

    t
    |> endOf(Second)
    |> string
    |> expect
    |> toBe("2020-02-24T13:16:42.999Z");
  });
  test("EndOfMin", () => {
    let t =
      make(
        ~y=2020,
        ~m=February,
        ~d=24,
        ~hour=13,
        ~min=16,
        ~sec=42,
        ~ms=932,
        z,
      );

    t
    |> endOf(Minute)
    |> string
    |> expect
    |> toBe("2020-02-24T13:16:59.999Z");
  });

  test("EndOfHour", () => {
    let t =
      make(
        ~y=2020,
        ~m=February,
        ~d=24,
        ~hour=13,
        ~min=16,
        ~sec=42,
        ~ms=932,
        z,
      );

    t |> endOf(Hour) |> string |> expect |> toBe("2020-02-24T13:59:59.999Z");
  });

  test("EndOfDay", () => {
    let t =
      make(
        ~y=2020,
        ~m=February,
        ~d=24,
        ~hour=13,
        ~min=16,
        ~sec=42,
        ~ms=932,
        z,
      );

    t |> endOf(Day) |> string |> expect |> toBe("2020-02-24T23:59:59.999Z");
  });

  test("EndOfMonth", () => {
    let t =
      make(
        ~y=2020,
        ~m=February,
        ~d=24,
        ~hour=13,
        ~min=16,
        ~sec=42,
        ~ms=932,
        z,
      );

    t |> endOf(Month) |> string |> expect |> toBe("2020-02-29T23:59:59.999Z");
  });
  test("EndOfYear", () => {
    let t =
      make(
        ~y=2020,
        ~m=February,
        ~d=24,
        ~hour=13,
        ~min=16,
        ~sec=42,
        ~ms=932,
        z,
      );

    t |> endOf(Year) |> string |> expect |> toBe("2020-12-31T23:59:59.999Z");
  });

  test("Print", () => {
    // Js.log(now() |> addDate(~y=1, ~m=15, ~d=50) |> string);

    // Js.log(Tajm.now() |> unix |> Int64.of_float);
    true |> expect |> toBe(true);
  });
});
