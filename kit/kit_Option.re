let default: ('a, option('a)) => 'a =
  (default, op) => {
    switch (op) {
    | None => default
    | Some(a) => a
    };
  };
