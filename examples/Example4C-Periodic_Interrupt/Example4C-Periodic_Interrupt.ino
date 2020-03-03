/*
  Getting the alarm to fire an interrupt on the RV-8803 Real Time Clock
  By: Andy England
  SparkFun Electronics
  Date: 2/22/2017
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14642

  This example shows how to set an alarm and make the RTC generate an interrupt when the clock time matches the alarm time
  The INT pin will be 3.3V. When the real time matches the alarm time the INT pin will go low.

  Hardware Connections:
    Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
    Plug the RTC into the shield (any port)
    Open the serial monitor at 115200 baud
*/

#include <SparkFun_RV8803.h>

RV8803 rtc;

long lastInterruptTime;

void setup() {

  Wire.begin();

  Serial.begin(115200);
  Serial.println("Alarm from RTC Example");

  if (rtc.begin() == false) {
    Serial.println("Something went wrong, check wiring");
  }
  else
  {
    Serial.println("RTC online!");
  }
  rtc.disableAllInterrupts();
  rtc.setPeriodicTimeUpdateFrequency(TIME_UPDATE_1_SECOND); //Can also use TIME_UPDATE_1_MINUTE (TIME_UPDATE_1_SECOND = false, TIME_UPDATE_1_MINUTE = true)
  rtc.enableHardwareInterrupt(UPDATE_INTERRUPT); //The update interrupt needs to have the hardware interrupt enabled to function
  lastInterruptTime = millis();
}

void loop() {
  if(rtc.getInterruptFlag(FLAG_UPDATE))
  {
    long timeSinceLastInterrupt = millis() - lastInterruptTime; //Change millis() to micros() if you end up using the 4096 Hz counter
    lastInterruptTime = millis(); //Change millis() to micros() if you end up using the 4096 Hz counter
    rtc.clearInterruptFlag(FLAG_UPDATE);
    //rtc.clearAllInterruptFlags(); // This can also be used, but beware as it will clear the entire flag register
    Serial.print("Time between interrupts: ");
    Serial.print(timeSinceLastInterrupt); 
    Serial.println(" mS");
  }
}
