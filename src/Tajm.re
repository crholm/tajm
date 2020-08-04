include Tajm_Type;
include Tajm_Const;
include Tajm_Func;

module Duration = Tajm_Duration;
module Days = Tajm_Days;
module Is = Tajm_Is;
module Conv = Tajm_Conv;

let format = (format: string, t: time_) => {
  Tajm_Fmt.tokenize([], format) |> Tajm_Fmt.printer(t);
};
let parse = (format: string, t: string) => {
  Tajm_Fmt.tokenize([], format) |> Tajm_Fmt.parse(t);
};

let string = (t: time_): string => {
  t |> atUTC |> format(fmtISOTimestampMs);
};

let toJs = Tajm_Kernel.toJs;
