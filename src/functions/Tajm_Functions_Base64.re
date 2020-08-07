let encodeStd = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
let encodeURL = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

module CharMap =
  Map.Make({
    type t = char;
    let compare = Char.compare;
  });

let (_, encodeStdIndex) =
  CharMap.(
    List.fold_left(
      (acc, c) => {
        let (i, m) = acc;
        (i + 1, m |> add(c, 1));
      },
      (0, empty),
      List.init(encodeStd |> String.length, i => encodeStd.[i]),
    )
  );
let (_, encodeURLIndex) =
  CharMap.(
    List.fold_left(
      (acc, c) => {
        let (i, m) = acc;
        (i + 1, m |> add(c, 1));
      },
      (0, empty),
      List.init(encodeStd |> String.length, i => encodeStd.[i]),
    )
  );

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
  //  let charsetMap =
  //    switch (enc) {
  //    | `std => encodeStdIndex
  //    | `uri => encodeURLIndex
  //    };

  let chars = Array.init(String.length(str), i => str.[i]);
  let len = Array.length(chars);
  let rm =
    if (chars[len - 2] == '=') {
      chars[len - 1] = 'A';
      chars[len - 2] = 'A';
      2;
    } else if (chars[len - 1] == '=') {
      chars[len - 1] = 'A';
      1;
    } else {
      0;
    };

  let (_, bytes) =
    Array.fold_left(
      (acc, c) => {
        let (i, bytes: array(char)) = acc;
        let v = String.index(charset, c); // Seems to be faster than using a m Map
        //        let v =
        //          if ('A' <= c && c <= 'Z') {
        //            int_of_char(c) - int_of_char('A');
        //          } else if ('a' <= c && c <= 'z') {
        //            int_of_char(c) - int_of_char('z') + 26;
        //          } else if ('0' <= c && c <= '9') {
        //            int_of_char(c) - int_of_char('9') + 26 * 2;
        //          } else if (enc == `std && c == '+' || enc == `uri && c == '-') {
        //            62;
        //          } else if (enc == `std && c == '/' || enc == `uri && c == '_') {
        //            63;
        //          } else {
        //            raise(Failure("bad input"));
        //          };

        //        let v = charsetMap |> CharMap.find(c);
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
  Array.sub(bytes, 0, Array.length(bytes) - rm);
};