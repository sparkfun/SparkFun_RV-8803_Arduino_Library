/******************************************************************************
SparkFun_RV8803.h
RV8803 Arduino Library
Andy England @ SparkFun Electronics
February 5, 2018
https://github.com/sparkfun/Qwiic_RTC

Resources:
Uses Wire.h for i2c operation
Uses SPI.h for SPI operation

Development environment specifics:
Arduino IDE 1.6.4

This code is released under the [MIT License](http://opensource.org/licenses/MIT).
Please review the LICENSE.md file included with this example. If you have any questions 
or concerns with licensing, please contact techsupport@sparkfun.com.
Distributed as-is; no warranty is given.
******************************************************************************/

#pragma once

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

//The 7-bit I2C address of the RV8803
#define RV8803_ADDR						0b1010010


//Register names:
#define RV8803_RAM						0x07
#define RV8803_HUNDREDTHS				0x10
#define RV8803_SECONDS					0x11
#define RV8803_MINUTES					0x12
#define RV8803_HOURS					0x13
#define RV8803_WEEKDAYS					0x14
#define RV8803_DATE         			0x15
#define RV8803_MONTHS        			0x16
#define RV8803_YEARS        			0x17
#define RV8803_MINUTES_ALARM     		0x18
#define RV8803_HOURS_ALARM       		0x19
#define RV8803_WEEKDAYS_DATE_ALARM   	0x1A
#define RV8803_TIMER_0					0x1B
#define RV8803_TIMER_1					0x1C
#define RV8803_EXTENSION				0x1D
#define RV8803_FLAG						0x1E
#define RV8803_CONTROL					0x1F
#define RV8803_HUNDREDTHS_TIMESTAMP		0x20
#define RV8803_SECONDS_TIMESTAMP		0x21
#define RV8803_OFFSET					0x2C
#define RV8803_EVENT_CONTROL			0x2F

//Enable Bits for Alarm Registers
#define ALARM_ENABLE_MINUTE				7
#define ALARM_ENABLE_HOUR				7
#define ALARM_ENABLE_WEEKDAY_DATE		7

//Extension Register Bits
#define EXTENSION_TEST					7
#define EXTENSION_WADA					6
#define EXTENSION_USEL					5
#define EXTENSION_TE					4
#define EXTENSION_FD					2
#define EXTENSION_TD					0

//Flag Register Bits
#define FLAG_UF							5
#define FLAG_TF							4
#define FLAG_AF							3
#define FLAG_EVF						2
#define FLAG_V2F						1
#define FLAG_V1F						0

//Control Register Bits
#define CONTROL_UIE						5
#define CONTROL_TIE						4
#define CONTROL_AIE						3
#define CONTROL_EIE						2
#define CONTROL_RESET					0

//Event Control Bits
#define EVENT_ECP						7
#define EVENT_EHL						6
#define EVENT_ET						5
#define EVENT_ERST						0

//Possible Settings
#define 12_HOUR_MODE					true
#define 24_HOUR_MODE					false
#define WEEKDAY_ALARM					false
#define DATE_ALARM						true

#define TIME_ARRAY_LENGTH 8 // Total number of writable values in device

enum time_order {
	TIME_HUNDREDTHS, // 0
	TIME_SECONDS,    // 1
	TIME_MINUTES,    // 2
	TIME_HOURS,      // 3
	TIME_WEEKDAY,	 // 4
	TIME_DATE,       // 5
	TIME_MONTH,      // 6
	TIME_YEAR,       // 7
};

class RV8803
{
  public:
	
    RV8803( void );

    boolean begin( TwoWire &wirePort = Wire);

	bool setTime(uint8_t sec, uint8_t min, uint8_t hour, uint8_t date, uint8_t month, uint8_t year, uint8_t day);
	bool setTime(uint8_t * time, uint8_t len);
	bool setSeconds(uint8_t value);
	bool setMinutes(uint8_t value);
	bool setHours(uint8_t value);
	bool setDate(uint8_t value);
	bool setWeekday(uint8_t value);
	bool setMonth(uint8_t value);
	bool setYear(uint8_t value);
	
	bool updateTime(); //Update the local array with the RTC registers
	
	char* stringDateUSA(); //Return date in mm-dd-yyyy
	char* stringDate(); //Return date in dd-mm-yyyy
	char* stringTime(); //Return time hh:mm:ss with AM/PM if in 12 hour mode
	char* stringTimeStamp(); //Return timeStamp in ISO 8601 format yyyy-mm-ddThh:mm:ss
	
	uint8_t getSeconds();
	uint8_t getMinutes();
	uint8_t getHours();
	uint8_t getDate();
	uint8_t getWeekday();
	uint8_t getMonth();
	uint8_t getYear();	
	
	bool setToCompilerTime(); //Uses the hours, mins, etc from compile time to set RTC
	
	bool is12Hour(); //Returns true if 12hour bit is set
	bool isPM(); //Returns true if is12Hour and PM bit is set
	void set12Hour();
	void set24Hour();
	
	uint8_t status(); //Returns the status byte
	
	bool setAlarm(uint8_t sec, uint8_t min, uint8_t hour, uint8_t date, uint8_t month);
	bool setAlarm(uint8_t * time, uint8_t len);
	void setAlarmMode(uint8_t mode); //0 to 7, alarm goes off with match of second, minute, hour, etc
 	
	void setCountdownTimer(uint8_t duration, uint8_t unit, bool repeat = true, bool pulse = true);

	void enableInterrupt(uint8_t source); //Enables a given interrupt within Interrupt Enable register
	void disableInterrupt(uint8_t source); //Disables a given interrupt within Interrupt Enable register
	void enableAlarmInterrupt(); //Use in conjuction with setAlarm and setAlarmMode
	
	void clearInterrupts();
		
	//Values in RTC are stored in Binary Coded Decimal. These functions convert to/from Decimal
	uint8_t BCDtoDEC(uint8_t val); 
	uint8_t DECtoBCD(uint8_t val);

	void reset(void); //Fully reset RTC to all zeroes
	
    uint8_t readRegister(uint8_t addr);
    bool writeRegister(uint8_t addr, uint8_t val);
	bool readMultipleRegisters(uint8_t addr, uint8_t * dest, uint8_t len);
	bool writeMultipleRegisters(uint8_t addr, uint8_t * values, uint8_t len);

private:
	uint8_t _time[TIME_ARRAY_LENGTH];
	bool _isTwelveHour = true;
	TwoWire *_i2cPort;
};
