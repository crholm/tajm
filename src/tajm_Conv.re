open Tajm_Type;

let stringOfWeekday = (weekday: weekday_): string => {
  // TODO apply timezone adjustment
  switch (weekday) {
  | Monday => "Monday"
  | Tuesday => "Tuesday"
  | Wednesday => "Wednesday"
  | Thursday => "Thursday"
  | Friday => "Friday"
  | Saturday => "Saturday"
  | Sunday => "Sunday"
  };
};

let stringOfWeekdayShort = (weekday: weekday_): string => {
  // TODO apply timezone adjustment
  switch (weekday) {
  | Monday => "Mon"
  | Tuesday => "Tue"
  | Wednesday => "Wed"
  | Thursday => "Thu"
  | Friday => "Fri"
  | Saturday => "Sat"
  | Sunday => "Sun"
  };
};

let intOfWeekday = (weekday: weekday_): int => {
  switch (weekday) {
  | Monday => 0
  | Tuesday => 1
  | Wednesday => 2
  | Thursday => 3
  | Friday => 4
  | Saturday => 5
  | Sunday => 6
  };
};
let weekdayOfInt = (i: int): weekday_ => {
  switch (abs(i) mod 7) {
  | 0 => Monday
  | 1 => Tuesday
  | 2 => Wednesday
  | 3 => Thursday
  | 4 => Friday
  | 5 => Saturday
  | 6 => Sunday
  | _ => raise(Failure("math is broken"))
  };
};

let intOfMonth = (month: month_): int => {
  // TODO apply timezone adjustment
  switch (month) {
  | January => 1
  | February => 2
  | March => 3
  | April => 4
  | May => 5
  | June => 6
  | July => 7
  | August => 8
  | September => 9
  | October => 10
  | November => 11
  | December => 12
  };
};
let monthOfInt = (i: int): month_ => {
  // TODO apply timezone adjustment
  switch (abs(i) mod 12) {
  | 1 => January
  | 2 => February
  | 3 => March
  | 4 => April
  | 5 => May
  | 6 => June
  | 7 => July
  | 8 => August
  | 9 => September
  | 10 => October
  | 11 => November
  | 0 => December
  | _ => raise(Failure("math is broken"))
  };
};
let stringOfMonth = (month: month_): string => {
  // TODO apply timezone adjustment
  switch (month) {
  | January => "January"
  | February => "February"
  | March => "March"
  | April => "April"
  | May => "May"
  | June => "June"
  | July => "July"
  | August => "August"
  | September => "September"
  | October => "October"
  | November => "November"
  | December => "December"
  };
};
let stringOfMonthShort = (month: month_): string => {
  // TODO apply timezone adjustment
  switch (month) {
  | January => "Jan"
  | February => "Feb"
  | March => "Mar"
  | April => "Apr"
  | May => "May"
  | June => "Jun"
  | July => "Jul"
  | August => "Aug"
  | September => "Sep"
  | October => "Oct"
  | November => "Nov"
  | December => "Dec"
  };
};

let daysInYear = (year: int): int => {
  let leap = year mod 400 === 0 || year mod 4 === 0 && year mod 100 !== 0;
  leap ? 366 : 365;
};
let daysInMonth = (year: int, month: month_): int => {
  switch (month) {
  | January => 31
  | February => daysInYear(year) == 366 ? 29 : 28
  | March => 31
  | April => 30
  | May => 31
  | June => 30
  | July => 31
  | August => 31
  | September => 30
  | October => 31
  | November => 30
  | December => 31
  };
};
