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
  let idx = i.zones[period].abbrev_idx;
  i.abbrev[idx];
};

let db = ref([]: list(tz));

let getLocation = (name: string): option(tz) =>
  try({
    let i = db^ |> List.find(a => a.name == name);
    Some(i);
  }) {
  | _ => None
  };

let loadLocation = (tz: tz) => {
  db := [tz] @ db^;
};
let loadLocations = (tzs: list(tz)) => {
  db := tzs @ db^;
};
