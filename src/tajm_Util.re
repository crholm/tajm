let _len = String.length;
let _min = (i: int, j: int) => i < j ? i : j;
let _leftPad = (l: int, padding: char, str: string) => {
  String.init(l - (str |> _len), _ => padding) ++ str;
};
let _rightPad = (l: int, padding: char, str: string) => {
  str ++ String.init(l - (str |> _len), _ => padding);
};
let _listOfString = (str: string) => List.init(str |> _len, i => str.[i]);
let _stringOfList = (l: list(char)) =>
  List.fold_left((acc, c) => acc ++ String.make(1, c), "", l);

let rec _drop: (int, list('a)) => list('a) =
  (i, l) =>
    if (l |> List.length < i) {
      [];
    } else {
      i > 0 ? _drop(i - 1, l |> List.tl) : l;
    };

let _dropRight: (int, list('a)) => list('a) =
  (i, l) => l |> List.rev |> _drop(i) |> List.rev;

let _slice: (int, int, list('a)) => list('a) =
  (starti, stopi, l) => {
    let len = l |> List.length;
    let start = starti >= 0 ? starti : 0;
    let stop = stopi >= 0 ? stopi : len;
    l |> _drop(start) |> _dropRight(len - stop);
  };

let _truncate: (int, list('a)) => list('b) =
  (i, l) => {
    let len = l |> List.length;
    _dropRight(len - i, l);
  };

let _valueOr: ('a, option('a)) => 'a =
  (default, op) => {
    switch (op) {
    | None => default
    | Some(a) => a
    };
  };

let _takeWhile: ('a => bool, list('a)) => list('a) =
  (fn, l) => {
    let rec take = (acc, l) => {
      let h = l |> List.hd;
      !fn(h) ? acc : take(acc @ [h], List.tl(l));
    };
    l |> List.length == 0 ? l : take([], l);
  };

let _takeRightWhile: ('a => bool, list('a)) => list('a) =
  (fn, l) => l |> List.rev |> _takeWhile(fn) |> List.rev;

let rec _dropWhile: ('a => bool, list('a)) => list('a) =
  (fn, l) => {
    List.length(l) == 0
      ? l
      : {
        let h = l |> List.hd;
        !fn(h) ? l : _dropWhile(fn, l |> List.tl);
      };
  };

let _dropRightWhile: ('a => bool, list('a)) => list('a) =
  (fn, l) => l |> List.rev |> _dropWhile(fn) |> List.rev;
