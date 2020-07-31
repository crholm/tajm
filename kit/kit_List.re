let init = List.init;
let length: list('a) => int = List.length;
let head: list('a) => 'a = List.hd;
let tail: list('a) => list('a) = List.tl;

let rev: list('a) => list('a) = List.rev;
let nth: (int, list('a)) => 'a = (i, l) => List.nth(l, i);

let find: ('a => bool, list('a)) => 'a = List.find;
let find_all: ('a => bool, list('a)) => list('a) = List.find_all;
let find_last: ('a => bool, list('a)) => 'a =
  (equal, l) => l |> rev |> find(equal);
let filter: ('a => bool, list('a)) => list('a) = List.filter;

let last: list('a) => 'a = l => l |> nth(length(l) - 1);

let exists: ('a => bool, list('a)) => bool = List.exists;

let sort_uniq: (('a, 'a) => int, list('a)) => list('a) = List.sort_uniq;

let rec drop: (int, list('a)) => list('a) =
  (i, l) =>
    if (l |> length < i) {
      [];
    } else {
      i > 0 ? drop(i - 1, l |> tail) : l;
    };

let drop_right: (int, list('a)) => list('a) =
  (i, l) => l |> rev |> drop(i) |> rev;

let slice: (int, int, list('a)) => list('a) =
  (starti, stopi, l) => {
    let len = l |> length;
    let start = starti >= 0 ? starti : 0;
    let stop = stopi >= 0 ? stopi : len;
    l |> drop(start) |> drop_right(len - stop);
  };

let truncate: (int, list('a)) => list('b) =
  (i, l) => {
    let len = l |> length;
    drop_right(len - i, l);
  };
let truncate2: (list('b), list('a)) => (list('a), list('b)) =
  (b, a) => {
    let len1 = length(a);
    let len2 = length(b);
    if (len1 == len2) {
      (a, b);
    } else {
      let len = len1 < len2 ? len1 : len2;
      (slice(0, len, a), slice(0, len, b));
    };
  };

let truncate3:
  (list('b), list('c), list('a)) => (list('a), list('b), list('c)) =
  (b, c, a) => {
    let len1 = length(a);
    let len2 = length(b);
    let len3 = length(c);
    if (len1 == len2 && len2 == len3) {
      (a, b, c);
    } else {
      let len_ = len1 < len2 ? len1 : len2;
      let len = len3 < len_ ? len3 : len_;
      (slice(0, len, a), slice(0, len, b), slice(0, len, c));
    };
  };

let reduce: (('a, 'b) => 'a, 'a, list('b)) => 'a = List.fold_left;
let reduce2: (('a, 'e1, 'e2) => 'a, 'a, list('e1), list('e2)) => 'a =
  (fn, def, l2, l1) => {
    let (l11, l22) = l1 |> truncate2(l2);
    List.fold_left2(fn, def, l22, l11);
  };

let take: (int, list('a)) => list('a) =
  (i, l) => {
    let rec take = (acc, l, i) => {
      i < 1 ? acc : take(acc @ [l |> head], l |> tail, i - 1);
    };

    if (l |> length < i) {
      [];
    } else {
      take([], l, i);
    };
  };

let take_right: (int, list('a)) => list('a) =
  (i, l) => {
    l |> rev |> take(i) |> rev;
  };

let take_while: ('a => bool, list('a)) => list('a) =
  (fn, l) => {
    let rec take = (acc, l) => {
      let h = l |> head;
      !fn(h) ? acc : take(acc @ [h], tail(l));
    };
    l |> length == 0 ? l : take([], l);
  };

let take_right_while: ('a => bool, list('a)) => list('a) =
  (fn, l) => l |> rev |> take_while(fn) |> rev;

let rec drop_while: ('a => bool, list('a)) => list('a) =
  (fn, l) => {
    let h = l |> head;
    !fn(h) ? l : drop_while(fn, l |> tail);
  };

let drop_right_while: ('a => bool, list('a)) => list('a) =
  (fn, l) => l |> rev |> drop_while(fn) |> rev;

let intersection: (('a, 'b) => bool, list('b), list('a)) => list('a) =
  (equal, l2, l1) => {
    l1 |> filter(a => {l2 |> exists(b => equal(a, b))});
  };

let complement: (('a, 'b) => bool, list('b), list('a)) => list('a) =
  (equal, l2, l1) => {
    l1 |> filter(a => {l2 |> exists(b => equal(a, b)) |> (b => !b)});
  };

let union: (('a, 'a) => bool, list('a), list('a)) => list('a) =
  (equal, l2, l1) => {
    l1 @ (l2 |> complement(equal, l1));
  };

let pull: ('a => bool, list('a)) => list('a) =
  (equal, l) => filter(a => {!equal(a)}, l);

let pulli: ((int, 'a) => bool, list('a)) => list('a) =
  (equal, l) => {
    let (__, ret) =
      reduce(
        (acc, e) => {
          let (i, added) = acc;
          equal(i, e) ? (i + 1, added) : (i + 1, added @ [e]);
        },
        (0, []),
        l,
      );
    ret;
  };

let pull_at: (list(int), list('a)) => list('a) =
  (lines, l) => {
    let (__, ret, _) =
      l
      |> reduce(
           (acc, e) => {
             let (i, added, remove) = acc;
             remove |> length == 0 || remove |> head != i
               ? (i + 1, added @ [e], remove)
               : (i + 1, added, remove |> tail);
           },
           (0, [], lines |> sort_uniq((a, b) => a - b)),
         );
    ret;
  };

let zip2_with: (('a, 'b) => 'c, list('b), list('a)) => list('c) =
  (zipper, l2, l1) => {
    reduce2((acc, a, b) => acc @ [zipper(a, b)], [], l1, l2);
  };

let zip2: (list('b), list('a)) => list(('a, 'b)) =
  (l2, l1) => zip2_with((a, b) => (a, b), l2, l1);

let zip3_with:
  (('a, 'b, 'c) => 'd, list('b), list('c), list('a)) => list('d) =
  (zipper, l2, l3, l1) => {
    let rec zip = (acc, l1, l2, l3) => {
      length(l1) == 0 || length(l2) == 0 || length(l3) == 0
        ? acc
        : {
          let h1 = l1 |> head;
          let h2 = l2 |> head;
          let h3 = l3 |> head;
          let t1 = l1 |> tail;
          let t2 = l2 |> tail;
          let t3 = l3 |> tail;
          zip(acc @ [zipper(h1, h2, h3)], t1, t2, t3);
        };
    };
    zip([], l1, l2, l3);
  };

let zip3: (list('b), list('c), list('a)) => list(('a, 'b, 'c)) =
  (l2, l3, l1) => {
    zip3_with((a, b, c) => (a, b, c), l2, l3, l1);
  };

let unzip2: list(('a, 'b)) => (list('a), list('b)) =
  l => {
    reduce(
      (acc, e) => {
        let (a, b) = e;
        let (l1, l2) = acc;
        (l1 @ [a], l2 @ [b]);
      },
      ([], []),
      l,
    );
  };
let unzip3: list(('a, 'b, 'c)) => (list('a), list('b), list('c)) =
  l => {
    reduce(
      (acc, e) => {
        let (a, b, c) = e;
        let (l1, l2, l3) = acc;
        (l1 @ [a], l2 @ [b], l3 @ [c]);
      },
      ([], [], []),
      l,
    );
  };
