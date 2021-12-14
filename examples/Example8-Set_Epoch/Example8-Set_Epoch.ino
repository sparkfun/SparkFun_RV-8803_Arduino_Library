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

  Hardware Connections:
    Plug the RTC into the Qwiic port on your microcontroller or on your Qwiic shield/adapter.
    If you are using an adapter cable, here is the wire color scheme:
    Black=GND, Red=3.3V, Blue=SDA, Yellow=SCL
    Open the serial monitor at 115200 baud
*/

#include <SparkFun_RV8803.h> //Get the library here:http://librarymanager/All#SparkFun_RV-8803

RV8803 rtc;

void setup()
{
  Wire.begin();

  Serial.begin(115200);
  Serial.println("Set UNIX Epoch Time Example");

  if (rtc.begin() == false) {
    Serial.println("Something went wrong, check wiring");
  }
  else
  {
    Serial.println("RTC online!");
  }

  // Set the RTC time using UNIX Epoch time
  rtc.setEpoch(1639007837); // E.g. https://www.epoch101.com/

  // Set the RTC time using UNIX 1970s Epoch time. This subtracts 946710000 from the value before sending to RTC.
  //rtc.setEpoch(2585717837, true); // E.g. https://www.epoch101.com/
}

void loop()
{
  if (rtc.updateTime() == true) //Updates the time variables from RTC
  {
    // Print the current UNIX Epoch time
    unsigned long unixtime = rtc.getEpoch();
    Serial.println(unixtime);

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
