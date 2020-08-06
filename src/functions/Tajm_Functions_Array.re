let to_string = (arr: array(char)): string => {
  String.init(Array.length(arr), i => arr[i]);
};
let of_string = (str: string): array(char) => {
  Array.init(String.length(str), i => str.[i]);
};

let drop: (int, array('a)) => array('a) =
  (i, l) => {
    let len = Array.length(l);
    if (len < i) {
      [||];
    } else {
      Array.sub(l, i, len - i);
    };
  };

let dropRight: (int, array('a)) => array('a) =
  (i, l) => {
    let len = Array.length(l);
    if (len < i) {
      [||];
    } else {
      Array.sub(l, len - i, i);
    };
  };

let rec dropWhile: ('a => bool, array('a)) => array('a) =
  (fn, l) => {
    let len = Array.length(l);
    len == 0
      ? l
      : {
        let h = l[0];
        !fn(h) ? l : dropWhile(fn, Array.sub(l, 1, len - 1));
      };
  };

let rec dropRightWhile: ('a => bool, array('a)) => array('a) =
  (fn, l) => {
    let len = Array.length(l);
    len == 0
      ? l
      : {
        let h = l[len - 1];
        !fn(h) ? l : dropRightWhile(fn, Array.sub(l, 0, len - 2));
      };
  };

let find: ('a => bool, array('a)) => option('a) =
  (fn, arr) => {
    let len = Array.length(arr);
    let rec find = i =>
      if (i == len) {
        None;
      } else if (fn(arr[i])) {
        Some(arr[i]);
      } else {
        find(i + 1);
      };
    find(0);
  };
