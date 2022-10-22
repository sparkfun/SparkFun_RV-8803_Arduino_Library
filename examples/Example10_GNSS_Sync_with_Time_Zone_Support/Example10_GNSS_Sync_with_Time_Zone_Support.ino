/*
  Set the RV-8803 Real Time Clock using GNSS time and your local time zone
  By: Paul Clark
  SparkFun Electronics
  Date: October 21st 2022

  License: MIT
  
  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/16281

  This example shows how to set the RTC using UTC time from a u-blox GNSS receiver and your local time zone.
  The time zone is set in quarter hours (increments 15 mins).
  If your time zone is 7 hours behind UTC, the offset is -28.
  If daylight saving is still in effect, Mountain Time is 6 hours behind UTC. The offset is -24 quarter hours.
  The time zone is stored in battery-backed memory inside the RV8803.

  Note: the GNSS outputs its PVT (Position, Velocity, Time) message slightly ahead of time. The
  true "top of second" is indicated by the timing pulse / pulse per second pin. To set the time
  _precisely_, you need to use the TP / PPS pin... This example does it the 'easy' way and accepts
  that the RTC will be _slightly_ fast.

  This example also illustrates how the epoch is applied differently on different platforms.
  It shows how to use setEpoch on both older platforms (like AVR) and newer ones (like ESP32 v2.0.5).
  
  Uncomment the "#define useAVR" if you are running this on an older board.

  Hardware Connections:
    Connect the GNSS and RTC into the Qwiic port on your microcontroller board.
    Open the serial monitor at 115200 baud
*/

//#define useAVR // Uncomment this line if you are running this on an older AVR-like board

// Define your time zone here:

#define myTimeZone -24   // This will set your RTC to Mountain Time with Daylight Saving, 6 hours behind UTC
//#define myTimeZone -28 // This will set your RTC to Mountain Time, 7 hours behind UTC (28 quarter hours)
//#define myTimeZone 4   // This will set the RTC to UK Summer Time, 1 hour ahead of UTC (4 quarter hours)
//#define myTimeZone 0   // This will set the RTC to UTC / GMT

#include <SparkFun_RV8803.h> //Get the library here: http://librarymanager/All#SparkFun_RV8803

RV8803 rtc;

#include <SparkFun_u-blox_GNSS_Arduino_Library.h> //Click here to get the library: http://librarymanager/All#SparkFun_u-blox_GNSS

SFE_UBLOX_GNSS myGNSS;

void setup()
{
  Wire.begin();

  delay(2000);

  Serial.begin(115200);
  Serial.println(F("Set RTC with GNSS time - with Time Zone"));

  if (rtc.begin() == false)
  {
    Serial.println(F("The RTC did not begin successfully. Please check the wiring."));
  }
  else
  {
    Serial.println(F("RTC online!"));
  }

  // Set the time zone
  rtc.setTimeZoneQuarterHours(myTimeZone); // myTimeZone is int8_t (8-bit signed)

  // Use 24-hour mode
  rtc.set24Hour();

  // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  // Initialize the GNSS

  Serial.println(F("Initializing the GNSS..."));

  //myGNSS.enableDebugging(); // Uncomment this line to see _many_ helpful debug messages on Serial

  bool begun = false;
  do
  {
    begun = myGNSS.begin();
    if (!begun)
    {
      Serial.println(F("u-blox GNSS not detected on I2C bus."));
      delay(1000);
    }
  }
  while (begun == false);

  Serial.println(F("GNSS online!"));
  
  // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  // Wait for a 3D fix

  //myGNSS.factoryDefault(); delay(5000); // Uncomment this line to reset the module back to its factory defaults

  myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output only UBX

  //myGNSS.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Optional: save (only) the communications port settings to flash and BBR

  Serial.print(F("Waiting for a 3D fix"));

  uint8_t fix = 0;

  do
  {
    fix = myGNSS.getFixType();
    delay(1000);
    Serial.print(F("."));
  }
  while ( fix != 3 ); // Wait for a 3D fix

  Serial.println();
  
  Serial.println(F("GNSS is ready."));

  // Get fresh Position, Velocity, Time data from the GNSS
  bool pvtValid = false;
  do
  {
    pvtValid = myGNSS.getPVT();
  }
  while (pvtValid == false);


  // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

  rtc.setHundredthsToZero(); // Clear the RTC hundredths-of-seconds

  // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#ifdef useAVR
  // On older platforms like AVR (original SparkFun RedBoards and Arduino Uno):
  // Using the Year 2000 Epoch (as used by the AVR compiler and time library)

  rtc.setEpoch(myGNSS.getUnixEpoch(), true);

  // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#else
  // On newer platforms like ESP32 v2.0.5, the UNIX Epoch (1970s) is used automatically:

  rtc.setEpoch(myGNSS.getUnixEpoch());

#endif
}

void loop()
{
  if (myGNSS.getPVT() == true) // Get the latest GNSS time
  {
    // Delay reading the RTC by a small ammount
    //
    // The RTC and GNSS are now closely synchronised.
    // But there will be small timing differences when we read them over I2C.
    // This delay prevents us from seeing skipped and repeated seconds from the RTC.
    delay(100); 
    
    if (rtc.updateTime() == true) //Updates the time variables from RTC
    {
      // Print the GNSS time
      Serial.print(F("GNSS time: "));
      char theTime[12] = {0};
      snprintf(theTime, sizeof(theTime), "%02d:%02d:%02d", myGNSS.getHour(), myGNSS.getMinute(), myGNSS.getSecond());
      Serial.print((const char *)theTime);
  
      // Print the RTC time
      Serial.print(F("  RTC local time: "));
      Serial.print(rtc.stringTime());
      Serial.print(F("  "));
      Serial.print(rtc.stringDayOfWeek());
      Serial.print(F(", "));
      Serial.print(rtc.stringMonth());
      Serial.print(F(" "));
      Serial.print(rtc.stringDateOrdinal());
      Serial.print(F(", "));
      Serial.print(rtc.getYear());
      Serial.println();
    }
    else
    {
      Serial.print(F("RTC failed to update"));
    }
  }
  else
  {
    Serial.print(F("GNSS failed to update"));
  }

  // We do not need to delay here
  // getPVT will only return when fresh GNSS PVT data is received (or a timeout happens)
  //delay(1000);
}
