/*
  Set the RV-8803 Real Time Clock using UNIX Epoch time
  By: Andy England
  SparkFun Electronics
  Updated by: Adam Garbo
  Date: 4/25/2020

  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14642

  This example shows how to print the time from the RTC.

  This example also illustrates how the epoch is applied differently on different platforms.
  It shows how to use setEpoch on both older platforms (like AVR) and newer ones (like ESP32).
  
  Uncomment the "#define useAVR" if you are running this on an older board.

  Hardware Connections:
    Plug the RTC into the Qwiic port on your microcontroller or on your Qwiic shield/adapter.
    If you are using an adapter cable, here is the wire color scheme:
    Black=GND, Red=3.3V, Blue=SDA, Yellow=SCL
    Open the serial monitor at 115200 baud
*/

//#define useAVR // Uncomment this line if you are running this on an older AVR-like board

#include <SparkFun_RV8803.h> //Get the library here:http://librarymanager/All#SparkFun_RV-8803

RV8803 rtc;

void setup()
{
  Wire.begin();

  delay(1000); // Delay for the ESP32

  Serial.begin(115200);
  Serial.println("Set UNIX Epoch Time Example");

  if (rtc.begin() == false) {
    Serial.println("Something went wrong, check wiring");
  }
  else
  {
    Serial.println("RTC online!");
  }

  rtc.setTimeZoneQuarterHours(0); // Make sure the time zone is zero, otherwise getEpoch will be offset

  // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#ifdef useAVR
  // On older platforms like AVR (original SparkFun RedBoards and Arduino Uno):

  // Set the RTC time to Thursday, December 31, 2020 12:00:00 (GMT / UTC)
  // The Unix Epoch UTC timestamp is 1609416000.

  // Using the Year 2000 Epoch (as used by the AVR compiler and time library)
  // The UTC epoch is: 662731200
  rtc.setEpoch(662731200);

  // Set the RTC time using UNIX 1970s Epoch time. See e.g. https://www.epoch101.com/
  // This subtracts 946684800 from the value before sending to RTC.
  //rtc.setEpoch(1609416000, true);

  // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#else
  // On newer platforms like ESP32 v2.0.5, the UNIX Epoch (1970s) is used automatically:

  rtc.setEpoch(1609416000);

#endif

  // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

}

void loop()
{
  if (rtc.updateTime() == true) //Updates the time variables from RTC
  {
    // Print the current Epoch

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#ifdef useAVR
    // On older platforms like AVR (original SparkFun RedBoards and Arduino Uno):

    unsigned long epochTime = rtc.getEpoch(true); // Use the UNIX Epoch (1970s)

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#else
  // On newer platforms like ESP32 v2.0.5, the UNIX Epoch (1970s) is used automatically:

    unsigned long epochTime = rtc.getEpoch();

#endif

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Serial.println(epochTime);

    // Confirm the date and time using the ISO 8601 timestamp
    String currentTime = rtc.stringTime8601();
    Serial.println(currentTime);
  }
  else
  {
    Serial.print("RTC failed to update");
  }

  delay(1000);
}