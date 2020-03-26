/*
  Setting Hundredths Register to 0 Using Hardware
  By: Andy England
  SparkFun Electronics
  Date: 3/3/2020
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14642

  This example shows how to set the hundredths register through hardware.

  Hardware Connections:
    Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
    Plug the RTC into the shield (any port)
    Open the serial monitor at 9600 baud
*/

#include <SparkFun_RV8803.h>

RV8803 rtc;

#define EVI_TRIGGER_PIN 13 //If you have a 3.3V microcontroller or a logic level converter, you can connect this pin to trigger the reset of the hundredths register

void setup() {

  Wire.begin();

  Serial.begin(115200);
  Serial.println("Set Hundredths using Hardware");

  if (rtc.begin() == false) {
    Serial.println("Something went wrong, check wiring");
  }
  else
  {
    Serial.println("RTC online!");
  }

  rtc.disableHardwareInterrupt(EVI_INTERRUPT); //Disbale the interrupt so we don't accidentally cause any based on this
  rtc.setEVICalibration(ENABLE); //Set's the RTC to reset the hundredths register on button press, or an external event. Must be run before the external event that you want to capture occurs.
  
  pinMode(EVI_TRIGGER_PIN, OUTPUT);
  digitalWrite(EVI_TRIGGER_PIN, HIGH); //Only write the pin HIGH if you are using a 3.3V microcontroller
  Serial.println("Ready for button press or external event");
}

void loop() {
  if (Serial.available()) //The 3.3V microcontroller waits for an 'r' to pull the pin to reset the hundredths register to :00
  {
    char resetCharacter = Serial.read();
    if (resetCharacter == 'r')
    {
      digitalWrite(EVI_TRIGGER_PIN, LOW); //The EVI pin is active low, this also clears the EVI calibration bit, so you will need to reset that bit before you try to reset hundredths using hardware again.
      rtc.updateTime(); //Grab time data from the RTC 
      uint8_t hundredths = rtc.getHundredths();
      Serial.print("Hundredths set to :");
      Serial.println(hundredths);
      delay(10);
      digitalWrite(EVI_TRIGGER_PIN, HIGH); 
      rtc.setEVICalibration(ENABLE); //Set the register once more to prepare it for another event
    }
  }
}
