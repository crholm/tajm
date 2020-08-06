let encodeStd = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
let encodeURL = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

type encoding = [ | `std | `uri];

let encode = (enc: encoding, bytes: array(char)): string => {
  let charset =
    switch (enc) {
    | `std => encodeStd
    | `uri => encodeURL
    };

  let (rr, i, res) =
    Array.fold_left(
      (acc, b) => {
        let (rr: char, i: int, a: string) = acc;

        let (rr, i, str) =
          switch (i) {
          | 0 =>
            let v = (int_of_char(b) land 0b11111100) lsr 2;
            let r = char_of_int((int_of_char(b) land 0b00000011) lsl 6);
            (r, 1, Char.escaped(charset.[v]));
          | 1 =>
            let v = (int_of_char(rr) lor int_of_char(b) lsr 2) lsr 2;
            let r = char_of_int((int_of_char(b) land 0b00001111) lsl 4);
            (r, 2, Char.escaped(charset.[v]));
          | 2 =>
            let v0 = (int_of_char(rr) lor int_of_char(b) lsr 4) lsr 2;
            let v1 = int_of_char(b) land 0b00111111;
            let str =
              Char.escaped(charset.[v0]) ++ Char.escaped(charset.[v1]);
            ('\000', 0, str);
          | _ => raise(Failure("should not happen"))
          };

        (rr, i, a ++ str);
      },
      ('\000', 0, ""),
      bytes,
    );

  let res =
    switch (i) {
    | 0 => res
    | 1 =>
      let v = int_of_char(rr) lsr 2;
      res ++ Char.escaped(charset.[v]) ++ "==";
    | 2 =>
      let v = int_of_char(rr) lsr 2;
      res ++ Char.escaped(charset.[v]) ++ "=";
    | _ => raise(Failure("should not happen"))
    };

  res;
};

let decode = (enc: encoding, str: string): array(char) => {
  let charset =
    switch (enc) {
    | `std => encodeStd
    | `uri => encodeURL
    };

  let chars = Array.init(String.length(str), i => str.[i]);
  let len = Array.length(chars);
  if (chars[len - 1] == '=') {
    chars[len - 1] = 'A';
  };
  if (chars[len - 2] == '=') {
    chars[len - 2] = 'A';
  };

  let (_, bytes) =
    Array.fold_left(
      (acc, c) => {
        let (i, bytes: array(char)) = acc;
        let v = String.index(charset, c);
        let len = Array.length(bytes);
        switch (i) {
        | 0 => (1, Array.append(bytes, [|char_of_int(v lsl 2)|]))
        | 1 =>
          let cur = int_of_char(bytes[len - 1]);
          let n = (v land 0b00110000) lsr 4;
          let rem = (v land 0b00001111) lsl 4;

          bytes[len - 1] = char_of_int(cur lor n);

          let bytes = Array.append(bytes, [|char_of_int(rem)|]);
          (2, bytes);
        | 2 =>
          let cur = int_of_char(bytes[len - 1]);
          let n = (v land 0b00111100) lsr 2;
          let rem = (v land 0b00000011) lsl 6;
          bytes[len - 1] = char_of_int(cur lor n);
          let bytes = Array.append(bytes, [|char_of_int(rem)|]);
          (3, bytes);
        | 3 =>
          let cur = int_of_char(bytes[len - 1]);
          let rem = v land 0b00111111;
          bytes[len - 1] = char_of_int(cur lor rem);
          (0, bytes);
        | 4 => (4, bytes)
        | _ => raise(Failure("should not happen"))
        };
      },
      (0, [||]),
      chars,
    );
  bytes;
};
