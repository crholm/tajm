type month_ =
  | January
  | February
  | March
  | April
  | May
  | June
  | July
  | August
  | September
  | October
  | November
  | December;

type weekday_ =
  | Monday
  | Tuesday
  | Wednesday
  | Thursday
  | Friday
  | Saturday
  | Sunday;

type location_ =
  | Fixed(string, int)
  | Local
  | IANA(Tajm_Iana_Tz.tz);

type time_ = {
  t: int64,
  loc: location_,
};

type duration_ = float;

type timeunit_ =
  | Year
  | Month
  | Day
  | Hour
  | Minute
  | Second
  | Millisecond;
