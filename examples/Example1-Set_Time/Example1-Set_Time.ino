/*
  Setting time from the RV-8803 Real Time Clock
  By: Andy England
  SparkFun Electronics
  Date: 3/3/2020
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14642

  This example shows how to set the time on the RTC to the compiler time or a custom time.

  Hardware Connections:
    Plug the RTC into the Qwiic port on your microcontroller or on your Qwiic shield/adapter.
    If you are using an adapter cable, here is the wire color scheme:
    Black=GND, Red=3.3V, Blue=SDA, Yellow=SCL
    Open the serial monitor at 115200 baud
*/

#include <SparkFun_RV8803.h> //Get the library here:http://librarymanager/All#SparkFun_RV-8803

RV8803 rtc;

//The below variables control what the date and time will be set to
int sec = 2;
int minute = 47;
int hour = 14; //Set things in 24 hour mode
int date = 2;
int month = 3;
int year = 2020;
int weekday = 2;

void setup()
{
  Wire.begin();

  Serial.begin(115200);
  Serial.println("Set Time on RTC");

  if (rtc.begin() == false)
  {
    Serial.println("Device not found. Please check wiring. Freezing.");
    while(1);
  }
  Serial.println("RTC online!");

  //Use the time from the Arduino compiler (build time) to set the RTC
  //Keep in mind that Arduino does not get the new compiler time every time it compiles. 
  //To ensure the proper time is loaded, open up a fresh version of the IDE and load the sketch.
  //Also note that due to upload times, compiler time may be a little bit off on seconds/hundredths
  if (rtc.setToCompilerTime() == false)
    Serial.println("Something went wrong setting the time");
  else
    Serial.println("New time set!");

  //Uncomment the below code to set the RTC to your own time
  /*if (rtc.setTime(sec, minute, hour, weekday, date, month, year) == false) {
    Serial.println("Something went wrong setting the time");
    }*/
  //rtc.set24Hour(); //Uncomment this line if you'd like to set the RTC to 24 hour mode
}

void loop()
{
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