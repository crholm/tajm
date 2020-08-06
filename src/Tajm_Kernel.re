open Tajm_Type;

let fromFloat = Js.Date.fromFloat;
let toJs = (t: time_) => {
  t.t |> Int64.to_float |> fromFloat;
};

let getTZName = (t: time_) => {
  switch (t.loc) {
  | Fixed(name, _) => name
  | IANA(i) =>
    t.t |> Int64.to_float |> Tajm_Iana.period(i) |> Tajm_Iana.abbr(i)
  | Local =>
    // TODO: this is uglt since it returns the long JS name of the timezone
    // eg. "Central European Summer Time" instead of CEST
    // eg. "Central European Standard Time" instead of CET
    let jsName: float => string = [%bs.raw
      {|
        function (time) {
            return (new Date(time)).toLocaleString('en-US', {
                        hour12: false,
                        hour: '2-digit',
                        timeZoneName: 'long'
                    }).replace(/^\d\d /, '')
        }
    |}
    ];
    t.t |> Int64.to_float |> jsName;
  };
};

let getTimezoneOffset = (t: time_) => {
  (t |> toJs |> Js.Date.getTimezoneOffset) *. 60.0; // offset in secounds
};

let tzAdjustment = (t: time_): int => {
  1000
  * (
    switch (t.loc) {
    | Fixed(_, sec) => - sec
    | Local => t |> getTimezoneOffset |> int_of_float
    | IANA(i) =>
      t.t |> Int64.to_float |> Tajm_Iana.period(i) |> Tajm_Iana.offset(i)
    }
  );
};

let _adjustToZ = (t: time_): time_ => {
  let adj = t |> tzAdjustment;

  {t: Int64.sub(t.t, Int64.of_int(adj)), loc: Tajm_Const.z};
};
let _adjustFromZ = (t: time_): time_ => {
  let adj = t |> tzAdjustment;

  {...t, t: Int64.add(t.t, Int64.of_int(adj))};
};

let setUTCFullYearMD = Js.Date.setUTCFullYearMD;
let toISOString = Js.Date.toISOString;
let withYMDHMS =
    (~year, ~month, ~date, ~hours, ~minutes, ~seconds, loc: location_): time_ => {
  let ms =
    Js.Date.utcWithYMDHMS(
      ~year,
      ~month,
      ~date,
      ~hours,
      ~minutes,
      ~seconds,
      (),
    );
  {t: Int64.of_float(ms), loc} |> _adjustFromZ;
};
let now = Js.Date.now;

let getDay = (t: time_) => {
  t |> _adjustToZ |> toJs |> Js.Date.getUTCDay;
};
let getMonth = (t: time_) => {
  t |> _adjustToZ |> toJs |> Js.Date.getUTCMonth;
};
let getDate = (t: time_) => {
  t |> _adjustToZ |> toJs |> Js.Date.getUTCDate;
};
let getHours = (t: time_) => {
  t |> _adjustToZ |> toJs |> Js.Date.getUTCHours;
};
let getMinutes = (t: time_) => {
  t |> _adjustToZ |> toJs |> Js.Date.getUTCMinutes;
};
let getSeconds = (t: time_) => {
  t |> _adjustToZ |> toJs |> Js.Date.getUTCSeconds;
};
let getMilliseconds = (t: time_) => {
  t |> _adjustToZ |> toJs |> Js.Date.getUTCMilliseconds;
};
let getFullYear = (t: time_) => {
  t |> _adjustToZ |> toJs |> Js.Date.getUTCFullYear;
};

let string_of_float = Js.Float.toString;
