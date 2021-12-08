/*
  Set the Hundredths Register to 0 using I2C
  By: Andy England
  SparkFun Electronics
  Date: 2/22/2017
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14642

  This example shows how to set the hundredths register through the I2C interface.

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
  Serial.println("Set Hundredths to 0");

  if (rtc.begin() == false)
  {
    Serial.println("Device not found. Please check wiring. Freezing.");
    while (1);
  }
  Serial.println("RTC online!");

  Serial.println("Press r to reset hundredths to zero");
}

void loop()
{
  if (Serial.available())
  {
    char incoming = Serial.read();
    if (incoming == 'r') //The microcontroller waits for an 'r' to reset the hundredths register to :00
    {
      rtc.setHundredthsToZero(); //This function resets the hundredths register to :00. Note that you can call this function based on any event.
      rtc.updateTime(); //Grab time data from the RTC
      uint8_t hundredths = rtc.getHundredths();
      Serial.print("Hundredths set to :");
      Serial.println(hundredths);
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