/******************************************************************************
SparkFun_RV8803.h
RV8803 Arduino Library
Andy England @ SparkFun Electronics
February 5, 2018
https://github.com/sparkfun/Qwiic_RTC

Development environment specifics:
Arduino IDE 1.6.4

This code is released under the [MIT License](http://opensource.org/licenses/MIT).
Please review the LICENSE.md file included with this example. If you have any questions
or concerns with licensing, please contact techsupport@sparkfun.com.
Distributed as-is; no warranty is given.
******************************************************************************/

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

boolean RV8803::begin(TwoWire &wirePort)
{
	_i2cPort = &wirePort;

	return(true);
}

//12/24 Hour mode are configurable in the library, not the RTC itself
void RV8803::set12Hour()
{
	_isTwelveHour = 12_HOUR_MODE;
}

//Configure RTC to output 0-23 hours
//Converts any current hour setting to 24 hour
void RV8803::set24Hour()
{
	_isTwelveHour = 24_HOUR_MODE;
}

//Returns true if RTC has been configured for 12 hour mode
bool RV8803::is12Hour()
{
	return _isTwelveHour;
}

//Returns true if RTC has PM bit set and 12Hour bit set
bool RV8803::isPM()
{
	return BCDtoDEC(_time[TIME_HOURS]) >= 12;
}

//Returns a pointer to array of chars that are the date in mm/dd/yyyy format because we're weird
char* RV8803::stringDateUSA()
{
	static char date[11]; //Max of mm/dd/yyyy with \0 terminator
	sprintf(date, "%02d/%02d/20%02d", BCDtoDEC(_time[TIME_MONTH]), BCDtoDEC(_time[TIME_DATE]), BCDtoDEC(_time[TIME_YEAR]));
	return(date);
}

//Returns a pointer to array of chars that are the date in dd/mm/yyyy format
char*  RV8803::stringDate()
{
	static char date[11]; //Max of dd/mm/yyyy with \0 terminator
	sprintf(date, "%02d/%02d/20%02d", BCDtoDEC(_time[TIME_DATE]), BCDtoDEC(_time[TIME_MONTH]), BCDtoDEC(_time[TIME_YEAR]));
	return(date);
}

//Returns a pointer to array of chars that represents the time in hh:mm:ss format
//Adds AM/PM if in 12 hour mode
char* RV8803::stringTime()
{
	static char time[11]; //Max of hh:mm:ssXM with \0 terminator

	if(is12Hour() == true)
	{
		char half = 'A';
		if(isPM()) half = 'P';
		
		sprintf(time, "%02d:%02d:%02d%cM", BCDtoDEC(_time[TIME_HOURS]), BCDtoDEC(_time[TIME_MINUTES]), BCDtoDEC(_time[TIME_SECONDS]), half);
	}
	else
	sprintf(time, "%02d:%02d:%02d", BCDtoDEC(_time[TIME_HOURS]), BCDtoDEC(_time[TIME_MINUTES]), BCDtoDEC(_time[TIME_SECONDS]));
	
	return(time);
}

char* RV8803::stringTimeStamp()
{
	static char timeStamp[21]; //Max of yyyy-mm-ddThh:mm:ss with \0 terminator

	sprintf(timeStamp, "20%02d-%02d-%02dT%02d:%02d:%02d", BCDtoDEC(_time[TIME_YEAR]), BCDtoDEC(_time[TIME_MONTH]), BCDtoDEC(_time[TIME_DATE]), BCDtoDEC(_time[TIME_HOURS]), BCDtoDEC(_time[TIME_MINUTES]), BCDtoDEC(_time[TIME_SECONDS]));
	
	return(timeStamp);
}

bool RV8803::setTime(uint8_t sec, uint8_t min, uint8_t hour, uint8_t date, uint8_t month, uint8_t year, uint8_t day)
{
	_time[TIME_SECONDS] = DECtoBCD(sec);
	_time[TIME_MINUTES] = DECtoBCD(min);
	_time[TIME_HOURS] = DECtoBCD(hour);
	_time[TIME_DATE] = DECtoBCD(date);
	_time[TIME_DAY] = DECtoBCD(day);
	_time[TIME_MONTH] = DECtoBCD(month);
	_time[TIME_YEAR] = DECtoBCD(year);
		
	return setTime(_time, TIME_ARRAY_LENGTH);
}

// setTime -- Set time and date/day registers of RV8803 (using data array)
bool RV8803::setTime(uint8_t * time, uint8_t len)
{
	if (len != TIME_ARRAY_LENGTH)
		return false;
	
	return writeMultipleRegisters(RV8803_SECONDS, time, len - 1); //We use length - 1 as that is the length without the read-only hundredths register
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

bool RV8803::setYear(uint8_t value)
{
	_time[TIME_YEAR] = DECtoBCD(value);
	return setTime(_time, TIME_ARRAY_LENGTH);
}


bool RV8803::setWeekday(uint8_t value)
{
	_time[TIME_DAY] = DECtoBCD(value);
	return setTime(_time, TIME_ARRAY_LENGTH);
}

//Move the hours, mins, sec, etc registers from RV-8803 into the _time array
//Needs to be called before printing time or date
//We do not protect the GPx registers. They will be overwritten. The user has plenty of RAM if they need it.
bool RV8803::updateTime()
{
	if (readMultipleRegisters(RV8803_HUNDREDTHS, _time, TIME_ARRAY_LENGTH) == false)
		return(false); //Something went wrong
	
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
	uint8_t tempHours = BCDtoDEC(_time[TIME_HOURS])
	if (is12Hour())
	{
		if (isPM())
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

uint8_t RV8803::getMonth()
{
	return BCDtoDEC(_time[TIME_MONTH]);
}

uint8_t RV8803::getYear()
{
	return BCDtoDEC(_time[TIME_YEAR]);
}

uint8_t RV8803::getWeekday()
{
	return BCDtoDEC(_time[TIME_DAY]);
}

//Takes the time from the last build and uses it as the current time
//Works very well as an arduino sketch
bool RV8803::setToCompilerTime()
{
	_time[TIME_SECONDS] = DECtoBCD(BUILD_SECOND);
	_time[TIME_MINUTES] = DECtoBCD(BUILD_MINUTE);
	_time[TIME_HOURS] = DECtoBCD(BUILD_HOUR);

	//Build_Hour is 0-23, convert to 1-12 if needed
	
	
	_time[TIME_MONTH] = DECtoBCD(BUILD_MONTH);
	_time[TIME_DATE] = DECtoBCD(BUILD_DATE);
	_time[TIME_YEAR] = DECtoBCD(BUILD_YEAR - 2000); //! Not Y2K (or Y2.1K)-proof :(
	
	// Calculate weekday (from here: http://stackoverflow.com/a/21235587)
	// 0 = Sunday, 6 = Saturday
	uint16_t d = BUILD_DATE;
	uint16_t m = BUILD_MONTH;
	uint16_t y = BUILD_YEAR;
	uint16_t weekday = (d+=m<3?y--:y-2,23*m/9+d+4+y/4-y/100+y/400)%7 + 1;
	_time[TIME_DAY] = DECtoBCD(weekday);
	
	return setTime(_time, TIME_ARRAY_LENGTH);
}

bool RV8803::setAlarmMinute(uint8_t minute)
{
	uint8_t value = readRegister(RV8803_MINUTES_ALARM);
	value &= (1 << ALARM_ENABLE_MINUTE); //clear everything but enable bit
	value |= DECtoBCD(minute);
	return writeRegister(RV8803_MINUTES_ALARM, value);
}

bool RV8803::setAlarmHour(uint8_t hour)
{
	uint8_t value = readRegister(RV8803_HOURS_ALARM);
	value &= (1 << ALARM_ENABLE_HOUR); //clear everything but enable bit
	value |= DECtoBCD(hour);
	return writeRegister(RV8803_HOURS_ALARM, value);
}

bool RV8803::setAlarmWeekday(uint8_t weekday)
{
	uint8_t value = readRegister(RV8803_WEEKDAYS_DATE_ALARM);
	value &= (1 << ALARM_ENABLE_WEEKDAY_DATE); //clear everything but enable bit
	value |= DECtoBCD(weekday);
	bool returnValue = writeRegister(RV8803_WEEKDAYS_DATE_ALARM, value);
	value = readRegister(RV8803_EXTENSION);
	value &= ~(1 << EXTENSION_WADA);
	value |= (WEEKDAY_ALARM << EXTENSION_WADA);
	returnValue &= writeRegister(RV8803_EXTENSION, value);
	return returnValue;
}

bool RV8803::setAlarmDate(uint8_t date)
{
	uint8_t value = readRegister(RV8803_WEEKDAYS_DATE_ALARM);
	value &= (1 << ALARM_ENABLE_WEEKDAY_DATE); //clear everything but enable bit
	value |= DECtoBCD(date);
	bool returnValue = writeRegister(RV8803_WEEKDAYS_DATE_ALARM, value);
	value = readRegister(RV8803_EXTENSION);
	value &= ~(1 << EXTENSION_WADA);
	value |= (DATE_ALARM << EXTENSION_WADA);
	returnValue &= writeRegister(RV8803_EXTENSION, value);
	return returnValue;
}

/*********************************
Given a bit location, enable the interrupt
INTERRUPT_BLIE	4
INTERRUPT_TIE	3
INTERRUPT_AIE	2
INTERRUPT_EIE	1
*********************************/
void RV8803::enableInterrupt(uint8_t source)
{
	uint8_t value = readRegister(RV8803_CONTROL);
	value |= (1<<source); //Set the interrupt enable bit
	writeRegister(RV8803_CONTROL, value);
}

void RV8803::disableInterrupt(uint8_t source)
{
	uint8_t value = readRegister(RV8803_CONTROL);
	value &= ~(1 << source); //Clear the interrupt enable bit
	writeRegister(RV8803_CONTROL, value);
}

void clearFlagBit(uint8_t flagToClear)
{
	uint8_t value = readRegister(RV8803_FLAG);
	value &= (1 << flagToClear); //clear everything but enable bit
	value |= DECtoBCD(hour);
	return writeRegister(RV8803_HOURS_ALARM, value);
}

/********************************
Set Alarm Mode controls which parts of the time have to match for the alarm to trigger.
When the RTC matches a given time, make an interrupt fire.
Setting a bit to 1 means that the RTC does not check if that value matches to trigger the alarm
********************************/
void RV8803::setItemsToMatchForAlarm(bool minuteAlarm, bool hourAlarm, bool dateAlarm, bool weekdayOrDate)
{
	uint8_t value = readRegister(RV8803_MINUTES_ALARM);
	value &= ~(1 << ALARM_ENABLE_MINUTE); //clear enable bit
	value &= (1 << ALARM_ENABLE_MINUTE);
	writeRegister(RV8803_MINUTES_ALARM, value);
	value = readRegister(RV8803_HOURS_ALARM);
	value &= ~(1 << ALARM_ENABLE_HOUR); //clear enable bit
	value &= (1 << ALARM_ENABLE_HOUR);
	writeRegister(RV8803_HOURS_ALARM, value);
	value = readRegister(RV8803_WEEKDAYS_DATE_ALARM);
	value &= ~(1 << ALARM_ENABLE_HOUR); //clear enable bit
	value &= (1 << ALARM_ENABLE_HOUR);
	writeRegister(RV8803_WEEKDAYS_DATE_ALARM, value);
}


void RV8803::setCountdownTimer(uint8_t duration, uint8_t unit, bool repeat, bool pulse)
{
	// Invalid configurations
	if (duration == 0 || unit > 0b11) {
		return;
	}

	// TIMER_VALUE_) and ! are used to control the countdown timer

	// Set timer value
	/*writeRegister(RV8803_CTDWN_TMR, (duration - 1));
	writeRegister(RV8803_TMR_INITIAL, (duration - 1));

	// Enable timer
	uint8_t value = readRegister(RV8803_CTDWN_TMR_CTRL);
	value &= 0b00011100; // Clear countdown timer bits while preserving ARPT
	value |= unit; // Set clock frequency
	value |= (!pulse << CTDWN_TMR_TM_OFFSET);
	value |= (repeat << CTDWN_TMR_TRPT_OFFSET);
	value |= (1 << CTDWN_TMR_TE_OFFSET); // Timer enable
	writeRegister(RV8803_CTDWN_TMR_CTRL, value);*/
}

void RV8803::clearInterrupts() //Read the status register to clear the current interrupt flags
{
	status();
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

uint8_t RV8803::readRegister(uint8_t addr)
{
	_i2cPort->beginTransmission(RV8803_ADDR);
	_i2cPort->write(addr);
	_i2cPort->endTransmission();

	_i2cPort->requestFrom(RV8803_ADDR, (uint8_t)1);
	if (_i2cPort->available()) {
		return (_i2cPort->read());
	}
	else {
		return (0xFF); //Error
	}
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

	_i2cPort->requestFrom(RV8803_ADDR, len);
	for (uint8_t i = 0; i < len; i++)
	{
		dest[i] = _i2cPort->read();
	}
	
	return(true);
}
