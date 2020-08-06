type endian = [ | `big];

let int_of_bytes = (_: endian, arr: array(char)) => {
  Array.fold_left((acc, b) => {acc lsl 8 lor int_of_char(b)}, 0, arr);
};

let float_of_bytes = (endian: endian, arr: array(char)) => {
  let len = Array.length(arr);

  if (len == 8) {
    Array.fold_left(
      (acc, b) => {
        let a = Int64.shift_left(acc, 8);
        let bb = Int64.of_int(int_of_char(b));
        Int64.logor(a, bb);
      },
      0L,
      arr,
    )
    |> Int64.to_float;
  } else {
    int_of_bytes(endian, arr) |> float_of_int;
  };
};

let bytes_of_int = (_: endian, i: int): array(char) => {
  let arr: array(char) = Array.init(4, _ => '\000');

  arr[3] = char_of_int(i land 0b11111111);
  let i = i lsr 8;
  arr[2] = char_of_int(i land 0b11111111);
  let i = i lsr 8;
  arr[1] = char_of_int(i land 0b11111111);
  let i = i lsr 8;
  arr[0] = char_of_int(i land 0b11111111);
  arr;
};

let bytes_of_float = (endian: endian, f: float): array(char) => {
  let i = Int64.of_float(f);
  let i1 = Int64.to_int(i);
  let i2 = Int64.to_int(Int64.shift_right(i, 32));
  Array.append(bytes_of_int(endian, i2), bytes_of_int(endian, i1));
};
