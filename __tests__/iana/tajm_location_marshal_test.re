open Jest;
open Expect;

describe("marshal", () => {
  let str =
    Node.Fs.readFileSync("data/iana-latest/Europe/Stockholm", `binary);
  // Js.log(1);
  let data = Tajm_Functions_Array.of_string(str);
  // Js.log(2);
  let stockholm =
    Tajm_Iana_Encoding.unmarshal_binary("Europe/Stockholm", data);
  // Js.log(3);
  let str = Tajm_Iana_Encoding.marshal(stockholm);
  // Js.log(4);
  let stockholm2 = Tajm_Iana_Encoding.unmarshal(str);
  // Js.log(5);

  test("abbrev", () => {
    stockholm.abbrev |> expect |> toEqual(stockholm2.abbrev)
  });

  test("zones", () => {
    stockholm.zones |> expect |> toEqual(stockholm2.zones)
  });
  test("zone_idxs", () => {
    stockholm.zone_idxs |> expect |> toEqual(stockholm2.zone_idxs)
  });
  test("name", () => {
    stockholm.name |> expect |> toEqual(stockholm2.name)
  });

  test("abbrev", () => {
    stockholm.transitions |> expect |> toEqual(stockholm2.transitions);
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
