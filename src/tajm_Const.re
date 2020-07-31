open Tajm_Type;

let z = Fixed("UTC", 0);
let local = Local;

let fmtANSIC = "Mon Jan _2 15:04:05 2006";
let fmtUnixDate = "Mon Jan _2 15:04:05 MST 2006";
let fmtRubyDate = "Mon Jan 02 15:04:05 -0700 2006";
let fmtRFC822 = "02 Jan 06 15:04 MST";
let fmtRFC822Z = "02 Jan 06 15:04 -0700"; // RFC822 with numeric zone
let fmtRFC850 = "Monday, 02-Jan-06 15:04:05 MST";
let fmtRFC1123 = "Mon, 02 Jan 2006 15:04:05 MST";
let fmtRFC1123Z = "Mon, 02 Jan 2006 15:04:05 -0700"; // RFC1123 with numeric zone
let fmtRFC3339 = "2006-01-02T15:04:05Z07:00";
let fmtRFC3339ms = "2006-01-02T15:04:05.999Z07:00";
let fmtISOTimestamp = "2006-01-02T15:04:05Z07:00"; // ISO 8601
let fmtISOTimestampMs = "2006-01-02T15:04:05.000Z07:00"; // ISO 8601
// let fmtISOWeek = "2006-W31"; // ISO 8601
// let fmtISOWeekDay = "2006-W31-7"; // ISO 8601
let fmtKitchen = "3:04PM";
