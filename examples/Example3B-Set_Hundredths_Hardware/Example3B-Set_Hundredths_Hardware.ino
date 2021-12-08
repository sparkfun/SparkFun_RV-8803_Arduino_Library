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
    Plug the RTC into the Qwiic port on your microcontroller or on your Qwiic shield/adapter.
    If you are using an adapter cable, here is the wire color scheme:
    Black=GND, Red=3.3V, Blue=SDA, Yellow=SCL
    Open the serial monitor at 115200 baud
*/

#include <SparkFun_RV8803.h> //Get the library here:http://librarymanager/All#SparkFun_RV-8803

RV8803 rtc;

#define EVI_TRIGGER_PIN 13 //If you have a 3.3V microcontroller or a logic level converter, you can connect this pin to trigger the reset of the hundredths register

void setup()
{
  Wire.begin();

  Serial.begin(115200);
  Serial.println("Set Hundredths using Hardware");

  if (rtc.begin() == false)
  {
    Serial.println("Device not found. Please check wiring. Freezing.");
    while (1);
  }
  Serial.println("RTC online!");

  rtc.disableHardwareInterrupt(EVI_INTERRUPT); //Disbale the interrupt so we don't accidentally cause any based on this
  rtc.setEVICalibration(RV8803_ENABLE); //Sets the RTC to reset the hundredths register on button press or an external event. Must be run before the external event that you want to capture occurs.

  pinMode(EVI_TRIGGER_PIN, OUTPUT);
  digitalWrite(EVI_TRIGGER_PIN, HIGH); //Only write the pin HIGH if you are using a 3.3V microcontroller
  Serial.println("Ready for button press or external event");
}

void loop()
{
  if (Serial.available())
  {
    char incoming = Serial.read();
    if (incoming == 'r') //The 3.3V microcontroller waits for an 'r' to pull the pin to reset the hundredths register to :00
    {
      digitalWrite(EVI_TRIGGER_PIN, LOW); //The EVI pin is active LOW, this also clears the EVI calibration bit. Reset that bit before you try to reset hundredths using hardware again. This is performed at the end of the loop in this example.
      rtc.updateTime(); //Grab time data from the RTC
      uint8_t hundredths = rtc.getHundredths();
      Serial.print("Hundredths set to :");
      Serial.println(hundredths);
      delay(10);
      digitalWrite(EVI_TRIGGER_PIN, HIGH);
      rtc.setEVICalibration(RV8803_ENABLE); //Set the register once more to prepare it for another event
    }
  }

  if (rtc.updateTime() == true) //Updates the time variables from RTC
  {
    String currentDate = rtc.stringDateUSA(); //Get the current date in mm/dd/yyyy format (we're weird)
    //String currentDate = rtc.stringDate(); //Get the current date in dd/mm/yyyy format
    String currentTime = rtc.stringTime(); //Get the time
    Serial.print(currentDate);
    Serial.print(" ");
    Serial.println(currentTime);
  }
  else
  {
    Serial.print("RTC read failed");
  }

  delay(1000);
}