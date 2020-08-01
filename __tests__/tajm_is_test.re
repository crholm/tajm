open Jest;
open Expect;

describe("Tajm Is", () => {
  let t =
    Tajm.make(
      ~y=2020,
      ~m=Tajm.August,
      ~d=1,
      ~hour=12,
      ~min=32,
      ~sec=42,
      ~ms=532,
      Tajm.z,
    );

  test("zero", () => {
    {...t, t: 0L} |> Tajm.Is.zero |> expect |> toBe(true)
  });
  test("zero", () => {
    Tajm.zero |> Tajm.Is.zero |> expect |> toBe(true)
  });
  test("zero not", () => {
    t |> Tajm.Is.zero |> expect |> toBe(false)
  });

  test("day", () => {
    t |> Tajm.Is.saturday |> expect |> toBe(true)
  });
  test("day not", () => {
    t |> Tajm.Is.sunday |> expect |> toBe(false)
  });
  test("weekend", () => {
    t |> Tajm.Is.weekend |> expect |> toBe(true)
  });
  test("leapyear0", () => {
    let t = Tajm.make(~y=2020, ~m=Tajm.August, ~d=1, Tajm.z);

    t |> Tajm.Is.leapYear |> expect |> toBe(true);
  });
  test("leapyear1", () => {
    let t = Tajm.make(~y=2019, ~m=Tajm.August, ~d=1, Tajm.z);
    t |> Tajm.Is.leapYear |> expect |> toBe(false);
  });
  test("leapyear2", () => {
    let t = Tajm.make(~y=2100, ~m=Tajm.August, ~d=1, Tajm.z);
    t |> Tajm.Is.leapYear |> expect |> toBe(false);
  });
  test("same Millisecond", () => {
    t |> Tajm.Is.sameMillisecond(t) |> expect |> toBe(true)
  });
  test("same Millisecond not", () => {
    t
    |> Tajm.set(~ms=0)
    |> Tajm.Is.sameMillisecond(t)
    |> expect
    |> toBe(false)
  });
  test("same Second", () => {
    t |> Tajm.set(~ms=0) |> Tajm.Is.sameSecound(t) |> expect |> toBe(true)
  });
  test("same Second not", () => {
    t |> Tajm.set(~sec=0) |> Tajm.Is.sameSecound(t) |> expect |> toBe(false)
  });

  test("same Minute", () => {
    t |> Tajm.set(~sec=0) |> Tajm.Is.sameMinute(t) |> expect |> toBe(true)
  });
  test("same Minute not", () => {
    t |> Tajm.set(~min=0) |> Tajm.Is.sameMinute(t) |> expect |> toBe(false)
  });
  test("same Hour", () => {
    t |> Tajm.set(~min=0) |> Tajm.Is.sameHour(t) |> expect |> toBe(true)
  });
  test("same Hour not", () => {
    t |> Tajm.set(~hour=0) |> Tajm.Is.sameHour(t) |> expect |> toBe(false)
  });
  test("same Day", () => {
    t |> Tajm.set(~hour=0) |> Tajm.Is.sameDay(t) |> expect |> toBe(true)
  });
  test("same Day not", () => {
    t |> Tajm.set(~d=2) |> Tajm.Is.sameDay(t) |> expect |> toBe(false)
  });
  test("same Month", () => {
    t |> Tajm.set(~d=2) |> Tajm.Is.sameMonth(t) |> expect |> toBe(true)
  });
  test("same Month not", () => {
    t
    |> Tajm.set(~m=Tajm.September)
    |> Tajm.Is.sameMonth(t)
    |> expect
    |> toBe(false)
  });
  test("same Year", () => {
    t
    |> Tajm.set(~m=Tajm.September)
    |> Tajm.Is.sameYear(t)
    |> expect
    |> toBe(true)
  });
  test("same Year not", () => {
    t |> Tajm.set(~y=2021) |> Tajm.Is.sameYear(t) |> expect |> toBe(false)
  });
});
