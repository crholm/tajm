type zone = {
  offset: int,
  dst: bool,
  abbrev_idx: int,
};

type tz = {
  name: string,
  transitions: array(float),
  zone_idxs: array(int),
  zones: array(zone),
  abbrev: array(string),
  leap_seconds: array((float, int)),
  isstds: array(bool),
  isuts: array(bool),
};
