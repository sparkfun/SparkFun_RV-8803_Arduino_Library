/******************************************************************************
SparkFun_RV8803.h
RV8803 Arduino Library
Andy England @ SparkFun Electronics
March 3, 2020
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
#include <time.h>

//The 7-bit I2C address of the RV8803
#define RV8803_ADDR							0x32

#define SUNDAY 0x01
#define MONDAY 0x02
#define TUESDAY 0x04
#define WEDNESDAY 0x08
#define THURSDAY 0x10
#define FRIDAY 0x20
#define SATURDAY 0x40

//Register names:
#define RV8803_RAM							0x07
#define RV8803_HUNDREDTHS					0x10
#define RV8803_SECONDS						0x11
#define RV8803_MINUTES						0x12
#define RV8803_HOURS						0x13
#define RV8803_WEEKDAYS						0x14
#define RV8803_DATE							0x15
#define RV8803_MONTHS						0x16
#define RV8803_YEARS						0x17
#define RV8803_MINUTES_ALARM				0x18
#define RV8803_HOURS_ALARM					0x19
#define RV8803_WEEKDAYS_DATE_ALARM			0x1A
#define RV8803_TIMER_0						0x1B
#define RV8803_TIMER_1						0x1C
#define RV8803_EXTENSION					0x1D
#define RV8803_FLAG							0x1E
#define RV8803_CONTROL						0x1F
#define RV8803_HUNDREDTHS_CAPTURE			0x20
#define RV8803_SECONDS_CAPTURE				0x21
#define RV8803_OFFSET						0x2C
#define RV8803_EVENT_CONTROL				0x2F

//Enable Bits for Alarm Registers
#define ALARM_ENABLE						7

//Extension Register Bits
#define EXTENSION_TEST						7
#define EXTENSION_WADA						6
#define EXTENSION_USEL						5
#define EXTENSION_TE						4
#define EXTENSION_FD						2
#define EXTENSION_TD						0

//Flag Register Bits
#define FLAG_UPDATE							5
#define FLAG_TIMER							4
#define FLAG_ALARM							3
#define FLAG_EVI							2
#define FLAG_V2F							1
#define FLAG_V1F							0

//Interrupt Control Register Bits
#define UPDATE_INTERRUPT					5
#define TIMER_INTERRUPT						4
#define ALARM_INTERRUPT						3 //
#define EVI_INTERRUPT						2 //External Event Interrupt
#define CONTROL_RESET						0

//Event Control Bits
#define EVENT_ECP							7
#define EVENT_EHL							6
#define EVENT_ET							4
#define EVENT_ERST							0

//Possible Settings
#define TWELVE_HOUR_MODE					true
#define TWENTYFOUR_HOUR_MODE				false
#define COUNTDOWN_TIMER_FREQUENCY_4096_HZ	0b00
#define COUNTDOWN_TIMER_FREQUENCY_64_HZ		0b01
#define COUNTDOWN_TIMER_FREQUENCY_1_HZ		0b10
#define COUNTDOWN_TIMER_FREQUENCY_1_60TH_HZ	0b11
#define CLOCK_OUT_FREQUENCY_32768_HZ		0b00
#define CLOCK_OUT_FREQUENCY_1024_HZ			0b01
#define CLOCK_OUT_FREQUENCY_1_HZ			0b10

#define COUNTDOWN_TIMER_ON					true
#define COUNTDOWN_TIMER_OFF					false
#define TIME_UPDATE_1_SECOND				false
#define TIME_UPDATE_1_MINUTE				true

#define ENABLE_EVI_CALIBRATION				true
#define DISABLE_EVI_CALIBRATION				false
#define EVI_DEBOUNCE_NONE					0b00
#define EVI_DEBOUNCE_256HZ					0b01
#define EVI_DEBOUNCE_64HZ					0b10
#define EVI_DEBOUNCE_8HZ					0b11
#define RISING_EDGE							true
#define FALLING_EDGE						false
#define EVI_CAPTURE_ENABLE					true
#define EVI_CAPTURE_DISABLE					false

#define RV8803_ENABLE						true
#define RV8803_DISABLE						false

#define TIME_ARRAY_LENGTH 8 // Total number of writable values in device

enum time_order {
	TIME_HUNDREDTHS,	// 0
	TIME_SECONDS,		// 1
	TIME_MINUTES,		// 2
	TIME_HOURS,			// 3
	TIME_WEEKDAY,		// 4
	TIME_DATE,			// 5
	TIME_MONTH,			// 6
	TIME_YEAR,			// 7
};

class RV8803
{
public:
	
	RV8803( void );

	bool begin(TwoWire &wirePort = Wire);
	
	void set12Hour();
	void set24Hour();
	bool is12Hour(); //Returns true if 12hour bit is set
	bool isPM(); //Returns true if is12Hour and PM bit is set

	char* stringDateUSA(char *buffer, size_t len); //Return date in mm-dd-yyyy in supplied buffer
	char* stringDateUSA(); //Return date in mm-dd-yyyy
	char* stringDate(char *buffer, size_t len); //Return date in dd-mm-yyyy in supplied buffer
	char* stringDate(); //Return date in dd-mm-yyyy
	char* stringTime(char *buffer, size_t len);  //Return time hh:mm:ss with AM/PM if in 12 hour mode in supplied buffer
	char* stringTime(); //Return time hh:mm:ss with AM/PM if in 12 hour mode
	char* stringTimestamp(char *buffer, size_t len); //Return timestamp in hh:mm:ss:hh in supplied buffer, note that this must be read the same minute that the timestamp occurs or the minute will be wrong
	char* stringTimestamp(); //Return timestamp in hh:mm:ss:hh, note that this must be read the same minute that the timestamp occurs or the minute will be wrong
	char* stringTime8601(char *buffer, size_t len); //Return time in ISO 8601 format yyyy-mm-ddThh:mm:ss in supplied buffer
	char* stringTime8601(); //Return time in ISO 8601 format yyyy-mm-ddThh:mm:ss
	char* stringTime8601TZ(char *buffer, size_t len); //Return time in ISO 8601 format yyyy-mm-ddThh:mm:ss+/-hh:mm in supplied buffer
	char* stringTime8601TZ(); //Return time in ISO 8601 format yyyy-mm-ddThh:mm:ss+/-hh:mm
	char *stringDayOfWeek(char *buffer, size_t len); //Return the day of week in supplied buffer. Returns the full name
	char *stringDayOfWeek(); //Return the day of week. Returns the full name
	char *stringDayOfWeekShort(char *buffer, size_t len); //Return the day of week in supplied buffer. Returns "Sun", "Mon" etc
	char *stringDayOfWeekShort(); //Return the day of week. Returns "Sun", "Mon" etc
	char *stringDateOrdinal(char *buffer, size_t len); //Return the ordinal for the date (day of month). Returns "1st", "2nd", "3rd", "4th" etc
	char *stringDateOrdinal(); //Return the ordinal for the date (day of month). Returns "1st", "2nd", "3rd", "4th" etc
	char *stringMonth(char *buffer, size_t len); //Return the name of the month. Returns "January", etc
	char *stringMonth(); //Return the name of the month. Returns "January", etc
	char *stringMonthShort(char *buffer, size_t len); //Return the name of the month (short). Returns "Jan", "Feb" etc
	char *stringMonthShort(); //Return the name of the month (short). Returns "Jan", "Feb" etc
		
	bool setTime(uint8_t sec, uint8_t min, uint8_t hour, uint8_t weekday, uint8_t date, uint8_t month, uint16_t year);
	bool setTime(uint8_t * time, uint8_t len = TIME_ARRAY_LENGTH);
	bool setEpoch(uint32_t value, bool use1970sEpoch = false, int8_t timeZoneQuarterHours = 0); // If timeZoneQuarterHours is non-zero, update RV8803_RAM. Add the zone to the epoch before setting
	bool setLocalEpoch(uint32_t value, bool use1970sEpoch = false); // Set the local epoch - without adding the time zone
	bool setHundredthsToZero();
	bool setSeconds(uint8_t value);
	bool setMinutes(uint8_t value);
	bool setHours(uint8_t value);
	bool setDate(uint8_t value);
	bool setWeekday(uint8_t value);
	bool setMonth(uint8_t value);
	bool setYear(uint16_t value);
	void setTimeZoneQuarterHours(int8_t quarterHours); // Write the time zone to RV8803_RAM as int8_t (signed) in 15 minute increments
	int8_t getTimeZoneQuarterHours(void); // Read RV8803_RAM (int8_t (signed))

	bool updateTime(); //Update the local array with the RTC registers

	uint8_t getHundredths();
	uint8_t getSeconds();
	uint8_t getMinutes();
	uint8_t getHours();
	uint8_t getDate();
	uint8_t getWeekday();
	uint8_t getMonth();
	uint16_t getYear();	
	uint32_t getEpoch(bool use1970sEpoch = false); // Get the epoch - with the time zone subtracted (i.e. return UTC epoch)
	uint32_t getLocalEpoch(bool use1970sEpoch = false); // Get the local epoch - without subtracting the time zone
	
	uint8_t getHundredthsCapture();
	uint8_t getSecondsCapture();
	
	bool setToCompilerTime(); //Uses the hours, mins, etc from compile time to set RTC
	
	bool setCalibrationOffset(float ppm);
	float getCalibrationOffset();
	
	bool setEVICalibration(bool eviCalibration);
	bool setEVIDebounceTime(uint8_t debounceTime);
	bool setEVIEdgeDetection(bool edge);
	bool setEVIEventCapture(bool capture);
	
	bool getEVICalibration();
	uint8_t getEVIDebounceTime();
	bool getEVIEdgeDetection();
	bool getEVIEventCapture();
	
	bool setCountdownTimerEnable(bool timerState); //Starts and stops our countdown timer
	bool setCountdownTimerClockTicks(uint16_t clockTicks);
	bool setCountdownTimerFrequency(uint8_t countdownTimerFrequency);
	
	bool setClockOutTimerFrequency(uint8_t clockOutTimerFrequency);
	uint8_t getClockOutTimerFrequency();
		
	bool getCountdownTimerEnable();
	uint16_t getCountdownTimerClockTicks();
	uint8_t getCountdownTimerFrequency();
	
	bool setPeriodicTimeUpdateFrequency(bool timeUpdateFrequency);
	bool getPeriodicTimeUpdateFrequency();
	
	void setItemsToMatchForAlarm(bool minuteAlarm, bool hourAlarm, bool weekdayAlarm, bool dateAlarm); //0 to 7, alarm goes off with match of second, minute, hour, etc
	bool setAlarmMinutes(uint8_t minute);
	bool setAlarmHours(uint8_t hour);
	bool setAlarmWeekday(uint8_t weekday);
	bool setAlarmDate(uint8_t date);
	
	uint8_t getAlarmMinutes();
	uint8_t getAlarmHours();
	uint8_t getAlarmWeekday();
	uint8_t getAlarmDate();

	bool enableHardwareInterrupt(uint8_t source); //Enables a given interrupt within Interrupt Enable register
	bool disableHardwareInterrupt(uint8_t source); //Disables a given interrupt within Interrupt Enable register
	bool disableAllInterrupts();
	
	bool getInterruptFlag(uint8_t flagToGet);
	bool clearInterruptFlag(uint8_t flagToClear);
	bool clearAllInterruptFlags();
		
	//Values in RTC are stored in Binary Coded Decimal. These functions convert to/from Decimal
	uint8_t BCDtoDEC(uint8_t val); 
	uint8_t DECtoBCD(uint8_t val);

	bool readBit(uint8_t regAddr, uint8_t bitAddr);
	uint8_t readTwoBits(uint8_t regAddr, uint8_t bitAddr);
	bool writeBit(uint8_t regAddr, uint8_t bitAddr, bool bitToWrite);
	bool writeBit(uint8_t regAddr, uint8_t bitAddr, uint8_t bitToWrite);
	uint8_t readRegister(uint8_t addr);
	bool writeRegister(uint8_t addr, uint8_t val);
	bool readMultipleRegisters(uint8_t addr, uint8_t * dest, uint8_t len);
	bool writeMultipleRegisters(uint8_t addr, uint8_t * values, uint8_t len);

	// When converting from a UTC based struct tm to a time_t value, you would normally use a utc
	// version of mktime - timegm(), but we don't have that on most micro controllers - so use 
	// the following. 
	static time_t sub_mkgmt(struct tm *tm, bool use1970sEpoch);
	time_t _timegm(struct tm *tm, bool use1970sEpoch);

  private:
	uint8_t _time[TIME_ARRAY_LENGTH];
	bool _isTwelveHour = true;
	TwoWire *_i2cPort;
};
