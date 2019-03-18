#include <math.h>

  
  
  
  Dusk2Dawn losAngeles(34.0522, -118.2437, -8);
  Dusk2Dawn antarctica(-77.85, 166.6667, 12);


  /*  Available methods are sunrise() and sunset(). Arguments are year, month,
   *  day, and if Daylight Saving Time is in effect.
   */
  int laSunrise  = losAngeles.sunrise(2017, 12, 31, false);
  int laSunset   = losAngeles.sunset(2017, 12, 31, false);
  int antSunrise = antarctica.sunrise(2017, 6, 30, true);


  /*  Time is returned in minutes elapsed since midnight. If no sunrises or
   *  sunsets are expected, a "-1" is returned.
   */
  Serial.println(laSunrise);  // 418
  Serial.println(laSunset);   // 1004
  Serial.println(antSunrise); // -1


  /*  A static method converts the returned time to a 24-hour clock format.
   *  Arguments are a character array and time in minutes.
   */
  char time[6];
  Dusk2Dawn::min2str(time, laSunrise);
  Serial.println(time); // 06:58


 /*  Alternatively, the array could be initialized with a dummy. This may be
  *  easier to remember.
  */
  char time2[] = "00:00";
  Dusk2Dawn::min2str(time2, laSunset);
  Serial.println(time2); // 16:53


  /*  Do some calculations with the minutes, then convert to time.
   */
  int laSolarNoon = laSunrise + (laSunset - laSunrise) / 2;
  char time3[] = "00:00";
  Dusk2Dawn::min2str(time3, laSolarNoon);
  Serial.println(time3); // 11:56


  /*  In case of an error, an error message is given. The static method also
   *  returns a false boolean value for error handling purposes.
   */
  char time4[] = "00:00";
  bool response = Dusk2Dawn::min2str(time4, antSunrise);
  if (response == false) {
    Serial.println(time4); // "ERROR"
    Serial.println("Uh oh!");
  }
