open Jest;
open Expect;

type test = {
  t: float,
  expAt: string,
  expPrior: string,
};

Tajm_Iana.lazyLoadLocations(Tajm_Iana_Lib_Europe.data);

describe("IANA", () => {
  let fmt = "2006-01-02T15:04:05 -07:00 MST";
  let table = [
    {
      t: (-2208992414000.0),
      expAt: "1899-12-31T23:59:46 +01:00 CET",
      expPrior: "1899-12-31T23:59:59 +01:00 SET",
    },
    {
      t: (-1692496800000.0),
      expAt: "1916-05-15T00:00:00 +02:00 CEST",
      expPrior: "1916-05-14T22:59:59 +01:00 CET",
    },
    {
      t: (-1680483600000.0),
      expAt: "1916-10-01T00:00:00 +01:00 CET",
      expPrior: "1916-10-01T00:59:59 +02:00 CEST",
    },
    {
      t: 1585443600000.0,
      expAt: "2020-03-29T03:00:00 +02:00 CEST",
      expPrior: "2020-03-29T01:59:59 +01:00 CET",
    },
    {
      t: 2140045200000.0,
      expAt: "2037-10-25T02:00:00 +01:00 CET",
      expPrior: "2037-10-25T02:59:59 +02:00 CEST",
    },
  ];

  table
  |> List.iter(entry => {
       test("zero", () => {
         let loc =
           Tajm_Iana.getLocation("Europe/Stockholm")
           |> (
             fun
             | Some(i) => i
             | None => raise(Failure("Could not find timezone"))
           );

         let t1 = entry.t |> Tajm.ofUnix |> Tajm.at(IANA(loc));
         let s1 = t1 |> Tajm.format(fmt);

         let s2 =
           t1 |> Tajm.add(-. Tajm.Duration.millisecond) |> Tajm.format(fmt);
         [s1, s2] |> expect |> toEqual([entry.expAt, entry.expPrior]);
       })
     });
});
