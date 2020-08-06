open Jest;
open Expect;

describe("marshal", () => {
  let str =
    Node.Fs.readFileSync("data/iana-latest/Europe/Stockholm", `binary);
  // Js.log(1);
  let data = Tajm_Functions_Array.of_string(str);
  // Js.log(2);
  let tz = Tajm_Iana_Encoding.unmarshal_binary("Europe/Stockholm", data);

  let len = Array.length(tz.transitions);
  let count =
    Array.fold_left((acc, f) => f < 0. ? acc + 1 : acc, 0, tz.transitions);
  let tz = {
    ...tz,
    transitions: Array.sub(tz.transitions, count, len - count),
    zone_idxs: Array.sub(tz.zone_idxs, count, len - count),
  };

  // Js.log(3);
  let str = Tajm_Iana_Encoding.marshal(tz);
  // Js.log(4);
  let tz2 = Tajm_Iana_Encoding.unmarshal(str);
  // Js.log(5);

  test("abbrev", () => {
    tz.abbrev |> expect |> toEqual(tz2.abbrev)
  });

  test("zones", () => {
    tz.zones |> expect |> toEqual(tz2.zones)
  });
  test("zone_idxs", () => {
    tz.zone_idxs |> expect |> toEqual(tz2.zone_idxs)
  });
  test("name", () => {
    tz.name |> expect |> toEqual(tz2.name)
  });

  test("transitions", () => {
    tz.transitions |> expect |> toEqual(tz2.transitions)
  });
  test("elements 1", () => {
    tz.transitions
    |> Array.length
    |> expect
    |> toEqual(tz2.zone_idxs |> Array.length)
  });
  test("elements 2", () => {
    tz.zone_idxs
    |> Array.length
    |> expect
    |> toEqual(tz2.transitions |> Array.length)
  });
});

// let str = Node.Fs.readFileSync("data/iana-latest/Europe/Stockholm", `binary);
// Js.log(1);
// let data = Tajm_Functions_Array.of_string(str);
// Js.log(2);
// let stockholm = Tajm_Iana_Encoding.unmarshal_binary("Europe/Stockholm", data);
// Js.log(3);
// let str = Tajm_Iana_Encoding.marshal(stockholm);
// Js.log(4);
// let stockholm2 = Tajm_Iana_Encoding.unmarshal(str);
// Js.log(5);
// Js.log(stockholm);
// Js.log(stockholm2);
