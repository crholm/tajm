module Arrays = Tajm_Functions_Array;

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
