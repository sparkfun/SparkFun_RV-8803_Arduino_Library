/*
  Set the RV-8803 Real Time Clock using UNIX Epoch time and your local time zone
  By: Paul Clark
  SparkFun Electronics
  Date: October 21st 2022

  License: MIT
  
  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/16281

  This example shows how to set the time zone and then apply it using setEpoch.
  The RTC is set using the sum of the Unix (UTC) Epoch and the time zone.
  The time zone is set in quarter hours (increments 15 mins).
  If your time zone is 7 hours behind UTC, the offset is -28.
  The time zone is stored in battery-backed memory inside the RV8803.
  You can also set the RTC using your local epoch, in which case the time zone is
  ignored when setting the RTC but the ISO8601 timestamp will still be correct.

  This example also illustrates how the epoch is applied differently on different platforms.
  It shows how to use setEpoch on both older platforms (like AVR) and newer ones (like ESP32 v2.0.5).
  
  Uncomment the "#define useAVR" if you are running this on an older board.

  Hardware Connections:
    Plug the RTC into the Qwiic port on your microcontroller or on your Qwiic shield/adapter.
    If you are using an adapter cable, here is the wire color scheme:
    Black=GND, Red=3.3V, Blue=SDA, Yellow=SCL
    Open the serial monitor at 115200 baud
*/

//#define useAVR // Uncomment this line if you are running this on an older AVR-like board

#include <SparkFun_RV8803.h> //Get the library here: http://librarymanager/All#SparkFun_RV8803

RV8803 rtc;

void setup()
{
  Wire.begin();

  delay(2000);

  Serial.begin(115200);
  Serial.println("Set UNIX Epoch Time Example - with Time Zone");

  if (rtc.begin() == false) {
    Serial.println("Something went wrong, check wiring");
  }
  else
  {
    Serial.println("RTC online!");
  }

  // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#ifdef useAVR
  // On older platforms like AVR (original SparkFun RedBoards and Arduino Uno):

  // Set the RTC time to 05:00 (5AM) Mountain Time (UTC -7 Hours) on Thursday, December 31, 2020
  // This is midday 12:00:00 (GMT / UTC). The Unix Epoch UTC timestamp is 1609416000.
  // The local epoch is 1609416000 - (7 * 60 * 60) = 1609390800
  // See https://www.unixtimestamp.com/ for more details
  
  // Using the Year 2000 Epoch (as used by the AVR compiler and time library)
  // The UTC epoch is: 662731200
  // The local epoch is: 662706000
  
  // Set the time zone to Mountain Time: 7 * 4 quarter hours behind UTC
  rtc.setTimeZoneQuarterHours(-28);

  // Set the RTC time using the UTC Unix timestamp and the UNIX Epoch (1970s)
  rtc.setEpoch(1609416000, true);

  // Or, we could set the time with the local epoch
  //rtc.setLocalEpoch(1609390800, true);

  // Or, we could use UTC and the Year 2000 epoch
  //rtc.setEpoch(662731200);

  // Or, we could use Local Time the Year 2000 epoch
  //rtc.setLocalEpoch(662706000);

  // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#else
  // On newer platforms like ESP32 v2.0.5, the UNIX Epoch (1970s) is used automatically:

  // Set the RTC time to 05:00 (5AM) Mountain Time (UTC -7 Hours) on Thursday, December 31, 2020
  // This is midday 12:00:00 (GMT / UTC). The Unix Epoch UTC timestamp is 1609416000.
  // The local epoch is 1609416000 - (7 * 60 * 60) = 1609390800
  // See https://www.unixtimestamp.com/ for more details

  // Set the time zone to Mountain Time: 7 * 4 quarter hours behind UTC
  rtc.setTimeZoneQuarterHours(-28);

  // Set the RTC time using the UTC Unix timestamp
  rtc.setEpoch(1609416000);

  // Or, we could set the time with the local epoch
  //rtc.setLocalEpoch(1609390800);

  // Or, if we wanted to go totally crazy, we could do this:
  //rtc.setTimeZoneQuarterHours(0);
  //rtc.setEpoch(1609390800);
  //rtc.setTimeZoneQuarterHours(-28);

#endif
}

void loop()
{
  if (rtc.updateTime() == true) //Updates the time variables from RTC
  {
    // Print the time in ISO8601 format, including time zone:
    Serial.print(rtc.stringTime8601TZ());

    // Print the date and time in a user-friendly way:
    Serial.print("\t");
    Serial.print(rtc.stringDayOfWeek());
    Serial.print(", ");
    Serial.print(rtc.stringMonth());
    Serial.print(" ");
    Serial.print(rtc.stringDateOrdinal());
    Serial.print(", ");
    Serial.print(rtc.getYear());

  // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#ifdef useAVR
  // On older platforms like AVR (original SparkFun RedBoards and Arduino Uno):

    // Print the UTC epoch and the local epoch
    Serial.print("\tUTC Epoch: ");
    Serial.print(rtc.getEpoch(true)); // Use the UNIX Epoch (1970s)
    Serial.print("  Local Epoch: ");
    Serial.println(rtc.getLocalEpoch(true)); // Use the UNIX Epoch (1970s)

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#else
  // On newer platforms like ESP32 v2.0.5, the UNIX Epoch (1970s) is used automatically:

    // Print the UTC epoch and the local epoch
    Serial.print("\tUTC Epoch: ");
    Serial.print(rtc.getEpoch());
    Serial.print("  Local Epoch: ");
    Serial.println(rtc.getLocalEpoch());

#endif
}
  else
  {
    Serial.print("RTC failed to update");
  }

  delay(1000);
}
