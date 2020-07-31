include Tajm_Type;
include Tajm_Const;
include Tajm_Fun;

module Duration = Tajm_Duration;
module Days = Tajm_Days;
module Is = Tajm_Is;
module Conv = Tajm_Conv;

let format = (s: string, t: time_) => {
  Tajm_Fmt.tokenize([], s) |> Tajm_Fmt.printer(t);
};

let string = (t: time_): string => {
  t |> inUTC |> format(fmtISOTimestampMs);
};

let toJs = Tajm_Kernel.toJs;
