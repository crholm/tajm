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

// let f = a => {
//   let b =
//     Tajm_Functions_Base64.encode(`std, Tajm_Functions_Array.of_string(a));

//   Js.log2(String.length(a), a);
//   Js.log(b);

//   let c =
//     Tajm_Functions_Base64.decode(`std, b) |> Tajm_Functions_Array.to_string;
//   Js.log2(String.length(c), c);
//   Js.log("----------");
// };

// f("Hello");

// f("HelloW");

// f("HelloWr");

let data = Tajm_Iana_Lib_All.data
Js.log("Marshel full");
let a = Tajm.now();
let tzs =
  Array.map((s: Tajm_Iana_Tz.Lazy.tz) => Tajm_Iana_Encoding.unmarshal(s.raw), data);
let b = Tajm.now();
Js.log3(" -", Array.length(tzs), "items");
Js.log2(" -",  Tajm.sub(b, a) |> Tajm.Duration.string);

Js.log("Lazy load 1");
let a = Tajm.now();
Tajm_Iana.lazyLoadLocations(data);
let loc = Tajm_Iana.getLocation("Europe/Stockholm")
let b = Tajm.now();
Js.log3(" -", loc);
Js.log2(" -",  Tajm.sub(b, a) |> Tajm.Duration.string);

Js.log("Lazy load 2");
let a = Tajm.now();
let loc = Tajm_Iana.getLocation("Europe/Stockholm")
let b = Tajm.now();
Js.log3(" -", loc);
Js.log2(" -",  Tajm.sub(b, a) |> Tajm.Duration.string);