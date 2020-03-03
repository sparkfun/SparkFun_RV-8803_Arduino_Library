/*
  Grab a Timestamp from the most recent button press/EVI pin event
  By: Andy England
  SparkFun Electronics
  Date: 2/22/2017
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14642

  This example shows how to get the timestamp of an event generated on the EVI pin, either by a button press or an MCU generated event

  Hardware Connections:
    Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
    Plug the RTC into the shield (any port)
    Open the serial monitor at 115200 baud
*/

#include <SparkFun_RV8803.h>

RV8803 rtc;

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
  
  rtc.setEVIEventCapture(ENABLE); //Enables the Timestamping function
  rtc.setEVIDebounceTime(EVI_DEBOUNCE_256HZ); //Debounce the button with a 3.9 ms(256 Hz) sampling period, other options are EVI_DEBOUNCE_NONE, EVI_DEBOUNCE_64HZ, and EVI_DEBOUNCE_8HZ
  //rtc.setEVIEdgeDetection(RISING_EDGE); // Uncomment to set event detection to button release instead of press
}

void loop() {
  if (rtc.getInterruptFlag(FLAG_EVI))
  {
    rtc.updateTime();
    rtc.clearInterruptFlag(FLAG_EVI);
    String currentDate = rtc.stringDateUSA(); //Get the current date in mm/dd/yyyy format (we're weird)
    String timestamp = rtc.stringTimestamp();
    Serial.print(currentDate);
    Serial.print(" ");
    Serial.println(timestamp);
  }
}
