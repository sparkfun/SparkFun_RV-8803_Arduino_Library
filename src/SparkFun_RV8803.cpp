 /******************************************************************************
SparkFun_RV8803.h
RV8803 Arduino Library
Andy England @ SparkFun Electronics
March 3, 2020
https://github.com/sparkfun/SparkFun_RV-8803_Arduino_Library

Development environment specifics:
Arduino IDE 1.6.4

This code is released under the [MIT License](http://opensource.org/licenses/MIT).
Please review the LICENSE.md file included with this example. If you have any questions
or concerns with licensing, please contact techsupport@sparkfun.com.
Distributed as-is; no warranty is given.
******************************************************************************/

#include <time.h>
#include "SparkFun_RV8803.h"

//****************************************************************************//
//
//  Settings and configuration
//
//****************************************************************************//

// Parse the __DATE__ predefined macro to generate date defaults:
// __Date__ Format: MMM DD YYYY (First D may be a space if <10)
// <MONTH>
#define BUILD_MONTH_JAN ((__DATE__[0] == 'J') && (__DATE__[1] == 'a')) ? 1 : 0
#define BUILD_MONTH_FEB (__DATE__[0] == 'F') ? 2 : 0
#define BUILD_MONTH_MAR ((__DATE__[0] == 'M') && (__DATE__[1] == 'a') && (__DATE__[2] == 'r')) ? 3 : 0
#define BUILD_MONTH_APR ((__DATE__[0] == 'A') && (__DATE__[1] == 'p')) ? 4 : 0
#define BUILD_MONTH_MAY ((__DATE__[0] == 'M') && (__DATE__[1] == 'a') && (__DATE__[2] == 'y')) ? 5 : 0
#define BUILD_MONTH_JUN ((__DATE__[0] == 'J') && (__DATE__[1] == 'u') && (__DATE__[2] == 'n')) ? 6 : 0
#define BUILD_MONTH_JUL ((__DATE__[0] == 'J') && (__DATE__[1] == 'u') && (__DATE__[2] == 'l')) ? 7 : 0
#define BUILD_MONTH_AUG ((__DATE__[0] == 'A') && (__DATE__[1] == 'u')) ? 8 : 0
#define BUILD_MONTH_SEP (__DATE__[0] == 'S') ? 9 : 0
#define BUILD_MONTH_OCT (__DATE__[0] == 'O') ? 10 : 0
#define BUILD_MONTH_NOV (__DATE__[0] == 'N') ? 11 : 0
#define BUILD_MONTH_DEC (__DATE__[0] == 'D') ? 12 : 0
#define BUILD_MONTH BUILD_MONTH_JAN | BUILD_MONTH_FEB | BUILD_MONTH_MAR | \
BUILD_MONTH_APR | BUILD_MONTH_MAY | BUILD_MONTH_JUN | \
BUILD_MONTH_JUL | BUILD_MONTH_AUG | BUILD_MONTH_SEP | \
BUILD_MONTH_OCT | BUILD_MONTH_NOV | BUILD_MONTH_DEC
// <DATE>
#define BUILD_DATE_0 ((__DATE__[4] == ' ') ? 0 : (__DATE__[4] - 0x30))
#define BUILD_DATE_1 (__DATE__[5] - 0x30)
#define BUILD_DATE ((BUILD_DATE_0 * 10) + BUILD_DATE_1)
// <YEAR>
#define BUILD_YEAR (((__DATE__[7] - 0x30) * 1000) + ((__DATE__[8] - 0x30) * 100) + \
((__DATE__[9] - 0x30) * 10)  + ((__DATE__[10] - 0x30) * 1))

// Parse the __TIME__ predefined macro to generate time defaults:
// __TIME__ Format: HH:MM:SS (First number of each is padded by 0 if <10)
// <HOUR>
#define BUILD_HOUR_0 ((__TIME__[0] == ' ') ? 0 : (__TIME__[0] - 0x30))
#define BUILD_HOUR_1 (__TIME__[1] - 0x30)
#define BUILD_HOUR ((BUILD_HOUR_0 * 10) + BUILD_HOUR_1)
// <MINUTE>
#define BUILD_MINUTE_0 ((__TIME__[3] == ' ') ? 0 : (__TIME__[3] - 0x30))
#define BUILD_MINUTE_1 (__TIME__[4] - 0x30)
#define BUILD_MINUTE ((BUILD_MINUTE_0 * 10) + BUILD_MINUTE_1)
// <SECOND>
#define BUILD_SECOND_0 ((__TIME__[6] == ' ') ? 0 : (__TIME__[6] - 0x30))
#define BUILD_SECOND_1 (__TIME__[7] - 0x30)
#define BUILD_SECOND ((BUILD_SECOND_0 * 10) + BUILD_SECOND_1)

RV8803::RV8803( void )
{

}

bool RV8803::begin(TwoWire &wirePort)
{
	_i2cPort = &wirePort;
	
	_i2cPort->beginTransmission(RV8803_ADDR);
	
	if (_i2cPort->endTransmission() != 0)
	{
		return (false); //Error: Sensor did not ack
	}
	return(true);
}

//Configures the microcontroller to convert to 12 hour mode.
void RV8803::set12Hour()
{
	_isTwelveHour = TWELVE_HOUR_MODE;
}

//Configures the microcontroller to not convert from the default 24 hour mode.
void RV8803::set24Hour()
{
	_isTwelveHour = TWENTYFOUR_HOUR_MODE;
}

//Returns true if the microcontroller has been configured for 12 hour mode
bool RV8803::is12Hour()
{
	return _isTwelveHour;
}

//Returns true if the microcontroller is in 12 hour mode and the RTC has an hours value greater than or equal to 12 (Noon).
bool RV8803::isPM()
{
	if (is12Hour())
	{
		return BCDtoDEC(_time[TIME_HOURS]) >= 12;
	}
	else
	{
		return false;
	}
}

//Returns the date in MM/DD/YYYY format.
char* RV8803::stringDateUSA(char *buffer, size_t len)
{
	snprintf(buffer, len, "%02d/%02d/20%02d", BCDtoDEC(_time[TIME_MONTH]), BCDtoDEC(_time[TIME_DATE]), BCDtoDEC(_time[TIME_YEAR]));
	return(buffer);
}

//Returns the date in MM/DD/YYYY format.
char* RV8803::stringDateUSA()
{
	static char date[11]; //Max of mm/dd/yyyy with \0 terminator
	return stringDateUSA(date, sizeof(date));
}

//Returns the date in the DD/MM/YYYY format.
char*  RV8803::stringDate(char *buffer, size_t len)
{
	snprintf(buffer, len, "%02d/%02d/20%02d", BCDtoDEC(_time[TIME_DATE]), BCDtoDEC(_time[TIME_MONTH]), BCDtoDEC(_time[TIME_YEAR]));
	return(buffer);
}

//Returns the date in the DD/MM/YYYY format.
char*  RV8803::stringDate()
{
	static char date[11]; //Max of dd/mm/yyyy with \0 terminator
	return stringDate(date, sizeof(date));
}

//Returns the time in hh:mm:ss (Adds AM/PM if in 12 hour mode).
char* RV8803::stringTime(char *buffer, size_t len)
{
	if(is12Hour() == true)
	{
		char half = 'A';
		uint8_t twelveHourCorrection = 0;
		if(isPM())
		{
			half = 'P';
			if (BCDtoDEC(_time[TIME_HOURS]) > 12)
			{
				twelveHourCorrection = 12;
			}
		}
		snprintf(buffer, len, "%02d:%02d:%02d%cM", BCDtoDEC(_time[TIME_HOURS]) - twelveHourCorrection, BCDtoDEC(_time[TIME_MINUTES]), BCDtoDEC(_time[TIME_SECONDS]), half);
	}
	else
	snprintf(buffer, len, "%02d:%02d:%02d", BCDtoDEC(_time[TIME_HOURS]), BCDtoDEC(_time[TIME_MINUTES]), BCDtoDEC(_time[TIME_SECONDS]));
	
	return(buffer);
}

//Returns the time in hh:mm:ss (Adds AM/PM if in 12 hour mode).
char* RV8803::stringTime()
{
	static char time[11];
	return stringTime(time, sizeof(time));
}

//Returns the most recent timestamp captured on the EVI pin (if the EVI pin has been configured to capture events)
char* RV8803::stringTimestamp(char *buffer, size_t len)
{
	if(is12Hour() == true)
	{
		char half = 'A';
		uint8_t twelveHourCorrection = 0;
		if(isPM())
		{
			half = 'P';
			if (BCDtoDEC(_time[TIME_HOURS]) > 12)
			{
				twelveHourCorrection = 12;
			}
		}
		snprintf(buffer, len, "%02d:%02d:%02d:%02d%cM", BCDtoDEC(_time[TIME_HOURS]) - twelveHourCorrection, BCDtoDEC(_time[TIME_MINUTES]),  BCDtoDEC(readRegister(RV8803_SECONDS_CAPTURE)), BCDtoDEC(readRegister(RV8803_HUNDREDTHS_CAPTURE)), half);
	}
	else
	snprintf(buffer, len, "%02d:%02d:%02d:%02d", BCDtoDEC(_time[TIME_HOURS]), BCDtoDEC(_time[TIME_MINUTES]), BCDtoDEC(readRegister(RV8803_SECONDS_CAPTURE)), BCDtoDEC(readRegister(RV8803_HUNDREDTHS_CAPTURE)));
	
	return(buffer);
}

char* RV8803::stringTimestamp()
{
	static char time[14]; //Max of hh:mm:ss:HHXM with \0 terminator
	return stringTimestamp(time, sizeof(time));
}

//Returns timestamp in ISO 8601 format (yyyy-mm-ddThh:mm:ss).
char* RV8803::stringTime8601(char *buffer, size_t len)
{
	snprintf(buffer, len, "20%02d-%02d-%02dT%02d:%02d:%02d", BCDtoDEC(_time[TIME_YEAR]), BCDtoDEC(_time[TIME_MONTH]), BCDtoDEC(_time[TIME_DATE]), BCDtoDEC(_time[TIME_HOURS]), BCDtoDEC(_time[TIME_MINUTES]), BCDtoDEC(_time[TIME_SECONDS]));
	return(buffer);
}

char* RV8803::stringTime8601()
{
	static char timeStamp[21]; //Max of yyyy-mm-ddThh:mm:ss with \0 terminator
	return stringTime8601(timeStamp, sizeof(timeStamp));
}

//Returns time in UNIX Epoch time format
uint32_t RV8803::getEpoch(bool use1970sEpoch)
{
	struct tm tm;

	tm.tm_isdst = -1;
	tm.tm_yday = 0;
	tm.tm_wday = 0;
	tm.tm_year = BCDtoDEC(_time[TIME_YEAR]) + 100;
	tm.tm_mon = BCDtoDEC(_time[TIME_MONTH]) - 1;
	tm.tm_mday = BCDtoDEC(_time[TIME_DATE]);
	tm.tm_hour = BCDtoDEC(_time[TIME_HOURS]);
	tm.tm_min = BCDtoDEC(_time[TIME_MINUTES]);
	tm.tm_sec = BCDtoDEC(_time[TIME_SECONDS]);

	time_t t = mktime(&tm);

	if(use1970sEpoch)
	{
	    // AVR GCC compiler sets the Epoch time to Jan 1st, 2000. We can 
    	// increase the offset to Jan 1st, 1970 if folks want that format
		t += 946684800;
	}

	return t;
}

//Sets time using UNIX Epoch time
bool RV8803::setEpoch(uint32_t value, bool use1970sEpoch)
{
	if(use1970sEpoch)
	{
	    // AVR GCC compiler sets the Epoch time to Jan 1st, 2000. We can 
    	// reduce the offset from Jan 1st, 1970 if folks want that format
		value -= 946684800;
	}

	time_t t = value;
	struct tm* tmp = gmtime(&t);

	_time[TIME_SECONDS] = DECtoBCD(tmp->tm_sec);
	_time[TIME_MINUTES] = DECtoBCD(tmp->tm_min);
	_time[TIME_HOURS] = DECtoBCD(tmp->tm_hour);
	_time[TIME_DATE] = DECtoBCD(tmp->tm_mday);
	_time[TIME_WEEKDAY] = 1 << tmp->tm_wday;
	_time[TIME_MONTH] = DECtoBCD(tmp->tm_mon + 1);
	_time[TIME_YEAR] = DECtoBCD(tmp->tm_year - 100);
		
	return setTime(_time, TIME_ARRAY_LENGTH); //Subtract one as we don't write to the hundredths register
}

//Set time and date/day registers of RV8803
bool RV8803::setTime(uint8_t sec, uint8_t min, uint8_t hour, uint8_t weekday, uint8_t date, uint8_t month, uint16_t year)
{
	_time[TIME_SECONDS] = DECtoBCD(sec);
	_time[TIME_MINUTES] = DECtoBCD(min);
	_time[TIME_HOURS] = DECtoBCD(hour);
	_time[TIME_DATE] = DECtoBCD(date);
	_time[TIME_WEEKDAY] = 1 << weekday;
	_time[TIME_MONTH] = DECtoBCD(month);
	_time[TIME_YEAR] = DECtoBCD(year - 2000);
		
	return setTime(_time, TIME_ARRAY_LENGTH); //Subtract one as we don't write to the hundredths register
}

//Set time and date/day registers of RV8803 (using data array)
bool RV8803::setTime(uint8_t * time, uint8_t len = 8)
{
	if (len != TIME_ARRAY_LENGTH)
		return false;
	
	return writeMultipleRegisters(RV8803_SECONDS, time + 1, len - 1); //We use length - 1 as that is the length without the read-only hundredths register We also point to the second element in the time array as hundredths is read only
}

bool RV8803::setHundredthsToZero()
{
	bool temp = writeBit(RV8803_CONTROL, CONTROL_RESET, RV8803_ENABLE);
	temp &= writeBit(RV8803_CONTROL, CONTROL_RESET, RV8803_DISABLE);
	return temp;
}

bool RV8803::setSeconds(uint8_t value)
{
	_time[TIME_SECONDS] = DECtoBCD(value);
	return setTime(_time, TIME_ARRAY_LENGTH);
}

bool RV8803::setMinutes(uint8_t value)
{
	_time[TIME_MINUTES] = DECtoBCD(value);
	return setTime(_time, TIME_ARRAY_LENGTH);
}

bool RV8803::setHours(uint8_t value)
{
	_time[TIME_HOURS] = DECtoBCD(value);
	return setTime(_time, TIME_ARRAY_LENGTH);
}

bool RV8803::setDate(uint8_t value)
{
	_time[TIME_DATE] = DECtoBCD(value);
	return setTime(_time, TIME_ARRAY_LENGTH);
}

bool RV8803::setMonth(uint8_t value)
{
	_time[TIME_MONTH] = DECtoBCD(value);
	return setTime(_time, TIME_ARRAY_LENGTH);
}

bool RV8803::setYear(uint16_t value)
{
	_time[TIME_YEAR] = DECtoBCD(value - 2000);
	return setTime(_time, TIME_ARRAY_LENGTH);
}


bool RV8803::setWeekday(uint8_t value) //value is anywhere between 0=sunday and 6=saturday
{
	if (value > 6)
	{
		value = 6;
	}
	_time[TIME_WEEKDAY] = 1 << value;
	return setTime(_time, TIME_ARRAY_LENGTH);
}

//Move the hours, mins, sec, etc registers from RV-8803 into the _time array
//Needs to be called before printing time or date
//We do not protect the GPx registers. They will be overwritten. The user has plenty of RAM if they need it.
bool RV8803::updateTime()
{
	if (readMultipleRegisters(RV8803_HUNDREDTHS, _time, TIME_ARRAY_LENGTH) == false)
		return(false); //Something went wrong
	
	if (BCDtoDEC(_time[TIME_SECONDS]) == 59) //If seconds are at 59, read again to make sure we didn't accidentally skip a minute
	{	
		uint8_t tempTime[TIME_ARRAY_LENGTH];
		if (readMultipleRegisters(RV8803_HUNDREDTHS, tempTime, TIME_ARRAY_LENGTH) == false)
		{
			return(false); //Something went wrong
		}
		if (BCDtoDEC(tempTime[TIME_SECONDS]) == 0) //If the reading for seconds changed, then our new data is correct, otherwise, we can leave the old data.
		{
			memcpy(_time, tempTime, TIME_ARRAY_LENGTH);
		}
	}
	return true;
}

uint8_t RV8803::getHundredths()
{
	return BCDtoDEC(_time[TIME_HUNDREDTHS]);
}

uint8_t RV8803::getSeconds()
{
	return BCDtoDEC(_time[TIME_SECONDS]);
}

uint8_t RV8803::getMinutes()
{
	return BCDtoDEC(_time[TIME_MINUTES]);
}

uint8_t RV8803::getHours()
{
	uint8_t tempHours = BCDtoDEC(_time[TIME_HOURS]);
	if (is12Hour())
	{
		if (tempHours > 12)
		{
			tempHours -= 12;
		}
	}
	return tempHours;
}

uint8_t RV8803::getDate()
{
	return BCDtoDEC(_time[TIME_DATE]);
}

uint8_t RV8803::getWeekday()
{
	uint8_t tempWeekday = _time[TIME_WEEKDAY];
	tempWeekday = log(tempWeekday) / log(2);
	return tempWeekday;
}

uint8_t RV8803::getMonth()
{
	return BCDtoDEC(_time[TIME_MONTH]);
}

uint16_t RV8803::getYear()
{
	return BCDtoDEC(_time[TIME_YEAR]) + 2000;
}

uint8_t RV8803::getHundredthsCapture()
{
	return BCDtoDEC(readRegister(RV8803_HUNDREDTHS_CAPTURE));
}

uint8_t RV8803::getSecondsCapture()
{
	return BCDtoDEC(readRegister(RV8803_SECONDS_CAPTURE));
}

//Takes the time from the last build and uses it as the current time
//Works very well as an arduino sketch
bool RV8803::setToCompilerTime()
{
	_time[TIME_SECONDS] = DECtoBCD(BUILD_SECOND);
	_time[TIME_MINUTES] = DECtoBCD(BUILD_MINUTE);
	_time[TIME_HOURS] = DECtoBCD(BUILD_HOUR);
	
	_time[TIME_MONTH] = DECtoBCD(BUILD_MONTH);
	_time[TIME_DATE] = DECtoBCD(BUILD_DATE);
	_time[TIME_YEAR] = DECtoBCD(BUILD_YEAR - 2000); //! Not Y2K (or Y2.1K)-proof :(
	
	// Calculate weekday (from here: http://stackoverflow.com/a/21235587)
	// 0 = Sunday, 6 = Saturday
	uint16_t d = BUILD_DATE;
	uint16_t m = BUILD_MONTH;
	uint16_t y = BUILD_YEAR;
	uint16_t weekday = (d+=m<3?y--:y-2,23*m/9+d+4+y/4-y/100+y/400)%7 + 1;
	_time[TIME_WEEKDAY] = 1 << weekday;
	
	return setTime(_time, TIME_ARRAY_LENGTH);
}

bool RV8803::setCalibrationOffset(float ppm)
{
	int8_t integerOffset = ppm / 0.2384; //.2384 is ppm/LSB
	if (integerOffset < 0)
	{
		integerOffset += 64;
	}
	return writeRegister(RV8803_OFFSET, integerOffset);
}

float RV8803:: getCalibrationOffset()
{
	int8_t value = readRegister(RV8803_OFFSET);
	if (value > 32)
	{
		value -= 64;
	}
	return value * .2384;
}

bool RV8803::setEVIDebounceTime(uint8_t debounceTime)
{
	return writeBit(RV8803_EVENT_CONTROL, EVENT_ET, debounceTime);
}

bool RV8803::setEVICalibration(bool eviCalibration)
{
	return writeBit(RV8803_EVENT_CONTROL, EVENT_ERST, eviCalibration);
}

bool RV8803::setEVIEdgeDetection(bool edge)
{
	return writeBit(RV8803_EVENT_CONTROL, EVENT_EHL, edge);
}

bool RV8803::setEVIEventCapture(bool capture)
{
	return writeBit(RV8803_EVENT_CONTROL, EVENT_ECP, capture);
}

uint8_t RV8803::getEVIDebounceTime()
{
	return readTwoBits(RV8803_EVENT_CONTROL, EVENT_ET);
}

bool RV8803::getEVICalibration()
{
	return readBit(RV8803_EVENT_CONTROL, EVENT_ERST);
}

bool RV8803::getEVIEdgeDetection()
{
	return readBit(RV8803_EVENT_CONTROL, EVENT_EHL);
}

bool RV8803::getEVIEventCapture()
{
	return readBit(RV8803_EVENT_CONTROL, EVENT_ECP);
}

bool RV8803::setCountdownTimerEnable(bool timerState)
{
	return writeBit(RV8803_EXTENSION, EXTENSION_TE, timerState);
}

bool RV8803::setCountdownTimerFrequency(uint8_t countdownTimerFrequency)
{
	return writeBit(RV8803_EXTENSION, EXTENSION_TD, countdownTimerFrequency);
}

bool RV8803::setCountdownTimerClockTicks(uint16_t clockTicks)
{
	//First handle the upper bit, as we need to preserve the GPX bits
	uint8_t value = readRegister(RV8803_TIMER_1);
	value &= ~(0b00001111); //Clear the least significant nibble
	value |= (clockTicks >> 8);
	bool returnValue = writeRegister(RV8803_TIMER_1, value);
	value = clockTicks & 0x00FF;
	returnValue &= writeRegister(RV8803_TIMER_0, value);
	return returnValue;
}

bool RV8803::setClockOutTimerFrequency(uint8_t clockOutTimerFrequency)
{
	return writeBit(RV8803_EXTENSION, EXTENSION_FD, clockOutTimerFrequency);
}

bool RV8803::getCountdownTimerEnable()
{
	return readBit(RV8803_EXTENSION, EXTENSION_TE);
}

uint8_t RV8803::getCountdownTimerFrequency()
{
	return readTwoBits(RV8803_EXTENSION, EXTENSION_TD);
}

uint16_t RV8803::getCountdownTimerClockTicks()
{
	uint16_t value = readRegister(RV8803_TIMER_1) << 8;
	value |= readRegister(RV8803_TIMER_0);
	return value;
}

uint8_t RV8803::getClockOutTimerFrequency()
{
	return readTwoBits(RV8803_EXTENSION, EXTENSION_FD);
}

bool RV8803::setPeriodicTimeUpdateFrequency(bool timeUpdateFrequency)
{
		return writeBit(RV8803_EXTENSION, EXTENSION_USEL, timeUpdateFrequency);
}

bool RV8803::getPeriodicTimeUpdateFrequency()
{	
	return readBit(RV8803_EXTENSION, EXTENSION_USEL);
}

/********************************
Set Alarm Mode controls which parts of the time have to match for the alarm to trigger.
When the RTC matches a given time, make an interrupt fire.
Setting a bit to 1 means that the RTC does not check if that value matches to trigger the alarm
********************************/
void RV8803::setItemsToMatchForAlarm(bool minuteAlarm, bool hourAlarm, bool weekdayAlarm, bool dateAlarm)
{
	writeBit(RV8803_MINUTES_ALARM, ALARM_ENABLE, !minuteAlarm); //For some reason these bits are active low
	writeBit(RV8803_HOURS_ALARM, ALARM_ENABLE, !hourAlarm);
	writeBit(RV8803_WEEKDAYS_DATE_ALARM, ALARM_ENABLE, !weekdayAlarm);
	writeBit(RV8803_EXTENSION, EXTENSION_WADA, dateAlarm);
	if (dateAlarm == true)//enabling both weekday and date alarm will default to a date alarm
	{
		writeBit(RV8803_WEEKDAYS_DATE_ALARM, ALARM_ENABLE, !dateAlarm);
	}
}

bool RV8803::setAlarmMinutes(uint8_t minute)
{
	uint8_t value = readRegister(RV8803_MINUTES_ALARM);
	value &= (1 << ALARM_ENABLE); //clear everything but enable bit
	value |= DECtoBCD(minute);
	return writeRegister(RV8803_MINUTES_ALARM, value);
}

bool RV8803::setAlarmHours(uint8_t hour)
{
	uint8_t value = readRegister(RV8803_HOURS_ALARM);
	value &= (1 << ALARM_ENABLE); //clear everything but enable bit
	value |= DECtoBCD(hour);
	return writeRegister(RV8803_HOURS_ALARM, value);
}

bool RV8803::setAlarmWeekday(uint8_t weekday)
{
	uint8_t value = readRegister(RV8803_WEEKDAYS_DATE_ALARM);
	value &= (1 << ALARM_ENABLE); //clear everything but enable bit
	value |= 0x7F & weekday;
	return writeRegister(RV8803_WEEKDAYS_DATE_ALARM, value);

}

bool RV8803::setAlarmDate(uint8_t date)
{
	uint8_t value = readRegister(RV8803_WEEKDAYS_DATE_ALARM);
	value &= (1 << ALARM_ENABLE); //clear everything but enable bit
	value |= DECtoBCD(date);
	return writeRegister(RV8803_WEEKDAYS_DATE_ALARM, value);
}


uint8_t RV8803::getAlarmMinutes()
{
	return BCDtoDEC(readRegister(RV8803_MINUTES_ALARM));
}


uint8_t RV8803::getAlarmHours()
{
	return BCDtoDEC(readRegister(RV8803_HOURS_ALARM));
}


uint8_t RV8803::getAlarmWeekday()
{
	return BCDtoDEC(readRegister(RV8803_WEEKDAYS_DATE_ALARM));
}

uint8_t RV8803::getAlarmDate()
{
	return BCDtoDEC(readRegister(RV8803_WEEKDAYS_DATE_ALARM));
}

/*********************************
Given a bit location, enable the interrupt
INTERRUPT_BLIE	4
INTERRUPT_TIE	3
INTERRUPT_AIE	2
INTERRUPT_EIE	1
*********************************/
bool RV8803::enableHardwareInterrupt(uint8_t source)
{
	uint8_t value = readRegister(RV8803_CONTROL);
	value |= (1<<source); //Set the interrupt enable bit
	return writeRegister(RV8803_CONTROL, value);
}

bool RV8803::disableHardwareInterrupt(uint8_t source)
{
	uint8_t value = readRegister(RV8803_CONTROL);
	value &= ~(1 << source); //Clear the interrupt enable bit
	return writeRegister(RV8803_CONTROL, value);
}

bool RV8803::disableAllInterrupts()
{
	uint8_t value = readRegister(RV8803_CONTROL);
	value &= 1; //Clear all bits except for Reset
	return writeRegister(RV8803_CONTROL, value);
}

bool RV8803::getInterruptFlag(uint8_t flagToGet)
{
	uint8_t flag = readRegister(RV8803_FLAG);
	flag &= (1 << flagToGet);
	flag = flag >> flagToGet;
	return flag;
}

bool RV8803::clearAllInterruptFlags() //Read the status register to clear the current interrupt flags
{
	return writeRegister(RV8803_FLAG, 0b00000000);//Write all 0's to clear all flags
}

bool RV8803::clearInterruptFlag(uint8_t flagToClear)
{
	uint8_t value = readRegister(RV8803_FLAG);
	value &= ~(1 << flagToClear); //clear flag
	return writeRegister(RV8803_FLAG, value);
}

uint8_t RV8803::BCDtoDEC(uint8_t val)
{
	return ( ( val / 0x10) * 10 ) + ( val % 0x10 );
}

// BCDtoDEC -- convert decimal to binary-coded decimal (BCD)
uint8_t RV8803::DECtoBCD(uint8_t val)
{
	return ( ( val / 10 ) * 0x10 ) + ( val % 10 );
}

bool RV8803::readBit(uint8_t regAddr, uint8_t bitAddr)
{
	return ((readRegister(regAddr) & (1 << bitAddr)) >> bitAddr);
}

uint8_t RV8803::readTwoBits(uint8_t regAddr, uint8_t bitAddr)
{
	return ((readRegister(regAddr) & (3 << bitAddr)) >> bitAddr);
}

bool RV8803::writeBit(uint8_t regAddr, uint8_t bitAddr, bool bitToWrite)
{
	uint8_t value = readRegister(regAddr);
	value &= ~(1 << bitAddr);
	value |= bitToWrite << bitAddr;
	return writeRegister(regAddr, value);
}

bool RV8803::writeBit(uint8_t regAddr, uint8_t bitAddr, uint8_t bitToWrite) //If we see an unsigned 8-bit, we know we have to write two bits.
{
	uint8_t value = readRegister(regAddr);
	value &= ~(3 << bitAddr);
	value |= bitToWrite << bitAddr;
	return writeRegister(regAddr, value);
}

uint8_t RV8803::readRegister(uint8_t addr)
{
	_i2cPort->beginTransmission(RV8803_ADDR);
	_i2cPort->write(addr);
	_i2cPort->endTransmission();

	//typecasting the 1 parameter in requestFrom so that the compiler
	//doesn't give us a warning about multiple candidates
	if (_i2cPort->requestFrom(static_cast<uint8_t>(RV8803_ADDR), static_cast<uint8_t>(1)) != 0)
	{
		return _i2cPort->read();
	}
	return false;
}

bool RV8803::writeRegister(uint8_t addr, uint8_t val)
{
	_i2cPort->beginTransmission(RV8803_ADDR);
	_i2cPort->write(addr);
	_i2cPort->write(val);
	if (_i2cPort->endTransmission() != 0)
		return (false); //Error: Sensor did not ack
	return(true);
}

bool RV8803::writeMultipleRegisters(uint8_t addr, uint8_t * values, uint8_t len)
{
	_i2cPort->beginTransmission(RV8803_ADDR);
	_i2cPort->write(addr);
	for (uint8_t i = 0; i < len; i++)
	{
		_i2cPort->write(values[i]);
	}

	if (_i2cPort->endTransmission() != 0)
		return (false); //Error: Sensor did not ack
	return(true);
}

bool RV8803::readMultipleRegisters(uint8_t addr, uint8_t * dest, uint8_t len)
{
	_i2cPort->beginTransmission(RV8803_ADDR);
	_i2cPort->write(addr);
	if (_i2cPort->endTransmission() != 0)
		return (false); //Error: Sensor did not ack

	_i2cPort->requestFrom(static_cast<uint8_t>(RV8803_ADDR), len);
	for (uint8_t i = 0; i < len; i++)
	{
		dest[i] = _i2cPort->read();
	}
	
	return(true);
}
