/*
  Trickle Charging the RV-8803 Real Time Clock
  By: Andy England
  SparkFun Electronics
  Date: 2/22/2017
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14642

  This example shows how to set the hundredths register through software.

  Hardware Connections:
    Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
    Plug the RTC into the shield (any port)
    Open the serial monitor at 9600 baud
*/

#include <SparkFun_RV8803.h>

RV8803 rtc;

void setup() {

  Wire.begin();

  Serial.begin(115200);
  Serial.println("Read Time from RTC Example");

  if (rtc.begin() == false) {
    Serial.println("Something went wrong, check wiring");
  }
  
  Serial.println("RTC online!");
}

void loop() {

  if(Serial.available()) //The microcontroller waits for an 'r' to reset the hundredths register to :00
  {
    char resetCharacter = Serial.read();
    if (resetCharacter == 'r')
    {
      rtc.setHundredthsToZero(); //This function resets the hundredths register to :00. Note that you can call this based on any event.
      Serial.println("Hundredths set to :00");
    }
  }
}
