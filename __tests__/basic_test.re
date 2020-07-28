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
    |> toBe(t.t +. Duration.secound +. Duration.millisecond);
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
    |> Duration.toString
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
    |> Duration.toString
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
    |> Duration.toString
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
    |> Duration.toString
    |> expect
    |> toBe("1h0m0s");
  });
  test("dur parse", () => {
    let d = "1h1m1s";
    d |> Duration.parse |> Duration.toString |> expect |> toBe(d);
  });
  test("dur parse2", () => {
    let d = "100h10m10.1s";
    d |> Duration.parse |> Duration.toString |> expect |> toBe(d);
  });
  test("dur parse3", () => {
    let d = "100h10m10.01s";
    d |> Duration.parse |> Duration.toString |> expect |> toBe(d);
  });
  test("dur parse4", () => {
    let d = "100h10m10.001s";
    d |> Duration.parse |> Duration.toString |> expect |> toBe(d);
  });
  test("dur parse4", () => {
    let d = "100h10m10.00100s";
    let exp = "100h10m10.001s";

    d |> Duration.parse |> Duration.toString |> expect |> toBe(exp);
  });

  test("Print", () => {
    Js.log(now() |> addDate(1, 15, 50) |> toString);

    true |> expect |> toBe(true);
  });
});
