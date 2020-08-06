module Arrays = Tajm_Functions_Array;


let leftPad = (l: int, padding: char, str: string) => {
  String.init(l - (str |> String.length), _ => padding) ++ str;
};
let rightPad = (l: int, padding: char, str: string) => {
  str ++ String.init(l - (str |> String.length), _ => padding);
};

let split = (c: char, str: string) => {
  String.split_on_char(c, str) |> Array.of_list
};

let join = (c: string, strs: array(string)) => {
  let len = Array.length(strs);
  let rec join = (acc, i, strs) =>
    if (i == len) {
      acc;
    } else {
      let acc = acc ++ (i == 0 ? "" : c) ++ List.hd(strs);
      join(acc, i + 1, List.tl(strs));
    };
  join("", 0, strs |> Array.to_list);
};
