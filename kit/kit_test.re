open Jest;
open Expect;
describe("Kit", () => {
  test("truncate", () => {
    let l1 = ["1", "2"];
    l1 |> Kit.List.truncate(1) |> expect |> toEqual(["1"]);
  });
  test("truncate", () => {
    let l1 = ["1", "2"];
    l1 |> Kit.List.truncate(10) |> expect |> toEqual(["1", "2"]);
  });
  test("truncate", () => {
    let l1 = ["1", "2"];
    l1 |> Kit.List.truncate(-1) |> expect |> toEqual([]);
  });

  test("truncate2", () => {
    let l1 = ["1", "2"];
    let l2 = ["1"];
    l1 |> Kit.List.truncate2(l2) |> expect |> toEqual((["1"], ["1"]));
  });

  test("truncate3", () => {
    let l1 = ["1", "2"];
    let l2 = ["1"];
    let l3 = ["1"];
    l1
    |> Kit.List.truncate3(l2, l3)
    |> expect
    |> toEqual((["1"], ["1"], ["1"]));
  });
  test("truncate33", () => {
    let l1 = ["1", "2"];
    let l2 = ["1"];
    let l3 = [];
    l1 |> Kit.List.truncate3(l2, l3) |> expect |> toEqual(([], [], []));
  });

  test("dropLeft", () => {
    let l1 = ["1", "2", "3", "4", "5"];
    l1 |> Kit.List.drop(3) |> expect |> toEqual(["4", "5"]);
  });
  test("dropLeft2", () => {
    let l1 = [];

    l1 |> Kit.List.drop(3) |> expect |> toEqual([]);
  });

  test("dropRight", () => {
    let l1 = ["1", "2", "3", "4", "5"];

    l1 |> Kit.List.drop_right(3) |> expect |> toEqual(["1", "2"]);
  });

  test("takeLeft", () => {
    let l1 = ["1", "2", "3", "4", "5"];

    l1 |> Kit.List.take(2) |> expect |> toEqual(["1", "2"]);
  });
  test("takeLeft2", () => {
    let l1 = [];
    l1 |> Kit.List.take(2) |> expect |> toEqual([]);
  });

  test("takeRight", () => {
    let l1 = ["1", "2", "3", "4", "5"];

    l1 |> Kit.List.take_right(2) |> expect |> toEqual(["4", "5"]);
  });

  test("takeLeftWhile", () => {
    let l1 = [0, 0, 0, 1, 1];

    l1 |> Kit.List.take_while(a => a == 0) |> expect |> toEqual([0, 0, 0]);
  });
  test("takeLeftWhile2", () => {
    let l1 = [];

    l1 |> Kit.List.take_while(a => a == 0) |> expect |> toEqual([]);
  });

  test("intersect", () => {
    let l1 = ["1", "2", "3", "4", "5"];
    let l2 = [4, 5, 6, 7, 8];

    l1
    |> Kit.List.intersection((a, b) => a == string_of_int(b), l2)
    |> expect
    |> toEqual(["4", "5"]);
  });
  test("intersect2", () => {
    let l1 = ["1", "2", "3", "4", "5"];
    let l2 = [];

    l1
    |> Kit.List.intersection((a, b) => a == string_of_int(b), l2)
    |> expect
    |> toEqual([]);
  });

  test("complement1", () => {
    let l1 = ["1", "2", "3", "4", "5"];
    let l2 = [4, 5, 6, 7, 8];

    l1
    |> Kit.List.complement((a, b) => a == string_of_int(b), l2)
    |> expect
    |> toEqual(["1", "2", "3"]);
  });

  test("complement2", () => {
    let l1 = ["1", "2", "3", "4", "5"];
    let l2 = [];

    l1
    |> Kit.List.complement((a, b) => a == string_of_int(b), l2)
    |> expect
    |> toEqual(l1);
  });
  test("complement3", () => {
    let l1 = [];
    let l2 = [4, 5, 6, 7, 8];

    l1
    |> Kit.List.complement((a, b) => a == string_of_int(b), l2)
    |> expect
    |> toEqual([]);
  });

  test("union", () => {
    let l1 = [1, 3, 2];
    let l2 = [3, 5, 4, 2];

    l1
    |> Kit.List.union((a, b) => a == b, l2)
    |> Kit.List.reduce((acc, a) => acc ++ string_of_int(a) ++ ", ", "")
    |> expect
    |> toEqual("1, 3, 2, 5, 4, ");
  });
  test("union1", () => {
    let l1 = [];
    let l2 = [3, 5, 4, 2];

    l1
    |> Kit.List.union((a, b) => a == b, l2)
    |> Kit.List.reduce((acc, a) => acc ++ string_of_int(a) ++ ", ", "")
    |> expect
    |> toEqual("3, 5, 4, 2, ");
  });
  test("union2", () => {
    let l1 = [3, 5, 4, 2];
    let l2 = [];

    l1
    |> Kit.List.union((a, b) => a == b, l2)
    |> Kit.List.reduce((acc, a) => acc ++ string_of_int(a) ++ ", ", "")
    |> expect
    |> toEqual("3, 5, 4, 2, ");
  });

  test("pull", () => {
    let l1 = [1, 2, 3, 4];
    l1 |> Kit.List.pull(a => a mod 2 == 0) |> expect |> toEqual([1, 3]);
  });
  test("pull2", () => {
    let l1 = [];
    l1 |> Kit.List.pull(a => a mod 2 == 0) |> expect |> toEqual([]);
  });

  test("pulli", () => {
    let l1 = [1, 2, 3, 4];
    l1 |> Kit.List.pulli((i, _) => i mod 2 == 0) |> expect |> toEqual([2, 4]);
  });
  test("pulli2", () => {
    let l1 = [];
    l1 |> Kit.List.pulli((i, _) => i mod 2 == 0) |> expect |> toEqual([]);
  });
  test("pull_at", () => {
    let l1 = [1, 2, 3, 4];
    l1 |> Kit.List.pull_at([1, 2]) |> expect |> toEqual([1, 4]);
  });
  test("pull_at2", () => {
    let l1 = [];
    l1 |> Kit.List.pull_at([1, 2]) |> expect |> toEqual([]);
  });
  test("pull_at2", () => {
    let l1 = [1, 2, 3, 4];
    l1 |> Kit.List.pull_at([1, 2, 9]) |> expect |> toEqual([1, 4]);
  });

  test("slice", () => {
    let l1 = [0, 1, 2, 3, 4];
    l1 |> Kit.List.slice(1, 3) |> expect |> toEqual([1, 2]);
  });
  test("slice1", () => {
    let l1 = [0, 1, 2, 3, 4, 5, 6, 7];
    l1 |> Kit.List.slice(4, 8) |> expect |> toEqual([4, 5, 6, 7]);
  });
  test("slice2", () => {
    let l1 = [0, 1, 2, 3, 4, 5, 6, 7];
    l1 |> Kit.List.slice(4, -1) |> expect |> toEqual([4, 5, 6, 7]);
  });
  test("slice3", () => {
    let l1 = [0, 1, 2, 3, 4, 5, 6, 7];
    l1 |> Kit.List.slice(0, 2) |> expect |> toEqual([0, 1]);
  });
  test("slice4", () => {
    let l1 = [0, 1, 2, 3, 4, 5, 6, 7];
    l1 |> Kit.List.slice(-1, 2) |> expect |> toEqual([0, 1]);
  });
  test("slice5", () => {
    let l1 = [0, 1, 2, 3, 4, 5, 6, 7];
    l1 |> Kit.List.slice(0, l1 |> Kit.List.length) |> expect |> toEqual(l1);
  });

  test("zip2", () => {
    let l1 = [0, 1];
    let l2 = ["a", "b"];
    l1 |> Kit.List.zip2(l2) |> expect |> toEqual([(0, "a"), (1, "b")]);
  });
  test("zip22", () => {
    let l1 = [0, 1];
    let l2 = ["a", "b", "c"];
    l1 |> Kit.List.zip2(l2) |> expect |> toEqual([(0, "a"), (1, "b")]);
  });
  test("zip3", () => {
    let l1 = [0, 1];
    let l2 = ["a", "b"];
    let l3 = [0., 1.];
    l1
    |> Kit.List.zip3(l2, l3)
    |> expect
    |> toEqual([(0, "a", 0.), (1, "b", 1.)]);
  });
  test("zip33", () => {
    let l1 = [0, 1, 2];
    let l2 = ["a", "b"];
    let l3 = [0., 1.];

    l1
    |> Kit.List.zip3(l2, l3)
    |> expect
    |> toEqual([(0, "a", 0.), (1, "b", 1.)]);
  });

  test("unzip2", () => {
    let l1 = [(1, "a"), (2, "b")];
    l1 |> Kit.List.unzip2 |> expect |> toEqual(([1, 2], ["a", "b"]));
  });
  test("unzip22", () => {
    let l1 = [];
    l1 |> Kit.List.unzip2 |> expect |> toEqual(([], []));
  });
  test("unzip3", () => {
    let l1 = [(1, "a", 1.), (2, "b", 2.)];
    l1
    |> Kit.List.unzip3
    |> expect
    |> toEqual(([1, 2], ["a", "b"], [1., 2.]));
  });
  test("unzip32", () => {
    let l1 = [];
    l1 |> Kit.List.unzip3 |> expect |> toEqual(([], [], []));
  });
});
