let tee: ('a => unit, 'a) => 'a =
  (fn, a) => {
    fn(a);
    a;
  };
let tee2: ('a => 'b, 'b => unit, 'a) => 'a =
  (fn, fn2, a) => {
    fn(a) |> fn2;
    a;
  };
let ln = () => Js.log("");

Js.log("Tajm.zero() |> Tajm.string |> Js.log");
Tajm.zero |> Tajm.string |> Js.log;
ln();

Js.log("Tajm.now() |> Tajm.string |> Js.log");
Tajm.now() |> Tajm.string |> Js.log;
ln();

Js.log("Tajm.ofUnix(1585443600000.)  |> Tajm.string |> Js.log");
Tajm.ofUnix(1585443600000.) |> Tajm.string |> Js.log;
ln();

Js.log(
  "Tajm.make(
  ~y=2020,
  ~m=February,
  ~d=29,
  ~hour=15,
  ~min=32,
  ~sec=42,
  ~ms=123,
  Tajm.z,
)
|> Tajm.string
|> Js.log;",
);
Tajm.make(
  ~y=2020,
  ~m=February,
  ~d=29,
  ~hour=15,
  ~min=32,
  ~sec=42,
  ~ms=123,
  Tajm.z,
)
|> Tajm.string
|> Js.log;
ln();

Js.log("Tajm.zero |> Tajm.compare(Tajm.now()) |> Js.log");
Tajm.zero |> Tajm.compare(Tajm.now()) |> Js.log;
ln();

Js.log("Tajm.now() |> Tajm.unix |> Js.log");
Tajm.now() |> Tajm.unix |> Js.log;
ln();

Js.log("Tajm.now() |> Tajm.toJs |> Js.log");
Tajm.now() |> Tajm.toJs |> Js.log;
ln();

Js.log(
  "Tajm.now()
 |> tee2(Tajm.string, Js.log)
 |> Tajm.add(Tajm.Duration.hour)
 |> Tajm.string
 |> Js.log",
);
Tajm.now()
|> tee2(Tajm.string, Js.log)
|> Tajm.add(Tajm.Duration.hour)
|> Tajm.string
|> Js.log;
ln();

Js.log(
  "let t1 = Tajm.now();
 let t2 =
   t1 |> Tajm.add(Tajm.Duration.hour) |> Tajm.add(15. *. Tajm.Duration.minute);
 Tajm.sub(t1, t2) |> Tajm.Duration.string |> Js.log",
);
let t1 = Tajm.now();
let t2 =
  t1 |> Tajm.add(Tajm.Duration.hour) |> Tajm.add(15. *. Tajm.Duration.minute);
Tajm.sub(t1, t2) |> Tajm.Duration.string |> Js.log;
ln();

Js.log(
  "Tajm.now()
 |> Tajm.add((-35.) *. Tajm.Duration.secound)
 |> Tajm.since
 |> Tajm.Duration.string
 |> Js.log;",
);
Tajm.now()
|> Tajm.add((-35.) *. Tajm.Duration.secound)
|> Tajm.since
|> Tajm.Duration.string
|> Js.log;
ln();

Js.log(
  "Tajm.now()
 |> Tajm.add(2. *. Tajm.Duration.minute)
 |> Tajm.until
 |> Tajm.Duration.string
 |> Js.log;",
);
Tajm.now()
|> Tajm.add(2. *. Tajm.Duration.minute)
|> Tajm.until
|> Tajm.Duration.string
|> Js.log;
ln();

Js.log(
  "Tajm.now() |> Tajm.truncate(Tajm.Duration.minute) |> Tajm.string |> Js.log",
);
Tajm.now() |> Tajm.truncate(Tajm.Duration.minute) |> Tajm.string |> Js.log;
ln();

Js.log("Tajm.now()->Tajm.after(Tajm.zero) |> Js.log;");
Tajm.now()->Tajm.after(Tajm.zero) |> Js.log;
ln();

Js.log("Tajm.now()->Tajm.before(Tajm.zero) |> Js.log;");
Tajm.now()->Tajm.before(Tajm.zero) |> Js.log;
ln();

Js.log("Tajm.zero |> Tajm.future |> Js.log;");
Tajm.zero |> Tajm.future |> Js.log;
ln();

Js.log("Tajm.zero |> Tajm.past |> Js.log;");
Tajm.zero |> Tajm.past |> Js.log;
ln();

Js.log(
  "Tajm.now()
 |> Tajm.atUTC
 |> Tajm.format(\"2006-01-02 15:04:05 Z07:00\")
 |> Js.log",
);
Tajm.now()
|> Tajm.atUTC
|> Tajm.format("2006-01-02 15:04:05 Z07:00")
|> Js.log;
ln();

Js.log(
  "Tajm.now()
  |> Tajm.atLocal
  |> Tajm.format(\"2006-01-02 15:04:05 Z07:00\")
  |> Js.log",
);
Tajm.now()
|> Tajm.atLocal
|> Tajm.format("2006-01-02 15:04:05 Z07:00")
|> Js.log;
ln();

Js.log(
  "Tajm.now()
|> Tajm.atFixed(\"CEST\", 2 * 60 * 60)
|> Tajm.format(\"2006-01-02 15:04:05 MST\")
|> Js.log",
);
Tajm.now()
|> Tajm.atFixed("CEST", 2 * 60 * 60)
|> Tajm.format("2006-01-02 15:04:05 MST")
|> Js.log;
ln();

Js.log(
  "Tajm.now()
|> Tajm.at(Tajm.local)
|> Tajm.format(\"2006-01-02 15:04:05 MST\")
|> Js.log;",
);
Tajm.now()
|> Tajm.at(Tajm.local)
|> Tajm.format("2006-01-02 15:04:05 MST")
|> Js.log;
ln();

Js.log(
  "let zone = Tajm.now() |> Tajm.at(Tajm.local) |> Tajm.zone;
Tajm.zero
|> Tajm.at(zone)
|> Tajm.format(\"2006-01-02 15:04:05 Z07:00\")
|> Js.log;",
);
let zone = Tajm.now() |> Tajm.at(Tajm.local) |> Tajm.zone;
Tajm.zero
|> Tajm.at(zone)
|> Tajm.format("2006-01-02 15:04:05 Z07:00")
|> Js.log;
ln();