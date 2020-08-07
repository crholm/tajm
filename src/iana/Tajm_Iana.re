open Tajm_Iana_Tz;

let period = (i: tz, f: float): int => {
  let len = i.transitions |> Array.length;
  let rec find = (n: int) =>
    if (n == len) {
      max(n - 1, 0);
    } else if (f < i.transitions[n]) {
      max(n - 1, 0);
    } else {
      find(n + 1);
    };
  find(0);
};

let offset = (i: tz, period: int): int => {
  let zidx = i.zone_idxs[period];
  - i.zones[zidx].offset;
};
let abbr = (i: tz, period: int): string => {
  let zidx = i.zone_idxs[period];
  let abbrev_idx = i.zones[zidx].abbrev_idx;
  i.abbrev[abbrev_idx];
};

module DbMap =
  Map.Make({
    type t = string;
    let compare = compare;
  });

let db = ref(DbMap.empty);
let dbz = ref(DbMap.empty);

let loadLocation = (tz: tz) => {
  db := db^ |> DbMap.add(tz.name, tz);
};
let loadLocations = (tzs: array(tz)) => {
  Array.iter(loadLocation, tzs);
};

let lazyLoadLocation = (tz: Lazy.tz) => {
  dbz := dbz^ |> DbMap.add(tz.name, tz);
};
let lazyLoadLocations = (tzs: array(Lazy.tz)) => {
  Array.iter(lazyLoadLocation, tzs);
};

let getLocation = (name: string): option(tz) =>
  if (db^ |> DbMap.mem(name)) {
    Some(db^ |> DbMap.find(name));
  } else if (dbz^ |> DbMap.mem(name)) {
    let l: Lazy.tz = dbz^ |> DbMap.find(name);
    let tz = Tajm_Iana_Encoding.unmarshal(l.raw);
    loadLocation(tz);
    Some(tz);
  } else {
    None;
  };