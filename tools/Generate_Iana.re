let path = "./data/iana-latest/";
let pathlen = String.length(path);
Glob.glob(
  path ++ "**",
  (_, files) => {
    let res =
      Array.fold_left(
        (acc, file) => {
          let len = String.length(file);
          if (len > pathlen) {
            try({
              let name = String.sub(file, pathlen, len - pathlen);
              Js.log2("+ Processing", name);

              let str = Node.Fs.readFileSync(file, `binary);
              let data = Tajm_Functions_Array.of_string(str);
              let loc = Tajm_Iana_Encoding.unmarshal_binary(name, data);
              let locStr = Tajm_Iana_Encoding.marshal(loc);
              acc ++ locStr ++ "\n";
            }) {
            | _ =>
              Js.log("|- Failed " ++ file ++ ", probably a dir");
              acc;
            };
          } else {
            acc;
          };
        },
        "",
        files,
      );
    Node.Fs.writeFileAsUtf8Sync("./data/iana-latest.txt", res);
  },
);
