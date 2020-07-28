module List = {
  include List;

  let head: list('a) => 'a = hd;
  let tail: list('a) => list('a) = tl;

  let find_first: ('a => bool, list('a)) => 'a = find;
  let find_last: ('a => bool, list('a)) => 'a =
    (equal, l) => l |> rev |> find(equal);

  let last: list('a) => 'a = l => nth(l, length(l) - 1);

  let rec drop_left: (int, list('a)) => list('a) =
    (i, l) => {
      if (l |> length < i) {
        raise(Invalid_argument("not_enough_elements"));
      };
      i > 0 ? drop_left(i - 1, l |> tl) : l;
    };

  let drop_right: (int, list('a)) => list('a) =
    (i, l) => l |> rev |> drop_left(i) |> rev;

  let take_left: (int, list('a)) => list('a) =
    (i, l) => {
      if (l |> length < i) {
        raise(Invalid_argument("not_enough_elements"));
      };
      let rec take = (acc, l, i) => {
        i < 1 ? acc : take(acc @ [l |> hd], l |> tl, i - 1);
      };
      take([], l, i);
    };

  let take_right: (int, list('a)) => list('a) =
    (i, l) => {
      l |> rev |> take_left(i) |> rev;
    };

  let take_left_while: ('a => bool, list('a)) => list('a) =
    (fn, l) => {
      let rec take = (acc, l) => {
        let h = l |> hd;
        !fn(h) ? acc : take(acc @ [h], tl(l));
      };
      l |> length == 0 ? l : take([], l);
    };

  let take_right_while: ('a => bool, list('a)) => list('a) =
    (fn, l) => l |> rev |> take_left_while(fn) |> rev;

  let rec drop_left_while: ('a => bool, list('a)) => list('a) =
    (fn, l) => {
      let h = l |> hd;
      !fn(h) ? l : drop_left_while(fn, l |> tl);
    };

  let drop_right_while: ('a => bool, list('a)) => list('a) =
    (fn, l) => l |> rev |> drop_left_while(fn) |> rev;

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
        fold_left(
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
        |> fold_left(
             (acc, e) => {
               let (i, added, remove) = acc;
               remove |> length == 0 || remove |> hd != i
                 ? (i + 1, added @ [e], remove)
                 : (i + 1, added, remove |> tl);
             },
             (0, [], lines |> sort_uniq((a, b) => a - b)),
           );
      ret;
    };

  let slice: (int, int, list('a)) => list('a) =
    (starti, stopi, l) => {
      let len = l |> length;
      let start = starti >= 0 ? starti : 0;
      let stop = stopi >= 0 ? stopi : len;
      l |> drop_left(start) |> drop_right(len - stop);
    };

  let zip2_with: (('a, 'b) => 'c, list('b), list('a)) => list('c) =
    (zipper, l2, l1) => {
      if (length(l1) != length(l2)) {
        raise(Invalid_argument("lists_of_different_sizes"));
      };
      fold_left2((acc, a, b) => acc @ [zipper(a, b)], [], l1, l2);
    };

  //   let zip2: (list('y), list('x)) => list(('x, 'y)) =
  //     zip2_with((x: 'z, y: 'y) => (x, y));
  let zip2: (list('b), list('a)) => list(('a, 'b)) =
    (l2, l1) => l1 |> zip2_with((a, b) => (a, b), l2);

  let zip3_with:
    (('a, 'b, 'c) => 'd, list('b), list('c), list('a)) => list('d) =
    (zipper, l2, l3, l1) => {
      if (length(l1) != length(l2) || length(l2) != length(l3)) {
        raise(Invalid_argument("lists_of_different_sizes"));
      };

      let rec zip = (acc, l1, l2, l3) => {
        l1 |> length == 0
          ? acc
          : {
            let h1 = l1 |> hd;
            let h2 = l2 |> hd;
            let h3 = l3 |> hd;
            let t1 = l1 |> tl;
            let t2 = l2 |> tl;
            let t3 = l3 |> tl;
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
      fold_left(
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
      fold_left(
        (acc, e) => {
          let (a, b, c) = e;
          let (l1, l2, l3) = acc;
          (l1 @ [a], l2 @ [b], l3 @ [c]);
        },
        ([], [], []),
        l,
      );
    };
};
