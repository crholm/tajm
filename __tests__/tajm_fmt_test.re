open Jest;
open Expect;
open Tajm;

describe("Tajm fmt", () => {
  test("fmt", () => {
    let t = now();
    t
    |> Fmt.string(Fmt.Common.ansic)
    |> Js.log3("1", (t |> string) ++ " -- " ++ Fmt.Common.ansic ++ " -- ");
    t
    |> Fmt.string(Fmt.Common.rfc3339)
    |> Js.log3("2", (t |> string) ++ " -- " ++ Fmt.Common.rfc3339 ++ " -- ");
    t
    |> Fmt.string(Fmt.Common.rfc3339ms)
    |> Js.log3("2", (t |> string) ++ " -- " ++ Fmt.Common.rfc3339ms ++ " -- ");

    true |> expect |> toBe(true);
  })
});
