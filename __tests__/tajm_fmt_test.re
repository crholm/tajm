open Jest;
open Expect;
open Tajm;

let t1 =
  make(~y=2020, ~m=February, ~d=29, ~hour=13, ~min=14, ~sec=35, ~ms=30, z);

describe("Tajm fmt", () => {
  test("fmtRFC3339ms", () => {
    let exp = "2020-02-29T13:14:35.03Z";
    t1 |> format(fmtRFC3339ms) |> expect |> toBe(exp);
  });
  test("fmtRFC3339", () => {
    let exp = "2020-02-29T13:14:35Z";
    t1 |> format(fmtRFC3339) |> expect |> toBe(exp);
  });
  test("fmtANSIC1", () => {
    let exp = "Sat Feb 29 13:14:35 2020";
    t1 |> format(fmtANSIC) |> expect |> toBe(exp);
  });
  test("fmtANSIC2", () => {
    let t = t1 |> setDate(2020, February, 4);
    let exp = "Tue Feb  4 13:14:35 2020";
    t |> format(fmtANSIC) |> expect |> toBe(exp);
  });
  test("fmtUnixDate", () => {
    let exp = "Sat Feb 29 13:14:35 UTC 2020";
    t1 |> format(fmtUnixDate) |> expect |> toBe(exp);
  });
  test("fmtRFC850", () => {
    let exp = "Saturday, 29-Feb-20 13:14:35 UTC";
    t1 |> format(fmtRFC850) |> expect |> toBe(exp);
  });

  test("fmtKitchenPM1", () => {
    let exp = "1:14PM";
    t1 |> format(fmtKitchen) |> expect |> toBe(exp);
  });
  test("fmtKitchenPM2", () => {
    let t = t1 |> setClock(12, 14, 0);
    let exp = "12:14PM";
    t |> format(fmtKitchen) |> expect |> toBe(exp);
  });
  test("fmtKitchenAM1", () => {
    let t = t1 |> setClock(5, 14, 0);
    let exp = "5:14AM";
    t |> format(fmtKitchen) |> expect |> toBe(exp);
  });
  test("fmtKitchenAM2", () => {
    let t = t1 |> setClock(0, 14, 0);
    let exp = "12:14AM";
    t |> format(fmtKitchen) |> expect |> toBe(exp);
  });
  test("fmtRFC3339tz", () => {
    let exp = "2020-02-29T14:44:35+01:30";

    t1 |> inFixed("misc", 90) |> format(fmtRFC3339) |> expect |> toBe(exp);
  });

  test("fmtRFC3339 make 1", () => {
    let t = make(~y=2020, ~m=January, ~d=20, ~hour=10, Fixed("CET", 60));
    let exp = "2020-01-20T10:00:00+01:00";

    t |> format(fmtRFC3339) |> expect |> toBe(exp);
  });
  test("fmtUnixDate make 1", () => {
    let t = make(~y=2020, ~m=January, ~d=20, ~hour=10, Fixed("CET", 60));
    let exp = "Mon Jan 20 10:00:00 CET 2020";

    t |> format(fmtUnixDate) |> expect |> toBe(exp);
  });
  test("fmtUnixDate make 2", () => {
    let t = make(~y=2020, ~m=January, ~d=20, ~hour=10, Fixed("CET", 60));
    let exp = "Mon Jan 20 9:00:00 UTC 2020";

    t |> inUTC |> format(fmtUnixDate) |> expect |> toBe(exp);
  });
  test("fmtUnixDate set 1", () => {
    let t = make(~y=2020, ~m=January, ~d=20, ~hour=10, Fixed("CET", 60));
    let exp = "Sat Jun 20 10:00:00 CET 2020";

    t |> set(~m=June) |> format(fmtUnixDate) |> expect |> toBe(exp);
  });

  test("fmtUnixDate set 2", () => {
    let t = make(~y=2020, ~m=January, ~d=20, ~hour=10, Local);
    let exp = "Sat Jun 20 10:00:00 Central European Summer Time 2020";

    t |> set(~m=June) |> format(fmtUnixDate) |> expect |> toBe(exp);
  });
  test("fmtUnixDate set 3", () => {
    let t = make(~y=2020, ~m=January, ~d=20, ~hour=10, Local);
    let exp = "Mon Jan 20 10:00:00 Central European Standard Time 2020";

    t |> format(fmtUnixDate) |> expect |> toBe(exp);
  });
  test("fmtUnixDate set 3", () => {
    let t1 =
      make(~y=2020, ~m=January, ~d=20, ~hour=10, Local)
      |> set(~m=June)
      |> unix;
    let t2 =
      make(~y=2020, ~m=January, ~d=20, ~hour=10, Fixed("CET", 60))
      |> set(~m=June)
      |> unix;

    t2 -. Duration.hour |> expect |> toBe(t1);
  });
});

t1
|> format(fmtRFC3339ms)
|> Js.log3("2", (t1 |> string) ++ " -- " ++ fmtRFC3339ms ++ " -- ");
