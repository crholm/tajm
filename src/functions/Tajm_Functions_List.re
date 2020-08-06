


let of_string = (str: string) => List.init(str |> String.length, i => str.[i]);
let to_string = (l: list(char)) =>
  List.fold_left((acc, c) => acc ++ String.make(1, c), "", l);






let rec drop: (int, list('a)) => list('a) =
  (i, l) =>
    if (l |> List.length < i) {
      [];
    } else {
      i > 0 ? drop(i - 1, l |> List.tl) : l;
    };

let dropRight: (int, list('a)) => list('a) =
  (i, l) => l |> List.rev |> drop(i) |> List.rev;

let slice: (int, int, list('a)) => list('a) =
  (starti, stopi, l) => {
    let len = l |> List.length;
    let start = starti >= 0 ? starti : 0;
    let stop = stopi >= 0 ? stopi : len;
    l |> drop(start) |> dropRight(len - stop);
  };

let truncate: (int, list('a)) => list('b) =
  (i, l) => {
    let len = l |> List.length;
    dropRight(len - i, l);
  };


let takeWhile: ('a => bool, list('a)) => list('a) =
  (fn, l) => {
    let rec take = (acc, l) => {
      let h = l |> List.hd;
      !fn(h) ? acc : take(acc @ [h], List.tl(l));
    };
    l |> List.length == 0 ? l : take([], l);
  };

let takeRightWhile: ('a => bool, list('a)) => list('a) =
  (fn, l) => l |> List.rev |> takeWhile(fn) |> List.rev;

let rec dropWhile: ('a => bool, list('a)) => list('a) =
  (fn, l) => {
    List.length(l) == 0
      ? l
      : {
        let h = l |> List.hd;
        !fn(h) ? l : dropWhile(fn, l |> List.tl);
      };
  };

let dropRightWhile: ('a => bool, list('a)) => list('a) =
  (fn, l) => l |> List.rev |> dropWhile(fn) |> List.rev;
