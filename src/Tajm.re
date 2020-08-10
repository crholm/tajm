include Tajm_Type;
include Tajm_Const;
include Tajm_Func;

module Duration = Tajm_Duration;
module Is = Tajm_Is;
module Conv = Tajm_Conv;

/*Formats a time to a string in accordance to the layout string.
  The layout is borrowed from golang time library
   with some addition and changes. It provides a human friendly
   way for developers to see what the expected output will be.
   */
let format = (layout: string, t: time_) => {
  Tajm_Fmt.tokenize([], layout) |> Tajm_Fmt.printer(t);
};

/*Parses a string into a time given a specific layout*/
let parse = (layout: string, t: string) => {
  Tajm_Fmt.tokenize([], layout) |> Tajm_Fmt.parse(t);
};

/*Returns a ISO Timestamp formatted string of the time*/
let string = (t: time_): string => {
  t |> atUTC |> format(fmtISOTimestampMs);
};

let toJs = Tajm_Kernel.toJs;
let ofJs = Tajm_Kernel.ofJs;