open Tajm_Iana_Tz;

let period = (i: tz, f: float): int => {
  let len = i.transitions |> Array.length;
  let rec find = (n: int) =>
    if (n == len) {
      n - 1;
    } else if (f < i.transitions[n]) {
      n - 1;
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

let db = ref([||]: array(tz));
let dbz = ref([||]: array(Lazy.tz));

let loadLocations = (tzs: array(tz)) => {
  db := Array.append(tzs, db^);
};
let loadLocation = (tz: tz) => {
  loadLocations([|tz|]);
};

let getLocation = (name: string): option(tz) => {
  let o = db^ |> Tajm_Functions_Array.find(a => a.name == name);
  switch (o) {
  | Some(t) => Some(t)
  | None =>
    let l = dbz^ |> Tajm_Functions_Array.find((a: Lazy.tz) => a.name == name);
    switch (l) {
    | Some(lz) =>
      let loc = Tajm_Iana_Encoding.unmarshal(lz.raw);
      loadLocation(loc);
      Some(loc);
    | None => None
    };
  };
};

let lazyLoadLocations = (tzs: array(Lazy.tz)) => {
  dbz := Array.append(tzs, dbz^);
};
let lazyLoadLocation = (tz: Lazy.tz) => {
  lazyLoadLocations([|tz|]);
};
