
# class LocalTime 

Global time settings.

## Members

---

### LocalTime & LocalTime::withConfig(LocalTimePosixTimezone config) 

Sets the default global timezone configuration.

```
LocalTime & withConfig(LocalTimePosixTimezone config)
```

---

### const LocalTimePosixTimezone & LocalTime::getConfig() const 

Gets the default global timezone configuration.

```
const LocalTimePosixTimezone & getConfig() const
```

---

### LocalTime & LocalTime::withScheduleLookaheadDays(int value) 

Sets the maximum number of days to look ahead in the schedule for a match (default: 3)

```
LocalTime & withScheduleLookaheadDays(int value)
```

#### Parameters
* `value` 

#### Returns
LocalTime&

---

### int LocalTime::getScheduleLookaheadDays() const 

Gets the maximum number of days to look ahead in the schedule for a match.

```
int getScheduleLookaheadDays() const
```

#### Returns
int

# class LocalTimeChange 

Handles the time change part of the Posix timezone string like "M3.2.0/2:00:00".

Other formats with shortened time of day are also allowed like "M3.2.0/2" or even "M3.2.0" (midnight) are also allowed. Since the hour is local time, it can also be negative "M3.2.0/-1".

## Members

---

### int8_t month 

1-12, 1=January

```
int8_t month
```

---

### int8_t week 

1-5, 1=first

```
int8_t week
```

---

### int8_t dayOfWeek 

0-6, 0=Sunday, 1=Monday, ...

```
int8_t dayOfWeek
```

---

### int8_t valid 

true = valid

```
int8_t valid
```

---

### LocalTimeHMS hms 

Local time when timezone change occurs.

```
LocalTimeHMS hms
```

---

###  LocalTimeChange::LocalTimeChange() 

Default contructor.

```
 LocalTimeChange()
```

---

###  LocalTimeChange::~LocalTimeChange() 

Destructor.

```
virtual  ~LocalTimeChange()
```

---

###  LocalTimeChange::LocalTimeChange(const char * str) 

Constructs a time change object with a string format (calls parse())

```
 LocalTimeChange(const char * str)
```

#### Parameters
* `str` the time change string to parse

The time change string is part of the POSIX timezone specification and looks something like "M3.2.0/2:00:00".

---

### void LocalTimeChange::clear() 

Clears all values.

```
void clear()
```

---

### void LocalTimeChange::parse(const char * str) 

Parses a time change string.

```
void parse(const char * str)
```

#### Parameters
* `str` the time change string to parse

The time change string is part of the POSIX timezone specification and looks something like "M3.2.0/2:00:00".

* M3 indicates that DST starts on the 3rd month (March)

* 2 is the week number (second week)

* 0 is the day of week (0 = Sunday)

* 2:00:00 at 2 AM local time, the transition occurs

Setting the week to 5 essentially means the last week of the month. If the month does not have a fifth week for that day of the week, then the fourth is used instead.

---

### String LocalTimeChange::toString() const 

Turns the parsed data into a normalized string like "M3.2.0/2:00:00".

```
String toString() const
```

---

### time_t LocalTimeChange::calculate(struct tm * pTimeInfo, LocalTimeHMS tzAdjust) const 

Calculate the time change in a given year.

```
time_t calculate(struct tm * pTimeInfo, LocalTimeHMS tzAdjust) const
```

On input, pTimeInfo must have a valid tm_year (121 = 2021) set.

On output, all struct tm values are set appropriately with UTC values of when the time change occurs.

# class LocalTimeConvert 

Perform time conversions. This is the main class you will need.

## Members

---

### Position position 

Where time is relative to DST.

```
Position position
```

---

### LocalTimePosixTimezone config 

Timezone configuration for this time conversion.

```
LocalTimePosixTimezone config
```

If you don't specify this using withConfig then the global setting is retrieved from the LocalTime singleton instance.

---

### time_t time 

The time that is being converted. This is always Unix time at UTC.

```
time_t time
```

Seconds after January 1, 1970, UTC. Using methods like nextDay() increment this value.

---

### LocalTimeValue localTimeValue 

The local time that corresponds to time.

```
LocalTimeValue localTimeValue
```

The convert() method sets this, as do methods like nextDay(). The local time will depend on the timezone set in config, as well as the date which will determine whether it's daylight saving or not.

---

### time_t dstStart 

The time that daylight saving starts, Unix time, UTC.

```
time_t dstStart
```

The config specifies the rule (2nd Sunday in March, for example), and the local time that the change occurs. This is the UTC value that corresponds to that rule in the year specified by time.

Note in the southern hemisphere, dstStart is after standardStart.

---

### struct tm dstStartTimeInfo 

The struct tm that corresponds to dstStart (UTC)

```
struct tm dstStartTimeInfo
```

---

### time_t standardStart 

The time that standard time starts, Unix time, UTC.

```
time_t standardStart
```

The config specifies the rule (1st Sunday in November, for example), and the local time that the change occurs. This is the UTC value that corresponds to that rule in the year specified by time.

Note in the southern hemisphere, dstStart is after standardStart.

---

### struct tm standardStartTimeInfo 

The struct tm that corresponds to standardStart (UTC)

```
struct tm standardStartTimeInfo
```

---

### LocalTimeConvert & LocalTimeConvert::withConfig(LocalTimePosixTimezone config) 

Sets the timezone configuration to use for time conversion.

```
LocalTimeConvert & withConfig(LocalTimePosixTimezone config)
```

If you do not use withConfig() the global default set in the LocalTime class is used. If neither are set, the local time is UTC (with no DST).

---

### LocalTimeConvert & LocalTimeConvert::withTime(time_t time) 

Sets the UTC time to begin conversion from.

```
LocalTimeConvert & withTime(time_t time)
```

#### Parameters
* `time` The time (UTC) to set. This is the Unix timestamp (seconds since January 1, 1970) UTC such as returned by Time.now().

This does not start the conversion; you must also call the convert() method after setting all of the settings you want to use.

For the current time, you can instead use withCurrentTime();

---

### LocalTimeConvert & LocalTimeConvert::withCurrentTime() 

Use the current time as the time to start with.

```
LocalTimeConvert & withCurrentTime()
```

This does not start the conversion; you must also call the convert() method after setting all of the settings you want to use.

---

### void LocalTimeConvert::convert() 

Do the time conversion.

```
void convert()
```

You must call this after changing the configuration or the time using withTime() or withCurrentTime()

---

### bool LocalTimeConvert::isDST() const 

Returns true if the current time is in daylight saving time.

```
bool isDST() const
```

---

### bool LocalTimeConvert::isStandardTime() const 

Returns true of the current time in in standard time.

```
bool isStandardTime() const
```

---

### void LocalTimeConvert::addSeconds(int seconds) 

Adds a number of seconds to the current object.

```
void addSeconds(int seconds)
```

#### Parameters
* `seconds`

---

### void LocalTimeConvert::nextMinuteMultiple(int increment, int startingModulo) 

Moves the current time the next specified multiple of minutes.

```
void nextMinuteMultiple(int increment, int startingModulo)
```

#### Parameters
* `increment` Typically something like 5, 15, 20, 30 that 60 is evenly divisible by

* `startingModulo` (optional). If present, must be 0 < startingModulo < increment

Moves the time forward to the next multiple of that number of minutes. For example, if the clock is at :10 past the hour and the multiple is 15, then time will be updated to :15. If the time is equal to an even multple, the next multiple is selected.

Upon completion, all fields are updated appropriately. For example:

* time specifies the time_t of the new time at UTC

* localTimeValue contains the broken-out values for the local time

* isDST() return true if the new time is in daylight saving time

---

### void LocalTimeConvert::nextTime(LocalTimeHMS hms) 

Moves the current time the next specified local time. This could be today or tomorrow.

```
void nextTime(LocalTimeHMS hms)
```

#### Parameters
* `hms` Moves to the next occurrence of that time of day (local time)

Upon completion, all fields are updated appropriately. For example:

* time specifies the time_t of the new time at UTC

* localTimeValue contains the broken-out values for the local time

* isDST() return true if the new time is in daylight saving time

---

### void LocalTimeConvert::nextTimeList(std::initializer_list< LocalTimeHMS > hmsList) 

Moves the current time the closest local time om hmsList. This could be today or tomorrow.

```
void nextTimeList(std::initializer_list< LocalTimeHMS > hmsList)
```

#### Parameters
* `hmsList` An initialize list of LocalTimeHMS surrounded by {}

For example, this sets the time to the nearest noon or midnight local time greater than the time set in this object:

conv.nextTimeList({LocalTimeHMS("00:00"), LocalTimeHMS("12:00")});

---

### void LocalTimeConvert::nextDay(LocalTimeHMS hms) 

Moves the current time to the next day.

```
void nextDay(LocalTimeHMS hms)
```

#### Parameters
* `hms` If specified, moves to that time of day (local time). If omitted, leaves the current time and only changes the date.

Upon completion, all fields are updated appropriately. For example:

* time specifies the time_t of the new time at UTC

* localTimeValue contains the broken-out values for the local time

* isDST() return true if the new time is in daylight saving time

---

### void LocalTimeConvert::nextDayOrTimeChange(LocalTimeHMS hms) 

Moves the current to the next day, or right after the next time change, whichever comes first.

```
void nextDayOrTimeChange(LocalTimeHMS hms)
```

#### Parameters
* `hms` If specified, moves to that time of day (local time). If omitted, leaves the current time and only changes the date.

Upon completion, all fields are updated appropriately. For example:

* time specifies the time_t of the new time at UTC

* localTimeValue contains the broken-out values for the local time

* isDST() return true if the new time is in daylight saving time

This method is used when you want to synchronize an external device clock daily to keep it synchronized, or right after a time change.

Do not pick the local time of the time change as the hms time! For example, in the United State do *not* select 02:00:00. The reason is that on spring forward, that time doesn't actually exist, because as soon as the clock hits 02:00:00 it jumps forward to 03:00:00 local time. Picking 03:00:00 or really any other time that's not between 02:00:00 and 02:59:59 is fine. During fall back, even though you've picked the time sync time to be 03:00 local time it will sync at the time of the actual time change correctly, which is why this function is different than nextDay().

---

### bool LocalTimeConvert::nextDayOfWeek(int dayOfWeek, LocalTimeHMS hms) 

Moves the current time to the next of the specified day of week.

```
bool nextDayOfWeek(int dayOfWeek, LocalTimeHMS hms)
```

#### Parameters
* `dayOfWeek` The day of week (0 - 6, 0 = Sunday, 1 = Monday, ..., 6 = Saturday)

* `hms` If specified, moves to that time of day (local time). If omitted, leaves the current time and only changes the date.

Upon completion, all fields are updated appropriately. For example:

* time specifies the time_t of the new time at UTC

* localTimeValue contains the broken-out values for the local time

* isDST() return true if the new time is in daylight saving time

---

### void LocalTimeConvert::nextWeekday(LocalTimeHMS hms) 

Returns the next day that is a weekday (Monday - Friday)

```
void nextWeekday(LocalTimeHMS hms)
```

#### Parameters
* `hms` If specified, moves to that time of day (local time). If omitted, leaves the current time and only changes the date.

Upon completion, all fields are updated appropriately. For example:

* time specifies the time_t of the new time at UTC

* localTimeValue contains the broken-out values for the local time

* isDST() return true if the new time is in daylight saving time

---

### void LocalTimeConvert::nextWeekendDay(LocalTimeHMS hms) 

Returns the next day that is a weekend day (Saturday or Sunday)

```
void nextWeekendDay(LocalTimeHMS hms)
```

#### Parameters
* `hms` If specified, moves to that time of day (local time). If omitted, leaves the current time and only changes the date.

Upon completion, all fields are updated appropriately. For example:

* time specifies the time_t of the new time at UTC

* localTimeValue contains the broken-out values for the local time

* isDST() return true if the new time is in daylight saving time

---

### bool LocalTimeConvert::nextDayOfMonth(int dayOfMonth, LocalTimeHMS hms) 

Moves the date and time (local time) forward to the specified day of month and local time.

```
bool nextDayOfMonth(int dayOfMonth, LocalTimeHMS hms)
```

#### Parameters
* `dayOfMonth` The day of the month (1 = first day of the month). See also special cases below.

* `hms` If specified, moves to that time of day (local time). If omitted, leaves the current time and only changes the date.

This version will move to the closest forward time. It could be as close as 1 second later, but it will always advance at least once second. It could be as much as 1 month minus 1 second later.

Upon completion, all fields are updated appropriately. For example:

* time specifies the time_t of the new time at UTC

* localTimeValue contains the broken-out values for the local time

* isDST() return true if the new time is in daylight saving time

dayOfMonth is normally a a day number (1 = first day of the month). There are also special cases: 0 = the last day of the month, -1 the second to last day of month, ... The number of days in the month are based on the date in local time.

---

### bool LocalTimeConvert::nextDayOfNextMonth(int dayOfMonth, LocalTimeHMS hms) 

Moves the date and time (local time) forward to the specified day of month and local time.

```
bool nextDayOfNextMonth(int dayOfMonth, LocalTimeHMS hms)
```

#### Parameters
* `dayOfMonth` The day of the month (1 = first day of the month). See also special cases below.

* `hms` If specified, moves to that time of day (local time). If omitted, leaves the current time and only changes the date.

This version always picks the next month, even if the target day of month hasn't been reached in this month yet. This will always more forward at least a month, and may be as much as two months minus one day.

Upon completion, all fields are updated appropriately. For example:

* time specifies the time_t of the new time at UTC

* localTimeValue contains the broken-out values for the local time

* isDST() return true if the new time is in daylight saving time

dayOfMonth is normally a a day number (1 = first day of the month). There are also special cases: 0 = the last day of the month, -1 the second to last day of month, ... The number of days in the month are based on the date in local time.

---

### bool LocalTimeConvert::nextDayOfWeekOrdinal(int dayOfWeek, int ordinal, LocalTimeHMS hms) 

Moves the date and time (local time) forward to the specified day of month and local time.

```
bool nextDayOfWeekOrdinal(int dayOfWeek, int ordinal, LocalTimeHMS hms)
```

#### Parameters
* `dayOfWeek` The day of week (0 - 6, 0 = Sunday, 1 = Monday, ..., 6 = Saturday)

* `ordinal` 1 = first of that day of week in month, 2 = second, ...

* `hms` If specified, moves to that time of day (local time). If omitted, leaves the current time and only changes the date.

If you specify an ordinal that does not exist (for example, there may not be a 5th ordinal for certain days in certain months), returns false and leaves the date unchanged.

Upon successful completion, all fields are updated appropriately. For example:

* time specifies the time_t of the new time at UTC

* localTimeValue contains the broken-out values for the local time

* isDST() return true if the new time is in daylight saving time

---

### void LocalTimeConvert::nextLocalTime(LocalTimeHMS hms) 

Sets the time to the nearest hms in local time in the future.

```
void nextLocalTime(LocalTimeHMS hms)
```

#### Parameters
* `hms` The time of day to set in local time

Moves the time forward to the next instance of hms in local time. If hms has not occurred yet, it will select the one today, but it will always move the time forward.

There is a weird special case on the beginning of daylight saving (spring forward in the northern hemisphere). If you select a hms between 2:00 AM and 2:59:59 AM local time, this time does not exist on spring forward day because it's the hour that is skipped. In order to preserve the requirement that the time will always be advanced by this call, it will jump forward to the next day when the 2:00 hour occurs next. (The hour may be different in other locations, for example it's 1:00 AM in the UK.)

In the case of fall back, if you specify a hms in the repeated hour (1:00:00 to 1:59:59) the time returned will be the second time this time occurs, in standard time in the US.

Upon completion, all fields are updated appropriately. For example:

* time specifies the time_t of the new time at UTC

* localTimeValue contains the broken-out values for the local time

* isDST() return true if the new time is in daylight saving time

---

### bool LocalTimeConvert::nextSchedule(const LocalTimeSchedule & schedule) 

Sets the time to the nearest scheduled time in the future base on the schedule.

```
bool nextSchedule(const LocalTimeSchedule & schedule)
```

#### Parameters
* `schedule`

---

### void LocalTimeConvert::atLocalTime(LocalTimeHMS hms) 

Changes the time of day to the specified hms in local time on the same local day.

```
void atLocalTime(LocalTimeHMS hms)
```

#### Parameters
* `hms` A LocalTimeHMS object with hour, minute, and second

You can use the string constructor like this to set the time to 2:00 PM local time.

```cpp
converter.atLocalTime(LocalTimeHms("14:00:00"));
```

It's possible that this will set the time to a time earlier than the object's current time. To only set a time in the future, use nextLocalTime() instead.

Upon completion, all fields are updated appropriately. For example:

* time specifies the time_t of the new time at UTC

* localTimeValue contains the broken-out values for the local time

* isDST() return true if the new time is in daylight saving time

---

### LocalTimeHMS LocalTimeConvert::getLocalTimeHMS() const 

Get the value of this object as a LocalTimeHMS (hour minute second)

```
LocalTimeHMS getLocalTimeHMS() const
```

#### Returns
LocalTimeHMS

---

### LocalTimeYMD LocalTimeConvert::getLocalTimeYMD() const 

Get the value of this object as a LocalTimeYMD (year month day0)

```
LocalTimeYMD getLocalTimeYMD() const
```

#### Returns
LocalTimeYMD

---

### String LocalTimeConvert::timeStr() 

Works like Time.timeStr() to generate a readable string of the local time.

```
String timeStr()
```

Uses asctime formatting, which looks like "Fri Jan  1 18:45:56 2021". The strings are not localized; they're always in English.

---

### String LocalTimeConvert::format(const char * formatSpec) 

Works like Time.format()

```
String format(const char * formatSpec)
```

#### Parameters
* `formatSpec` the format specifies, which can be

* TIME_FORMAT_DEFAULT (example: "Thu Apr  1 12:00:00 2021")

* TIME_FORMAT_ISO8601_FULL (example: "2021-04-01T12:00:00-04:00")

* custom format based on strftime()

There are many options to strftime described here: [https://www.cplusplus.com/reference/ctime/strftime/?kw=strftime](https://www.cplusplus.com/reference/ctime/strftime/?kw=strftime)

Unlike Time.format(), you can use Z to output the timezone abbreviation, for example "EDT" for the Eastern United States, daylight saving instead of -04:00.

The z formatting matches that of Time.format(), which is wrong. The correct output should be "-400" but the output will be "-04:00" for compatibility.

---

### String LocalTimeConvert::zoneName() const 

Returns the abbreviated time zone name for the current time.

```
String zoneName() const
```

For example, for the United States east coast, EST or EDT depending on whether the current time is DST or not. See also isDST().

This string comes from the LocalTimePosixTimezone object.

---

### int LocalTimeConvert::lastDayOfMonth() const 

Returns the last day of the month, local time (based on localTimeValue)

```
int lastDayOfMonth() const
```

---

### enum Position 

Whether the specified time is DST or not. See also isDST().

```
enum Position
```

 Values                         | Descriptions                                
--------------------------------|---------------------------------------------
BEFORE_DST            | This time is before the start of DST (northern hemisphere)
IN_DST            | This time is in daylight saving time (northern hemisphere)
AFTER_DST            | This time is after the end of DST (northern hemisphere)
BEFORE_STANDARD            | This time is before the start of standard time (southern hemisphere)
IN_STANDARD            | This time is in standard saving time (southern hemisphere)
AFTER_STANDARD            | This time is after the end of standard time (southern hemisphere)
NO_DST            | This config does not use daylight saving.

# class LocalTimeDayOfWeek 

Class for managing a mask value of zero or more days of the week.

Day 0 = Sunday, 1 = Monday, ..., 6 = Saturday

This class is copyable and assignable and can be tested for equality and inequality

## Members

---

### uint8_t dayOfWeekMask 

Mask value for this object.

```
uint8_t dayOfWeekMask
```

---

###  LocalTimeDayOfWeek::LocalTimeDayOfWeek() 

Default constructor with no days of week set.

```
 LocalTimeDayOfWeek()
```

---

###  LocalTimeDayOfWeek::LocalTimeDayOfWeek(uint8_t mask) 

Sets days of weeks with a bit mask of days.

```
 LocalTimeDayOfWeek(uint8_t mask)
```

#### Parameters
* `mask` Pass values like MASK_SUNDAY, MASK_WEEKDAYS, MASK_WEEKENDS, MASK_ALL, or a custom value

---

### LocalTimeDayOfWeek & LocalTimeDayOfWeek::withWeekdays() 

Convenience fluent setter to set all weekdays (Monday - Friday)

```
LocalTimeDayOfWeek & withWeekdays()
```

#### Returns
LocalTimeDayOfWeek&

---

### LocalTimeDayOfWeek & LocalTimeDayOfWeek::withWeekends() 

Convenience fluent setter to set weekend days (Saturday and Sunday)

```
LocalTimeDayOfWeek & withWeekends()
```

#### Returns
LocalTimeDayOfWeek&

---

### LocalTimeDayOfWeek & LocalTimeDayOfWeek::withAllDays() 

Convenience fluent setter to set all days.

```
LocalTimeDayOfWeek & withAllDays()
```

#### Returns
LocalTimeDayOfWeek&

---

### bool LocalTimeDayOfWeek::isSet(int dayOfWeek) const 

Returns true if the specified dayOfWeek is set in the mask.

```
bool isSet(int dayOfWeek) const
```

#### Parameters
* `dayOfWeek` Same as struct tm. 0 < dayOfWeek <= 6. Sunday = 0. 

#### Returns
true 

#### Returns
false

---

### bool LocalTimeDayOfWeek::isSet(LocalTimeYMD ymd) const 

return true if ymd is a day set in this object

```
bool isSet(LocalTimeYMD ymd) const
```

#### Parameters
* `ymd` 

#### Returns
true 

#### Returns
false

---

### bool LocalTimeDayOfWeek::isEmpty() const 

Returns true if no days of the week are set in this object.

```
bool isEmpty() const
```

#### Returns
true 

#### Returns
false

---

### uint8_t LocalTimeDayOfWeek::getMask() const 

Get the current mask value.

```
uint8_t getMask() const
```

#### Returns
uint8_t

---

### void LocalTimeDayOfWeek::setMask(uint8_t mask) 

Set the mask value.

```
void setMask(uint8_t mask)
```

#### Parameters
* `mask`

---

### String LocalTimeDayOfWeek::toString() const 

Converts this object to a human-readable string.

```
String toString() const
```

#### Returns
String

---

### bool LocalTimeDayOfWeek::operator==(const LocalTimeDayOfWeek & other) const 

Tests if this object has the same mask as other.

```
bool operator==(const LocalTimeDayOfWeek & other) const
```

#### Parameters
* `other` 

#### Returns
true 

#### Returns
false

---

### bool LocalTimeDayOfWeek::operator!=(const LocalTimeDayOfWeek & other) const 

Tests if this object does not have the same mask as other.

```
bool operator!=(const LocalTimeDayOfWeek & other) const
```

#### Parameters
* `other` 

#### Returns
true 

#### Returns
false

# class LocalTimeHMS 

Container for holding an hour minute second time value.

## Members

---

### int8_t hour 

0-23 hour (could also be negative)

```
int8_t hour
```

---

### int8_t minute 

0-59 minute

```
int8_t minute
```

---

### int8_t second 

0-59 second

```
int8_t second
```

---

### int8_t ignore 

Special case.

```
int8_t ignore
```

---

###  LocalTimeHMS::LocalTimeHMS() 

Default constructor. Sets time to 00:00:00.

```
 LocalTimeHMS()
```

---

###  LocalTimeHMS::~LocalTimeHMS() 

Destructor.

```
virtual  ~LocalTimeHMS()
```

---

###  LocalTimeHMS::LocalTimeHMS(const char * str) 

Constructs the object from a time string.

```
 LocalTimeHMS(const char * str)
```

#### Parameters
* `str` The time string

The time string is normally of the form HH:MM:SS, such as "04:00:00" for 4:00 AM. The hour is in 24-hour format. Other formats are supported as well, including omitting the seconds (04:00), or including only the hour "04", or omitting the leadings zeros (4:0:0).

Additionally, the hour could be negative, used in UTC DST offsets. The minute and second are always positive (0-59). The hour could also be > 24 when used as a timezone offset.

---

###  LocalTimeHMS::LocalTimeHMS(const LocalTimeValue & value) 

Construct this HMS from a LocalTimeValue (which contains YMD and HMS)

```
 LocalTimeHMS(const LocalTimeValue & value)
```

#### Parameters
* `value`

---

### void LocalTimeHMS::clear() 

Sets the hour, minute, and second to 0.

```
void clear()
```

---

### void LocalTimeHMS::parse(const char * str) 

Parse a "H:MM:SS" string.

```
void parse(const char * str)
```

#### Parameters
* `str` Input string

Multiple formats are supported, and parts are optional:

* H:MM:SS (examples: "2:00:00" or "2:0:0")

* H:MM (examples: "2:00" or "2:0")

* H (examples: "2")

Hours are always 0 - 23 (24-hour clock). Can also be a negative hour -1 to -23.

---

### String LocalTimeHMS::toString() const 

Turns the parsed data into a normalized string of the form: "HH:MM:SS" (24-hour clock, with leading zeroes)

```
String toString() const
```

---

### int LocalTimeHMS::toSeconds() const 

Convert hour minute second into a number of seconds (simple multiplication and addition)

```
int toSeconds() const
```

---

### void LocalTimeHMS::fromTimeInfo(const struct tm * pTimeInfo) 

Sets the hour, minute, and second fields from a struct tm.

```
void fromTimeInfo(const struct tm * pTimeInfo)
```

---

### void LocalTimeHMS::fromLocalTimeValue(const LocalTimeValue & value) 

Sets the HMS from a LocalTimeValue.

```
void fromLocalTimeValue(const LocalTimeValue & value)
```

#### Parameters
* `value`

---

### void LocalTimeHMS::toTimeInfo(struct tm * pTimeInfo) const 

Fill in the tm_hour, tm_min, and tm_sec fields of a struct tm from the values in this object.

```
void toTimeInfo(struct tm * pTimeInfo) const
```

#### Parameters
* `pTimeInfo` The struct tm to modify

---

### void LocalTimeHMS::adjustTimeInfo(struct tm * pTimeInfo) const 

Adjust the values in a struct tm from the values in this object.

```
void adjustTimeInfo(struct tm * pTimeInfo) const
```

#### Parameters
* `pTimeInfo` The struct tm to modify

After calling this, the values in the struct tm may be out of range, for example tm_hour > 23. This is fine, as calling mktime/gmtime normalizes this case and carries out-of-range values into the other fields as necessary.

---

### void LocalTimeHMS::fromJson(JSONValue jsonObj) 

Parses a JSON value of type string in HH:MM:SS format.

```
void fromJson(JSONValue jsonObj)
```

#### Parameters
* `jsonObj`

---

### LocalTimeHMS & LocalTimeHMS::withHour(int hour) 

Sets this object to be the specified hour, with minute and second set to 0.

```
LocalTimeHMS & withHour(int hour)
```

#### Parameters
* `hour` 0 <= hour < 24 

#### Returns
LocalTimeHMS&

---

### LocalTimeHMS & LocalTimeHMS::withHourMinute(int hour, int minute) 

Sets this object to be the specified hour and minute, with second set to 0.

```
LocalTimeHMS & withHourMinute(int hour, int minute)
```

#### Parameters
* `hour` 0 <= hour < 24 

* `minute` 0 <= minute < 60 

#### Returns
LocalTimeHMS&

---

### int LocalTimeHMS::compareTo(const LocalTimeHMS & other) const 

Compare two LocalTimeHMS objects.

```
int compareTo(const LocalTimeHMS & other) const
```

#### Parameters
* `other` The item to compare to 

#### Returns
int -1 if this item is < other; 0 if this = other, or +1 if this > other

---

### bool LocalTimeHMS::operator==(const LocalTimeHMS & other) const 

Returns true if this item is equal to other. Compares hour, minute, and second.

```
bool operator==(const LocalTimeHMS & other) const
```

#### Parameters
* `other` 

#### Returns
true 

#### Returns
false

---

### bool LocalTimeHMS::operator!=(const LocalTimeHMS & other) const 

Returns true if this item is not equal to other.

```
bool operator!=(const LocalTimeHMS & other) const
```

#### Parameters
* `other` 

#### Returns
true 

#### Returns
false

---

### bool LocalTimeHMS::operator<(const LocalTimeHMS & other) const 

Returns true if this item is < other.

```
bool operator<(const LocalTimeHMS & other) const
```

#### Parameters
* `other` 

#### Returns
true 

#### Returns
false

---

### bool LocalTimeHMS::operator>(const LocalTimeHMS & other) const 

Returns true if this item is > other.

```
bool operator>(const LocalTimeHMS & other) const
```

#### Parameters
* `other` 

#### Returns
true 

#### Returns
false

---

### bool LocalTimeHMS::operator<=(const LocalTimeHMS & other) const 

Returns true if this item <= other.

```
bool operator<=(const LocalTimeHMS & other) const
```

#### Parameters
* `other` 

#### Returns
true 

#### Returns
false

---

### bool LocalTimeHMS::operator>=(const LocalTimeHMS & other) const 

Returns true if this item is >= other.

```
bool operator>=(const LocalTimeHMS & other) const
```

#### Parameters
* `other` 

#### Returns
true 

#### Returns
false

# class LocalTimeHMSRestricted 

```
class LocalTimeHMSRestricted
  : public LocalTimeHMS
  : public LocalTimeRestrictedDate
```  

HMS values, but only on specific days of week or dates (with optional exceptions)

## Members

---

###  LocalTimeHMSRestricted::LocalTimeHMSRestricted() 

Default constructor restricts to no valid dates!

```
 LocalTimeHMSRestricted()
```

---

###  LocalTimeHMSRestricted::LocalTimeHMSRestricted(LocalTimeHMS hms) 

Default Sets a hour minute second value on any date.

```
 LocalTimeHMSRestricted(LocalTimeHMS hms)
```

#### Parameters
* `hms` The hour minute second to set

---

###  LocalTimeHMSRestricted::LocalTimeHMSRestricted(LocalTimeHMS hms, LocalTimeRestrictedDate restrictedDate) 

Default Sets a hour minute second value with date restrictions.

```
 LocalTimeHMSRestricted(LocalTimeHMS hms, LocalTimeRestrictedDate restrictedDate)
```

#### Parameters
* `hms` The hour minute second to set 

* `restrictedDate` Date restrictions

---

### void LocalTimeHMSRestricted::fromJson(JSONValue jsonObj) 

Fills in this object from JSON data.

```
void fromJson(JSONValue jsonObj)
```

#### Parameters
* `jsonObj` 

Keys:

* t (string) Time string in HH:MM:SS format (can omit MM and SS parts, see LocalTimeHMS)

* y (integer) mask value for onlyOnDays (optional, from LocalTimeRestrictedDate)

* a (array) Array of YYYY-MM-DD value strings to allow (optional, from LocalTimeRestrictedDate)

* x (array) Array of YYYY-MM-DD values to exclude (optional, from LocalTimeRestrictedDate)

# class LocalTimeIgnoreHMS 

```
class LocalTimeIgnoreHMS
  : public LocalTimeHMS
```  

This class can be passed to most functions that take a LocalTimeHMS to instead not set the HMS.

## Members

---

###  LocalTimeIgnoreHMS::LocalTimeIgnoreHMS() 

Special version of LocalTimeHMS that does not set the HMS.

```
 LocalTimeIgnoreHMS()
```

---

### String LocalTimeIgnoreHMS::toString() const 

Returns a human readable version of this object.

```
virtual String toString() const
```

#### Returns
String

# class LocalTimePosixTimezone 

Parses a Posix timezone string into its component parts.

For the Eastern US timezone, the string is: "EST5EDT,M3.2.0/2:00:00,M11.1.0/2:00:00"

* EST is the standard timezone name

* 5 is the offset in hours (the sign is backwards from the normal offset from UTC)

* EDT is the daylight saving timezone name

* M3 indicates that DST starts on the 3rd month (March)

* 2 is the week number (second week)

* 0 is the day of week (0 = Sunday)

* 2:00:00 at 2 AM local time, the transition occurs

* M11 indicates that standard time begins in the 11th month (November)

* 1 is the week number (first week)

* 0 is the day of week (0 = Sunday)

* 2:00:00 at 2 AM local time, the transition occurs

There are many other acceptable formats, including formats for locations that don't have DST.

For more information, see: [https://developer.ibm.com/technologies/systems/articles/au-aix-posix/](https://developer.ibm.com/technologies/systems/articles/au-aix-posix/)

## Members

---

### String dstName 

Daylight saving timezone name (empty string if no DST)

```
String dstName
```

---

### LocalTimeHMS dstHMS 

Daylight saving time shift (relative to UTC)

```
LocalTimeHMS dstHMS
```

---

### String standardName 

Standard time timezone name.

```
String standardName
```

---

### LocalTimeHMS standardHMS 

Standard time shift (relative to UTC). Note that this is positive in the United States, which is kind of backwards.

```
LocalTimeHMS standardHMS
```

---

### LocalTimeChange dstStart 

Rule for when DST starts.

```
LocalTimeChange dstStart
```

---

### LocalTimeChange standardStart 

Rule for when standard time starts.

```
LocalTimeChange standardStart
```

---

### bool valid 

true if the configuration looks valid

```
bool valid
```

---

###  LocalTimePosixTimezone::LocalTimePosixTimezone() 

Default constructor (no timezone set)

```
 LocalTimePosixTimezone()
```

---

###  LocalTimePosixTimezone::~LocalTimePosixTimezone() 

Destructor.

```
virtual  ~LocalTimePosixTimezone()
```

---

###  LocalTimePosixTimezone::LocalTimePosixTimezone(const char * str) 

Constructs the object with a specified timezone configuration.

```
 LocalTimePosixTimezone(const char * str)
```

Calls parse() internally.

---

### void LocalTimePosixTimezone::clear() 

Clears the timezone setting in this object.

```
void clear()
```

---

### bool LocalTimePosixTimezone::parse(const char * str) 

Parses the timezone configuration string.

```
bool parse(const char * str)
```

#### Parameters
* `str` The string, for example: "EST5EDT,M3.2.0/2:00:00,M11.1.0/2:00:00"

If the string is not valid this function returns false and the valid flag will be clear. You can call isValid() to check the validity at any time (such as if you are using the constructor with a string that does not return a boolean).

---

### bool LocalTimePosixTimezone::hasDST() const 

Returns true if this timezone configuration has daylight saving.

```
bool hasDST() const
```

---

### bool LocalTimePosixTimezone::isValid() const 

Returns true if this timezone configuration has been set and appears valid.

```
bool isValid() const
```

---

### bool LocalTimePosixTimezone::isZ() const 

Returns true if this timezone configuration is UTC.

```
bool isZ() const
```

# class LocalTimeRange 

```
class LocalTimeRange
  : public LocalTimeRestrictedDate
```  

Class to hold a time range in local time in HH:MM:SS format.

## Members

---

### LocalTimeHMS hmsStart 

Starting time, inclusive.

```
LocalTimeHMS hmsStart
```

---

### LocalTimeHMS hmsEnd 

Ending time, inclusive.

```
LocalTimeHMS hmsEnd
```

---

###  LocalTimeRange::LocalTimeRange() 

Construct a new Time Range object with the range of the entire day (inclusive)

```
 LocalTimeRange()
```

This is start = 00:00:00, end = 23:59:59. The system clock does not have a concept of leap seconds.

---

###  LocalTimeRange::LocalTimeRange(LocalTimeHMS hmsStart, LocalTimeHMS hmsEnd) 

Construct a new Time Range object with the specifies start and end times.

```
 LocalTimeRange(LocalTimeHMS hmsStart, LocalTimeHMS hmsEnd)
```

#### Parameters
* `hmsStart` Start time in local time 00:00:00 <= hmsStart <= 23:59:59 

* `hmsEnd` End time in local time 00:00:00 <= hmsStart <= 23:59:59 (optional)

Note that 24:00:00 is not a valid time. You should generally use inclusive times such that 23:59:59 is the end of the day.

---

###  LocalTimeRange::LocalTimeRange(LocalTimeHMS hmsStart, LocalTimeHMS hmsEnd, LocalTimeRestrictedDate dateRestriction) 

Construct a new object that specifies start time, end time, and date restrictions.

```
 LocalTimeRange(LocalTimeHMS hmsStart, LocalTimeHMS hmsEnd, LocalTimeRestrictedDate dateRestriction)
```

#### Parameters
* `hmsStart` Start time in local time 00:00:00 <= hmsStart <= 23:59:59 

* `hmsEnd` End time in local time 00:00:00 <= hmsStart <= 23:59:59 

* `dateRestriction` Only use this time range on certain dates

---

### void LocalTimeRange::clear() 

Clear time range to all day, every day.

```
void clear()
```

---

### time_t LocalTimeRange::getTimeSpan(const LocalTimeConvert & conv) const 

Get the number of seconds between start and end based on a LocalTimeConvert object.

```
time_t getTimeSpan(const LocalTimeConvert & conv) const
```

The reason for the conv object is that it contains the time to calculate at, as well as the daylight saving time settings. This methods takes into account the actual number of seconds including when a time change is crossed.

#### Parameters
* `conv` The time and timezone settings to calculate the time span at 

#### Returns
time_t Time difference in seconds

In the weird case that start > end, it can return a negative value, as time_t is a signed long (or long long) value.

This does not take into account date restrictions!

---

### int LocalTimeRange::compareTo(LocalTimeHMS hms) const 

Compares a time (LocalTimeHHS, local time) to this time range.

```
int compareTo(LocalTimeHMS hms) const
```

#### Parameters
* `hms` 

#### Returns
int -1 if hms is before hmsStart, 0 if in range, +1 if hms is after hmsEnd

---

### bool LocalTimeRange::isValidDate(LocalTimeYMD ymd) const 

Returns true if the date restrictions allow this day.

```
bool isValidDate(LocalTimeYMD ymd) const
```

#### Parameters
* `ymd` 

#### Returns
true 

#### Returns
false

---

### bool LocalTimeRange::inRange(LocalTimeValue localTimeValue) const 

Returns true if the date restrictions allow this date and the time is in this range (inclusive)

```
bool inRange(LocalTimeValue localTimeValue) const
```

#### Parameters
* `localTimeValue` 

#### Returns
true 

#### Returns
false

---

### void LocalTimeRange::fromTime(LocalTimeHMSRestricted hms) 

Set the date restrictions from a LocalTimeHMSRestricted object.

```
void fromTime(LocalTimeHMSRestricted hms)
```

#### Parameters
* `hms` LocalTimeHMSRestricted, really only uses the date restrictions, not the HMS part

---

### void LocalTimeRange::fromJson(JSONValue jsonObj) 

Fills in the time range from a JSON object.

```
void fromJson(JSONValue jsonObj)
```

#### Parameters
* `jsonObj` 

Keys:

* s (string) The start time (HH:MM:SS format, can omit MM or SS)

* e (string) The end time (HH:MM:SS format, can omit MM or SS)

# class LocalTimeRestrictedDate 

Day of week, date, or date exception restrictions.

This class can specify that something (typically a LocalTimeHMSRestricted or a LocalTimeRange) only applies on certain dates. This can be a mask of days of the week, optionally with specific dates that should be disallowed. Or you can schedule only on specific dates.

## Members

---

### LocalTimeDayOfWeek onlyOnDays 

Allow on that day of week if mask bit is set.

```
LocalTimeDayOfWeek onlyOnDays
```

---

### std::vector< LocalTimeYMD > onlyOnDates 

Dates to allow.

```
std::vector< LocalTimeYMD > onlyOnDates
```

---

### std::vector< LocalTimeYMD > exceptDates 

Dates to exclude.

```
std::vector< LocalTimeYMD > exceptDates
```

---

###  LocalTimeRestrictedDate::LocalTimeRestrictedDate() 

Create an empty restricted date object. It will return false for any date passed to isValid.

```
 LocalTimeRestrictedDate()
```

---

###  LocalTimeRestrictedDate::LocalTimeRestrictedDate(uint8_t mask) 

Create a date restricted object restricted to days of the week.

```
 LocalTimeRestrictedDate(uint8_t mask)
```

#### Parameters
* `mask` The days of the week to enable. Pass LocalTimeDayOfWeek::MASK_ALL to allow on every day (no restrictions)

---

###  LocalTimeRestrictedDate::LocalTimeRestrictedDate(uint8_t mask, std::initializer_list< const char * > onlyOnDates, std::initializer_list< const char * > exceptDates) 

Construct an object with an initializer list of strings.

```
 LocalTimeRestrictedDate(uint8_t mask, std::initializer_list< const char * > onlyOnDates, std::initializer_list< const char * > exceptDates)
```

#### Parameters
* `mask` mask value, see LocalTimeDayOfWeek for values 

* `onlyOnDates` Initializer list of strings of the form YYYY-MM-DD 

* `exceptDates` Initializer list of strings of the form YYYY-MM-DD

---

###  LocalTimeRestrictedDate::LocalTimeRestrictedDate(uint8_t mask, std::initializer_list< LocalTimeYMD > onlyOnDates, std::initializer_list< LocalTimeYMD > exceptDates) 

Construct an object with an initializer list of LocalTimeYMD objects.

```
 LocalTimeRestrictedDate(uint8_t mask, std::initializer_list< LocalTimeYMD > onlyOnDates, std::initializer_list< LocalTimeYMD > exceptDates)
```

#### Parameters
* `mask` mask value, see LocalTimeDayOfWeek for values 

* `onlyOnDates` Initializer list of LocalTimeYMD values 

* `exceptDates` Initializer list of LocalTimeYMD values

---

### LocalTimeRestrictedDate & LocalTimeRestrictedDate::withOnAllDays() 

Set the mask value to MASK_ALL. Does not change only on date or except on date lists.

```
LocalTimeRestrictedDate & withOnAllDays()
```

#### Returns
LocalTimeRestrictedDate&

---

### LocalTimeRestrictedDate & LocalTimeRestrictedDate::withOnlyOnDays(LocalTimeDayOfWeek value) 

Restrict to days of the week.

```
LocalTimeRestrictedDate & withOnlyOnDays(LocalTimeDayOfWeek value)
```

#### Parameters
* `value` A LocalTimeDayOfWeek object specifying the days of the week (mask bits for Sunday - Saturday) 

#### Returns
LocalTimeRestrictedDate&

A day of the week is allowed if the day of week mask bit is set. If a date is in the except dates list, then isValid return false. If a date is in the only on days mask OR only on dates list, then isValid will return true.

---

### LocalTimeRestrictedDate & LocalTimeRestrictedDate::withOnlyOnDays(uint8_t mask) 

Restrict to certain dates.

```
LocalTimeRestrictedDate & withOnlyOnDays(uint8_t mask)
```

#### Parameters
* `mask` Mask value, such as LocalTimeDayOfWeek::MASK_MONDAY

#### Returns
LocalTimeRestrictedDate&

A day of the week is allowed if the day of week mask bit is set. If a date is in the except dates list, then isValid return false. If a date is in the only on days mask OR only on dates list, then isValid will return true.

---

### LocalTimeRestrictedDate & LocalTimeRestrictedDate::withOnlyOnDates(std::initializer_list< const char * > dates) 

Restrict to certain dates.

```
LocalTimeRestrictedDate & withOnlyOnDates(std::initializer_list< const char * > dates)
```

#### Parameters
* `dates` A {} list of strings of the form YYYY-MM-DD. No other date formats are allowed! 

#### Returns
LocalTimeRestrictedDate&

If a date is in the except dates list, then isValid return false. If a date is in the only on days mask OR only on dates list, then isValid will return true.

---

### LocalTimeRestrictedDate & LocalTimeRestrictedDate::withOnlyOnDates(std::initializer_list< LocalTimeYMD > dates) 

Restrict to certain dates.

```
LocalTimeRestrictedDate & withOnlyOnDates(std::initializer_list< LocalTimeYMD > dates)
```

#### Parameters
* `dates` A {} list of LocalTimeYMD objects 

#### Returns
LocalTimeRestrictedDate&

If a date is in the except dates list, then isValid return false. If a date is in the only on days mask OR only on dates list, then isValid will return true.

---

### LocalTimeRestrictedDate & LocalTimeRestrictedDate::withExceptDates(std::initializer_list< const char * > dates) 

Dates that will always return false for isValid.

```
LocalTimeRestrictedDate & withExceptDates(std::initializer_list< const char * > dates)
```

#### Parameters
* `dates` A {} list of strings of the form YYYY-MM-DD. No other date formats are allowed! 

#### Returns
LocalTimeRestrictedDate&

If a date is in the except dates list, then isValid return false. If a date is in the only on days mask OR only on dates list, then isValid will return true.

---

### LocalTimeRestrictedDate & LocalTimeRestrictedDate::withExceptDates(std::initializer_list< LocalTimeYMD > dates) 

Dates that will always return false for isValid.

```
LocalTimeRestrictedDate & withExceptDates(std::initializer_list< LocalTimeYMD > dates)
```

#### Parameters
* `dates` A {} list of LocalTimeYMD objects 

#### Returns
LocalTimeRestrictedDate&

If a date is in the except dates list, then isValid return false. If a date is in the only on days mask OR only on dates list, then isValid will return true.

---

### bool LocalTimeRestrictedDate::isEmpty() const 

Returns true if onlyOnDays mask is 0 and the onlyOnDates and exceptDates lists are empty.

```
bool isEmpty() const
```

#### Returns
true 

#### Returns
false

---

### void LocalTimeRestrictedDate::clear() 

Clear all settings.

```
void clear()
```

---

### bool LocalTimeRestrictedDate::isValid(LocalTimeValue localTimeValue) const 

Returns true if a date is in the onlyOnDays or onlyOnDates list, and not in the exceptDates list.

```
bool isValid(LocalTimeValue localTimeValue) const
```

#### Parameters
* `localTimeValue` Date to check (local time) 

#### Returns
true 

#### Returns
false

---

### bool LocalTimeRestrictedDate::isValid(LocalTimeYMD ymd) const 

Returns true if a date is in the onlyOnDays or onlyOnDates list, and not in the exceptDates list.

```
bool isValid(LocalTimeYMD ymd) const
```

#### Parameters
* `ymd` Date to check (local time) 

#### Returns
true 

#### Returns
false

---

### bool LocalTimeRestrictedDate::inOnlyOnDates(LocalTimeYMD ymd) const 

Returns true of a date is in the onlyOnDates list.

```
bool inOnlyOnDates(LocalTimeYMD ymd) const
```

#### Parameters
* `ymd` 

#### Returns
true 

#### Returns
false

---

### bool LocalTimeRestrictedDate::inExceptDates(LocalTimeYMD ymd) const 

Returns true of a date is in the exceptDates list.

```
bool inExceptDates(LocalTimeYMD ymd) const
```

#### Parameters
* `ymd` 

#### Returns
true 

#### Returns
false

---

### void LocalTimeRestrictedDate::fromJson(JSONValue jsonObj) 

Fills in this object from JSON data.

```
void fromJson(JSONValue jsonObj)
```

#### Parameters
* `jsonObj` 

Keys:

* y (integer) mask value for onlyOnDays (optional)

* a (array) Array of YYYY-MM-DD value strings to allow (optional)

* x (array) Array of YYYY-MM-DD values to exclude (optional)

# class LocalTimeSchedule 

A complete time schedule.

A time schedule consists of minute multiples ("every 15 minutes"), optionally within a time range (all day, or from 09:00:00 to 17:00:00 local time, for example.

It can also have hour multiples, optionally in a time range, at a defined minute ("every 4 hours at :15 
past the hour").

Schedules can be at a specifc day week, with an ordinal (first Monday, last Friday) at a specific time, optionally with exceptions.

Schedules can be a specific day of the month (the 1st, the 5th of the month, the last day of the month, the second to last day of month).

It can also have any number of specific times in the day ("at 08:17:30 local time, 18:15:20 local time") every day, specific days of the week, on specific dates, or with date exceptions.

## Members

---

### String name 

Name of this schedule (optional, typically used with LocalTimeScheduleManager)

```
String name
```

---

### uint32_t flags 

Flags (optional, typically used with LocalTimeScheduleManager)

```
uint32_t flags
```

---

### std::vector< LocalTimeScheduleItem > scheduleItems 

LocalTimeSchedule items.

```
std::vector< LocalTimeScheduleItem > scheduleItems
```

---

###  LocalTimeSchedule::LocalTimeSchedule() 

Construct a new, empty schedule.

```
 LocalTimeSchedule()
```

---

### LocalTimeSchedule & LocalTimeSchedule::withMinuteOfHour(int increment, LocalTimeRange timeRange) 

Adds a minute multiple schedule in a time range.

```
LocalTimeSchedule & withMinuteOfHour(int increment, LocalTimeRange timeRange)
```

#### Parameters
* `increment` Number of minutes (must be 1 <= minutes <= 59). A value that is is divisible by is recommended. 

* `timeRange` When to apply this minute multiple and/or minute offset (optional)

This schedule publishes every n minutes within the hour. This really is every hour, not rolling, so you should use a value that 60 is divisible by (2, 3, 4, 5, 6, 10, 12, 15, 20, 30) otherwise there will be an inconsistent period at the top of the hour.

If you specify a time range that does not start at 00:00:00 you can customize which minute the schedule starts at. For example: `15, LocalTimeRange(LocalTimeHMS("00:05:00"), LocalTimeHMS("23:59:59")` will schedule every 15 minutes, but starting at 5 minutes past the hour, so 05:00, 20:00, 35:00, 50:00.

The largest value for hmsEnd of the time range is 23:59:59.

#### Returns
LocalTimeSchedule&

---

### LocalTimeSchedule & LocalTimeSchedule::withHourOfDay(int hourMultiple, LocalTimeRange timeRange) 

Adds multiple times periodically in a time range with an hour increment.

```
LocalTimeSchedule & withHourOfDay(int hourMultiple, LocalTimeRange timeRange)
```

#### Parameters
* `hourMultiple` Hours between items must be >= 1. For example: 2 = every other hour. 

* `timeRange` Time range to add items to. This is optional; if not specified then the entire day. Also is used to specify a minute offset.

#### Returns
LocalTimeSchedule&

Hours are per day, local time. For whole-day schedules, you will typically use a value that 24 is evenly divisible by (2, 3, 4, 6, 8, 12), because otherwise the time periods will brief unequal at midnight.

Also note that times are local, and take into account daylight saving. Thus during a time switch, the interval may end up being a different number of hours than specified. For example, if the times would have been 00:00 and 04:00, a hourMultiple of 4, and you do this over a spring forward, the actual number hours between 00:00 and 04:00 is 5 (at least in the US where DST starts at 2:00).

---

### LocalTimeSchedule & LocalTimeSchedule::withDayOfWeekOfMonth(int dayOfWeek, int instance, LocalTimeRange timeRange) 

Schedule an item on a specific instance of a day of week of the month.

```
LocalTimeSchedule & withDayOfWeekOfMonth(int dayOfWeek, int instance, LocalTimeRange timeRange)
```

#### Parameters
* `dayOfWeek` Day of week 0 = Sunday, 1 = Monday, ..., 6 = Saturday 

* `instance` 1 = first week, 2 = second week, ..., -1 = last week, -2 = 2nd to last week 

* `timeRange` Optional to restrict dates or to set a time for the item 

#### Returns
LocalTimeSchedule&

---

### LocalTimeSchedule & LocalTimeSchedule::withDayOfMonth(int dayOfMonth, LocalTimeRange timeRange) 

Schedule an item on a specific day of the month.

```
LocalTimeSchedule & withDayOfMonth(int dayOfMonth, LocalTimeRange timeRange)
```

#### Parameters
* `dayOfMonth` 1 = 1st, 2 = 2nd of the month, ..., -1 = last day of the month, -2 = second to last day of the month, ... 

* `timeRange` Optional to restrict dates or to set a time for the item 

#### Returns
LocalTimeSchedule&

---

### LocalTimeSchedule & LocalTimeSchedule::withTime(LocalTimeHMSRestricted hms) 

Add a scheduled item at a time in local time during the day.

```
LocalTimeSchedule & withTime(LocalTimeHMSRestricted hms)
```

#### Parameters
* `hms` The time in local time 00:00:00 to 23:59:59, optionally with date restrictions 

#### Returns
LocalTimeSchedule&

You can call this multiple times, and also combine it with minute multiple schedules.

---

### LocalTimeSchedule & LocalTimeSchedule::withTimes(std::initializer_list< LocalTimeHMSRestricted > timesParam) 

Add multiple scheduled items at a time in local time during the day.

```
LocalTimeSchedule & withTimes(std::initializer_list< LocalTimeHMSRestricted > timesParam)
```

#### Parameters
* `timesParam` an auto-initialized list of LocalTimeHMS objects 

#### Returns
LocalTimeSchedule&

You can call this multiple times, and also combine it with minute multiple schedules.

schedule.withTimes({LocalTimeHMS("06:00"), LocalTimeHMS("18:30")});

---

### LocalTimeSchedule & LocalTimeSchedule::withName(const char * name) 

Sets the name of this schedule (optional)

```
LocalTimeSchedule & withName(const char * name)
```

#### Parameters
* `name` 

#### Returns
LocalTimeSchedule&

---

### LocalTimeSchedule & LocalTimeSchedule::withFlags(uint32_t flags) 

Sets the flags for this schedule (optional)

```
LocalTimeSchedule & withFlags(uint32_t flags)
```

#### Parameters
* `flags` 

#### Returns
LocalTimeSchedule&

---

### bool LocalTimeSchedule::isEmpty() const 

Returns true if the schedule does not have any items in it.

```
bool isEmpty() const
```

#### Returns
true 

#### Returns
false

---

### void LocalTimeSchedule::clear() 

Clear the existing settings.

```
void clear()
```

---

### void LocalTimeSchedule::fromJson(const char * jsonStr) 

Set the schedule from a JSON string containing an array of objects.

```
void fromJson(const char * jsonStr)
```

#### Parameters
* `jsonStr` 

See the overload that takes a JSONValue if the JSON string has already been parsed.

---

### void LocalTimeSchedule::fromJson(JSONValue jsonArray) 

Set the schedule of this object from a JSONValue, typically the outer object.

```
void fromJson(JSONValue jsonArray)
```

#### Parameters
* `jsonArray` A JSONValue containing an array of objects

Array of LocalTimeScheduleItem objects:

* mh (integer): Minute of hour (takes place of setting m and i separately)

* hd (integer): Hour of day (takes place of setting m and i separately)

* dw (integer): Day of week (takes place of setting m and i separately)

* dm (integer): Day of month (takes place of setting m and i separately)

* tm (string) Time string in HH:MM:SS format (can omit MM and SS parts, see LocalTimeHMS) for TIME items

* m (integer) type of multiple (optional if mm, )

* i (integer) increment

* f (integer) flag bits (optional)

* s (string) The start time (HH:MM:SS format, can omit MM or SS) [from LocalTimeRange via LocalTimeRange]

* e (string) The end time (HH:MM:SS format, can omit MM or SS) [from LocalTimeRange via LocalTimeRange]

* y (integer) mask value for onlyOnDays [from LocalTimeRestrictedDate via LocalTimeRange]

* a (array) Array of YYYY-MM-DD value strings to allow [from LocalTimeRestrictedDate via LocalTimeRange]

* x (array) Array of YYYY-MM-DD values to exclude [from LocalTimeRestrictedDate via LocalTimeRange]

---

### bool LocalTimeSchedule::getNextScheduledTime(LocalTimeConvert & conv) const 

Update the conv object to point at the next schedule item.

```
bool getNextScheduledTime(LocalTimeConvert & conv) const
```

#### Parameters
* `conv` LocalTimeConvert object, may be modified 

#### Returns
true if there is an item available or false if not. if false, conv will be unchanged.

This method finds closest scheduled time for this objecvt, if it's in the near future. The LocalTime::instance().getScheduleLookaheadDays() setting determines how far in the future to check; the default is 3 days. The way schedules work each day needs to be checked to make sure all of the constraints are met, so long look-aheads are computationally intensive. This is not normally an issue, because the idea is that you'll wake from sleep or check the schedule at least every few days, at which point the new schedule may be available.

---

### bool LocalTimeSchedule::getNextScheduledTime(LocalTimeConvert & conv, std::function< bool(LocalTimeScheduleItem &item)> filter) const 

Update the conv object to point at the next schedule item.

```
bool getNextScheduledTime(LocalTimeConvert & conv, std::function< bool(LocalTimeScheduleItem &item)> filter) const
```

#### Parameters
* `conv` LocalTimeConvert object, may be modified 

* `filter` A function to determine, for each schedule item, if it should be tested 

#### Returns
true if there is an item available or false if not. if false, conv will be unchanged.

This method finds closest scheduled time for this objecvt, if it's in the near future. The LocalTime::instance().getScheduleLookaheadDays() setting determines how far in the future to check; the default is 3 days. The way schedules work each day needs to be checked to make sure all of the constraints are met, so long look-aheads are computationally intensive. This is not normally an issue, because the idea is that you'll wake from sleep or check the schedule at least every few days, at which point the new schedule may be available.

The filter function or lambda has this prototype:

bool filterCallback(LocalTimeScheduleItem &item)

If should return true to check this item, or false to skip this item for schedule checking.

# class LocalTimeScheduleItem 

A single item in a schedule, such as minute of hour, hour of day, or a specific time.

## Members

---

### LocalTimeRange timeRange 

Range of local time, inclusive.

```
LocalTimeRange timeRange
```

---

### int increment 

Increment value, or sometimes ordinal value.

```
int increment
```

---

### int dayOfWeek 

Used for DAY_OF_WEEK_OF_MONTH only.

```
int dayOfWeek
```

---

### int flags 

Optional scheduling flags.

```
int flags
```

---

### String name 

Optional name.

```
String name
```

---

### ScheduleItemType scheduleItemType 

The type of schedule item.

```
ScheduleItemType scheduleItemType
```

---

###  LocalTimeScheduleItem::LocalTimeScheduleItem() 

Default constructor. Set increment and optionally timeRange to use.

```
 LocalTimeScheduleItem()
```

---

### bool LocalTimeScheduleItem::isValid() const 

Returns true ScheduleItemType is not NONE.

```
bool isValid() const
```

#### Returns
true 

#### Returns
false

This is used to check if an object was constructed by the default constructor and never set.

---

### time_t LocalTimeScheduleItem::getTimeSpan(const LocalTimeConvert & conv) const 

Get number of seconds in the time range at a given time.

```
time_t getTimeSpan(const LocalTimeConvert & conv) const
```

#### Parameters
* `conv` The timezone and date information for time span calculation 

#### Returns
time_t

The conv object is necessary because getTimeSpan takes into account daylight saving transitions. When springing forward to daylight saving, from 01:15:00 to 03:15:00 is only one hour because of the DST transition.

---

### bool LocalTimeScheduleItem::getNextScheduledTime(LocalTimeConvert & conv) const 

Update the conv object to point at the next schedule item.

```
bool getNextScheduledTime(LocalTimeConvert & conv) const
```

#### Parameters
* `conv` LocalTimeConvert object, may be modified 

#### Returns
true if there is an item available or false if not. if false, conv will be unchanged.

This method finds the next scheduled time of this item, if it's in the near future. The LocalTime::instance().getScheduleLookaheadDays() setting determines how far in the future to check; the default is 3 days. The way schedules work each day needs to be checked to make sure all of the constraints are met, so long look-aheads are computationally intensive. This is not normally an issue, because the idea is that you'll wake from sleep or check the schedule at least every few days, at which point the new schedule may be available.

---

### void LocalTimeScheduleItem::fromJson(JSONValue jsonObj) 

Creates an object from JSON.

```
void fromJson(JSONValue jsonObj)
```

#### Parameters
* `jsonObj` The schedule. This should be the object containing the values, not the array.

Keys:

* m (integer) ScheduleItemType (1 = minute of hour, 2 = hour of day, 3 = day of week, 4 = day of month)

* i (integer) increment or ordinal value

* d (integer) dayOfWeek value (optional, only used for DAY_OF_WEEK_OF_MONTH)

* f (integer) flag bits (optional)

* n (string) a user-defined name for this item (optional)

* s (string) The start time (HH:MM:SS format, can omit MM or SS) [from LocalTimeRange via LocalTimeRange]

* e (string) The end time (HH:MM:SS format, can omit MM or SS) [from LocalTimeRange via LocalTimeRange]

* y (integer) mask value for onlyOnDays [from LocalTimeRestrictedDate via LocalTimeRange]

* a (array) Array of YYYY-MM-DD value strings to allow [from LocalTimeRestrictedDate via LocalTimeRange]

* x (array) Array of YYYY-MM-DD values to exclude [from LocalTimeRestrictedDate via LocalTimeRange]

---

### enum ScheduleItemType 

Type of schedule item this is.

```
enum ScheduleItemType
```

 Values                         | Descriptions                                
--------------------------------|---------------------------------------------
NONE            | No schedule defined.
MINUTE_OF_HOUR            | Minute of the hour (1)
HOUR_OF_DAY            | Hour of day (2)
DAY_OF_WEEK_OF_MONTH            | The nth day of week of the month (3)
DAY_OF_MONTH            | Day of the month (4)
TIME            | Specific time (5) <br/>

# class LocalTimeScheduleManager 

Class for managing multiple named schedules.

This is used for the quick and full wake schedules, but can be extended for other uses.

## Members

---

### std::vector< LocalTimeSchedule > schedules 

Vector of all of the schedules. Names and flags are in the schedule object.

```
std::vector< LocalTimeSchedule > schedules
```

---

### time_t LocalTimeScheduleManager::getNextTimeByName(const char * name, const LocalTimeConvert & conv) 

Get the next scheduled time of the schedule with name "name".

```
time_t getNextTimeByName(const char * name, const LocalTimeConvert & conv)
```

#### Parameters
* `name` The name to look for (c string) 

* `conv` The LocalTimeConvert that contains the timezone information to use 

#### Returns
time_t Time of 0 if there is no schedule with that name

---

### time_t LocalTimeScheduleManager::getNextWake(const LocalTimeConvert & conv) const 

Get the wake of any type (quick or full)

```
time_t getNextWake(const LocalTimeConvert & conv) const
```

#### Parameters
* `conv` The LocalTimeConvert that contains the timezone information to use 

#### Returns
time_t Time of 0 if there is no schedule

---

### time_t LocalTimeScheduleManager::getNextFullWake(const LocalTimeConvert & conv) const 

Get the full wake, typically with a publish.

```
time_t getNextFullWake(const LocalTimeConvert & conv) const
```

#### Parameters
* `conv` The LocalTimeConvert that contains the timezone information to use 

#### Returns
time_t Time of 0 if there is no schedule

---

### time_t LocalTimeScheduleManager::getNextDataCapture(const LocalTimeConvert & conv) const 

Get the next scheduled data capture. Data capture schedules are a special class of quick wake that also runs while powered on.

```
time_t getNextDataCapture(const LocalTimeConvert & conv) const
```

#### Parameters
* `conv` 

#### Returns
time_t

---

### void LocalTimeScheduleManager::forEach(std::function< void(LocalTimeSchedule &schedule)> callback) 

Call a function or lambda for each schedule.

```
void forEach(std::function< void(LocalTimeSchedule &schedule)> callback)
```

#### Parameters
* `callback` Function or lambda to call.

The callback has this prototype:

void callback(LocalTimeSchedule &schedule)

---

### LocalTimeSchedule & LocalTimeScheduleManager::getScheduleByName(const char * name) 

Get a LocalTimeSchedule reference by name and creates it if it does not exist.

```
LocalTimeSchedule & getScheduleByName(const char * name)
```

#### Parameters
* `name` Name to get or create 

#### Returns
LocalTimeSchedule& Reference to the schedule

---

### void LocalTimeScheduleManager::setFromJsonObject(const JSONValue & obj) 

Set the schedules from a JSON object.

```
void setFromJsonObject(const JSONValue & obj)
```

#### Parameters
* `obj` 

Only the keys in obj that already exist as named schedules are processed! This allows a single object to contain both schedules and other settings. Plus, in order for a named schedule to be useful you probably need to have code to handle it, and this eliminates the need to pass the schedule flags in the JSON, since they should be constant depending on how the schedule is being used in the code, not by the schedule settings.

# class LocalTimeValue 

```
class LocalTimeValue
  : public tm
```  

Container for a local time value with accessors similar to the Wiring Time class.

Really just a C++ wrapper around struct tm with adjustments for weekday and month being 0-based in struct tm and 1-based in Wiring. Also tm_year being weird in struct tm.

If you want to format a time string, use the methods in LocalTimeConvert. The reason is that the LocalTimeValue is only the value container and doesn't know the current timezone offset for the local time.

## Members

---

### int LocalTimeValue::hour() const 

Returns the hour (0 - 23)

```
int hour() const
```

---

### int LocalTimeValue::hourFormat12() const 

Returns the hour (1 - 12) used in AM/PM mode.

```
int hourFormat12() const
```

---

### uint8_t LocalTimeValue::isAM() const 

Returns true if the time is in the AM (before noon)

```
uint8_t isAM() const
```

---

### uint8_t LocalTimeValue::isPM() const 

Returns true if the time is in the PM (>= 12:00:00 in 24-hour clock).

```
uint8_t isPM() const
```

---

### int LocalTimeValue::minute() const 

Returns the minute 0 - 59.

```
int minute() const
```

---

### int LocalTimeValue::second() const 

Returns the second 0 - 59.

```
int second() const
```

---

### int LocalTimeValue::day() const 

Returns the day of the month 1 - 31 (or less in some months)

```
int day() const
```

---

### int LocalTimeValue::weekday() const 

Returns the day of week 1 - 7 (Sunday = 1, Monday = 2, ..., Saturday = 7)

```
int weekday() const
```

Note: the underlying struct tm tm_wday is 0 - 6 (Sunday = 0, Monday = 1, ..., Saturday = 6) but Wiring uses 1 - 7 instead of 0 - 6.

---

### int LocalTimeValue::month() const 

Returns the month of the year 1 - 12 (1 = January, 2 = February, ...)

```
int month() const
```

Note: the underlying struct tm tm_mon is 0 - 11, but this returns the more common 1 - 12.

---

### int LocalTimeValue::year() const 

Returns the 4-digit year.

```
int year() const
```

---

### LocalTimeHMS LocalTimeValue::hms() const 

Gets the local time as a LocalTimeHMS object.

```
LocalTimeHMS hms() const
```

---

### void LocalTimeValue::setHMS(LocalTimeHMS hms) 

Sets the local time from a LocalTimeHMS object.

```
void setHMS(LocalTimeHMS hms)
```

---

### LocalTimeYMD LocalTimeValue::ymd() const 

Get the date portion of this object as a LocalTimeYMD.

```
LocalTimeYMD ymd() const
```

#### Returns
LocalTimeYMD

---

### time_t LocalTimeValue::toUTC(LocalTimePosixTimezone config) const 

Converts the specified local time into a UTC time.

```
time_t toUTC(LocalTimePosixTimezone config) const
```

There are some caveats to this that occur on when the time change occurs. On spring forward, there is an hour that doesn't technically map to a UTC time. For example, in the United States, 2:00 AM to 3:00 AM local time doesn't exist because at 2:00 AM local time, the local time springs forward to 3:00 AM.

When falling back, the hour from 1:00 AM to 2:00 AM is not unique, because it happens twice, once in DST before falling back, and a second time after falling back. The toUTC() function returns the second one that occurs in standard time.

---

### void LocalTimeValue::fromString(const char * str) 

Converts time from ISO-8601 format, ignoring the timezone.

```
void fromString(const char * str)
```

#### Parameters
* `str` The string to convert

The string should be of the form:

YYYY-MM-DDTHH:MM:SS

The T can be any single character, such as a space. For example:

2021-04-01 10:00:00

Any characters after the seconds are ignored.

---

### int LocalTimeValue::ordinal() const 

Returns which week of this day it is.

```
int ordinal() const
```

For example, if this day is a Friday and it's the first Friday of the month, then 1 is returned. If it's the second Friday, then 2 is returned.

(This is different than the week number of the month, which depends on which day you begin the week on.)

# class LocalTimeYMD 

Class for holding a year month day efficiently (4 bytes of storage)

There is no method to get this object from a time_t because time_t is at UTC and this object is intended to be the YMD at local time to correspond with a LocalTimeHMS. Thus it requires a LocalTimeConvert object, and there is a method to get a LocalTimeYMD from a LocalTimeConvert, not from this object.

## Members

---

### YMD ymd 

Packed value for year, month, and day of month (4 bytes)

```
YMD ymd
```

---

###  LocalTimeYMD::LocalTimeYMD() 

Default contructor with an invalid date (0000-00-00) set.

```
 LocalTimeYMD()
```

---

###  LocalTimeYMD::LocalTimeYMD(const char * s) 

Construct a YMD value from a string.

```
 LocalTimeYMD(const char * s)
```

#### Parameters
* `s` String, must be in YYYY-MM-DD format. No other formars are allowed!

---

###  LocalTimeYMD::LocalTimeYMD(const LocalTimeValue & value) 

Construct from a LocalTimeValue object.

```
 LocalTimeYMD(const LocalTimeValue & value)
```

#### Parameters
* `value` The date to copy from

---

### bool LocalTimeYMD::isEmpty() const 

Returns true if the date is uninitialized, as from the default constructor.

```
bool isEmpty() const
```

#### Returns
true 

#### Returns
false

---

### int LocalTimeYMD::getYear() const 

Get the year as a 4-digit year, for example: 2022.

```
int getYear() const
```

#### Returns
int The year, 4-digit

---

### void LocalTimeYMD::setYear(int year) 

Set the year value.

```
void setYear(int year)
```

#### Parameters
* `year` Year to set, can be several different values but typically is 4-digit year (2022, for example)

---

### int LocalTimeYMD::getMonth() const 

Get the month, 1 - 12 inclusive.

```
int getMonth() const
```

#### Returns
int month

---

### void LocalTimeYMD::setMonth(int month) 

Set the month, 1 - 12 inclusive.

```
void setMonth(int month)
```

#### Parameters
* `month` Month value

---

### int LocalTimeYMD::getDay() const 

Get the day of month, starting a 1.

```
int getDay() const
```

#### Returns
int

---

### void LocalTimeYMD::setDay(int day) 

Set the day of the month, staring at 1.

```
void setDay(int day)
```

#### Parameters
* `day` 

This method does not validate the date value, but you should avoid setting invalid date values since the results can be unpredictable.

---

### void LocalTimeYMD::fromTimeInfo(const struct tm * pTimeInfo) 

Copies the year, month, and day from a struct tm.

```
void fromTimeInfo(const struct tm * pTimeInfo)
```

#### Parameters
* `pTimeInfo` The pointer to a struct tm to copy the year, month, and day from.

The tm should be in local time.

---

### void LocalTimeYMD::fromLocalTimeValue(const LocalTimeValue & value) 

The LocalTimeValue to copy the year, month and day from.

```
void fromLocalTimeValue(const LocalTimeValue & value)
```

#### Parameters
* `value` Source of the year, month, and day values

Since LocalTimeValue contains a struct tm, this uses fromTimeInfo internally.

---

### void LocalTimeYMD::addDay(int numberOfDays) 

Add a number of days to the current YMD (updating month or year as necessary)

```
void addDay(int numberOfDays)
```

#### Parameters
* `numberOfDays` Number of days to add (positive) or subtract (negative)

Works correctly with leap years.

---

### int LocalTimeYMD::getDayOfWeek() const 

Get the day of the week, 0 = Sunday, 1 = Monday, 2 = Tuesday, ..., 6 = Saturday.

```
int getDayOfWeek() const
```

#### Returns
int the day of the week

---

### int LocalTimeYMD::compareTo(const LocalTimeYMD other) const 

Compare to another LocalTimeYMD object.

```
int compareTo(const LocalTimeYMD other) const
```

#### Parameters
* `other` 

#### Returns
int -1 if this is < other, 0 if this == other, or 1 if this > other.

---

### bool LocalTimeYMD::operator==(const LocalTimeYMD other) const 

Tests if this LocalTimeYMD is equal to other.

```
bool operator==(const LocalTimeYMD other) const
```

#### Parameters
* `other` 

#### Returns
true 

#### Returns
false

---

### bool LocalTimeYMD::operator!=(const LocalTimeYMD other) const 

Tests if this LocalTimeYMD is not equal to other.

```
bool operator!=(const LocalTimeYMD other) const
```

#### Parameters
* `other` 

#### Returns
true 

#### Returns
false

---

### bool LocalTimeYMD::operator<(const LocalTimeYMD other) const 

Tests if this LocalTimeYMD is less than other.

```
bool operator<(const LocalTimeYMD other) const
```

#### Parameters
* `other` 

#### Returns
true 

#### Returns
false

---

### bool LocalTimeYMD::operator<=(const LocalTimeYMD other) const 

Tests if this LocalTimeYMD is less than or equal to other.

```
bool operator<=(const LocalTimeYMD other) const
```

#### Parameters
* `other` 

#### Returns
true 

#### Returns
false

---

### bool LocalTimeYMD::operator>(const LocalTimeYMD other) const 

Tests if this LocalTimeYMD is greater than other.

```
bool operator>(const LocalTimeYMD other) const
```

#### Parameters
* `other` 

#### Returns
true 

#### Returns
false

---

### bool LocalTimeYMD::operator>=(const LocalTimeYMD other) const 

Tests if this LocalTimeYMD is greater than or equal to other.

```
bool operator>=(const LocalTimeYMD other) const
```

#### Parameters
* `other` 

#### Returns
true 

#### Returns
false

---

### bool LocalTimeYMD::parse(const char * s) 

Parse a YMD string in the format "YYYY-MD-DD". Only this format is supported!

```
bool parse(const char * s)
```

#### Parameters
* `s` 

#### Returns
true 

#### Returns
false

Do not use this function with other date formats like "mm/dd/yyyy"!

---

### String LocalTimeYMD::toString() const 

Converts the value to YYYY-MM-DD format as a String with leading zeros.

```
String toString() const
```

#### Returns
String

# struct LocalTimeYMD::YMD 

Packed structure to hold the YMD value.

## Members

---

### unsigned year 

Year, add 1900 (like struct tm)

```
unsigned year
```

---

### unsigned month 

Month, 1 - 12 (not like struct tm which is 0 - 11)

```
unsigned month
```

---

### unsigned day 

Day, 1 - 31 ish.

```
unsigned day
```

Generated by [Moxygen](https://sourcey.com/moxygen)