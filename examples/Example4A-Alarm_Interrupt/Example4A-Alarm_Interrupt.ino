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

//Make sure to change these values to the decimal values that you want to match
uint8_t minuteAlarmValue = 55; //0-60, change this to a minute or two from now to see the alarm get generated
uint8_t hourAlarmValue = 0; //0-24
uint8_t weekdayAlarmValue = 0; //0-6
uint8_t dateAlarmValue = 0; //1-31

//Define which alarm registers we want to match, make sure you only enable weekday or date alarm, enabling both will default to a date alarm
//In it's current state, an alarm will be generated once an hour, when the MINUTES registers on the time and alarm match. Setting MINUTE_ALARM_ENABLE to false would trigger an alarm every minute
#define MINUTE_ALARM_ENABLE true
#define HOUR_ALARM_ENABLE false
#define WEEKDAY_ALARM_ENABLE false
#define DATE_ALARM_ENABLE false

void setup() {

  Wire.begin();

  Serial.begin(115200);
  Serial.println("Alarm from RTC Example");

  if (rtc.begin() == false) {
    Serial.println("Something went wrong, check wiring");
  }

  rtc.setItemsToMatchForAlarm(MINUTE_ALARM_ENABLE, HOUR_ALARM_ENABLE, WEEKDAY_ALARM_ENABLE, DATE_ALARM_ENABLE); //The alarm interrupt compares the alarm interrupt registers with the current time registers. We must choose which registers we want to compare by setting bits to true or false
  rtc.setAlarmMinute(minuteAlarmValue);
  rtc.setAlarmHour(hourAlarmValue);
  rtc.setAlarmWeekday(weekdayAlarmValue);
  //rtc.setAlarmDate(dateAlarmValue);
  rtc.disableAllInterrupts();
  //rtc.enableHardwareInterrupt(ALARM_INTERRUPT); //Uncomment this line if you'd like the Interrupt pin to pull low when the alarm flag goes up.
}

void loop() {
  if (rtc.getInterruptFlag(FLAG_ALARM)) //Check if our Alarm flag is up
  {
    Serial.println("Alarm Triggered, clearing flag");
    rtc.clearInterruptFlag(FLAG_ALARM);
    //rtc.clearAllInterruptFlags(); // This can also be used, but beware as it will clear the entire flag register
  }
  else
  {
    Serial.println("Waiting for alarm...");
  }
  delay(1000); //Wait 1 second to check again
}