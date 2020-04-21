/*
  Prints the UNIX Epoch time from the RV-8803 Real Time Clock
  By: Andy England
  SparkFun Electronics
  Updated by: Adam Garbo
  Date: 4/21/2020
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

#include <SparkFun_RV8803.h>

RV8803 rtc;

void setup() {

  Wire.begin();

  Serial.begin(115200);
  Serial.println("Print UNIX Epoch Time Example");

  if (rtc.begin() == false) {
    Serial.println("Something went wrong, check wiring");
  }
  else
  {
    Serial.println("RTC online!");
  }
}

void loop() {

  if (rtc.updateTime() == false) //Updates the time variables from RTC
  {
    Serial.print("RTC failed to update");
  }
  
  // Get the time in ISO 8601 format
  String currentTime = rtc.stringTime8601();
  Serial.println(currentTime);

  // Get the UNIX Epoch time
  unsigned long unixtime = rtc.getEpoch();
  Serial.println(unixtime);

  delay(1000);
}
