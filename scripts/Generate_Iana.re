module Strings = Tajm_Functions_String;
module Arrays = Tajm_Functions_Array;
module Model = Tajm_Iana_Tz;

let path = "./data/iana-latest/";
let pathlen = String.length(path);

let gen = (outname: string, pattern: string, adjust: Model.tz => Model.tz) => {
  Js.log3("Generating", outname, pattern);
  Glob.glob(
    path ++ pattern,
    (_, files) => {
      let res =
        Array.fold_left(
          (acc, file) => {
            let len = String.length(file);
            if (len > pathlen) {
              try({
                let name = String.sub(file, pathlen, len - pathlen);

                let str = Node.Fs.readFileSync(file, `binary);
                let data = Arrays.of_string(str);
                let loc = Tajm_Iana_Encoding.unmarshal_binary(name, data);
                let locStr = loc |> adjust |> Tajm_Iana_Encoding.marshal;
                acc @ [locStr];
              }) {
              | _ =>
                Js.log("|- Failed " ++ file ++ ", probably a dir");
                acc;
              };
            } else {
              acc;
            };
          },
          [],
          files,
        )
        |> Array.of_list;

      Node.Fs.writeFileAsUtf8Sync(
        "./data/iana-latest-" ++ outname ++ ".txt",
        Strings.join("\n", res),
      );
      let str =
        "open Tajm_Iana_Tz.Lazy;\n"
        ++ "let data = [|\n"
        ++ Array.fold_left(
             (acc, a) => {
               acc
               ++ "{name:\""
               ++ String.sub(a, 0, String.index(a, '|'))
               ++ "\", raw:\""
               ++ a
               ++ "\"},\n"
             },
             "",
             res,
           )
        ++ "|];";
      Node.Fs.writeFileAsUtf8Sync(
        "./src/iana/lib/Tajm_Iana_Lib_" ++ outname ++ ".re",
        str,
      );
      ();
    },
  );
};

let gt70 = (tz: Model.tz) => {
  let len = Array.length(tz.transitions);
  let count =
    Array.fold_left((acc, f) => f < 0. ? acc + 1 : acc, 0, tz.transitions);
  {
    ...tz,
    transitions: Array.sub(tz.transitions, count, len - count),
    zone_idxs: Array.sub(tz.zone_idxs, count, len - count),
  };
};

gen("All", "**", a => a);
gen("All70", "**", gt70);

let continents = [
  "Africa",
  "America",
  "Antarctica",
  "Arctic",
  "Asia",
  "Atlantic",
  "Australia",
  "Brazil",
  "Canada",
  "Chile",
  "Etc",
  "Europe",
  "Indian",
  "Mexico",
  "Pacific",
  "US",
];

List.iter(
  c => {
    gen(c, c ++ "/*", a => a);
    gen(c ++ "70", c ++ "/*", gt70);
  },
  continents,
);
