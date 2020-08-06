module Arrays = Tajm_Functions_Array;

let split = (c: char, str: string) => {
  let rec split = (acc, cur, l): array(string) =>
    if (l |> List.length == 0) {
      Array.append(acc, [|cur|]);
    } else {
      let h = l |> List.hd;
      let tl = l |> List.tl;
      let (acc, cur) =
        if (h == c) {
          let acc = Array.append(acc, [|cur|]);
          let cur = "";
          (acc, cur);
        } else {
          let cur = cur ++ Char.escaped(h);
          (acc, cur);
        };
      split(acc, cur, tl);
    };
  split([||], "", Arrays.of_string(str) |> Array.to_list);
};

let join = (c: string, strs: array(string)) => {
  let len = Array.length(strs);
  let rec join = (acc, i, strs) =>
    if (i == len) {
      acc;
    } else {
      let acc = acc ++ c ++ List.hd(strs);
      join(acc, i + 1, List.tl(strs));
    };
  join("", 0, strs |> Array.to_list);
};
