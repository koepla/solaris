//
// MIT License
//
// Copyright (c) 2023 Elias Engelbert Plank
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef SOLARIS_TIME_H
#define SOLARIS_TIME_H

#include <time.h>

#include <solaris/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Time {
    s64 year;
    s64 month;
    s64 day;
    s64 hour;
    s64 minute;
    s64 second;
    s64 millisecond;
} Time;

typedef enum TimeUnit { UNIT_SECONDS, UNIT_MINUTES, UNIT_HOURS, UNIT_DAYS, UNIT_MONTHS, UNIT_YEARS } TimeUnit;

/// Add the specified amount to the DateTime instance
/// @param date The dateTime instance that is modified
/// @param amount The amount to add
/// @param unit The unit of time that is added
SOLARIS_API void time_add(Time* date, s64 amount, TimeUnit unit);

/// Retrieves the local DateTime
/// @return Local DateTime
SOLARIS_API Time time_now(void);

/// Retrieves the corresponding UTC DateTime
/// @return UTC DateTime
SOLARIS_API Time time_utc(void);

/// Retrieves the UTC DateTime which is relative to the specified local time
/// @param local_time The local time
/// @return UTC DateTime relative to the specified local time
///
/// @note There is no guarantee that the relative UTC DateTime is fully correct.
///       Internally, the current UTC offset is used, so there can be differences
///       when there would be a time shift.
SOLARIS_API Time time_utc_local(Time* local_time);

/// Calculates the difference of the two DateTimes in seconds
/// @param a First DateTime
/// @param b Second DateTime
/// @return Difference in seconds (b - a)
SOLARIS_API s64 time_difference(Time* a, Time* b);

/// Calculates the julian day number for the given date
/// @param date The date which is used for the calculation
/// @return Julian day number
SOLARIS_API f64 time_jdn(Time* date);

/// Calculates the mean julian day number for the given date
/// @param date The date which is used for the calculation
/// @return Mean julian day number
SOLARIS_API f64 time_mjdn(Time* date);

/// Calculates the julian centuries since J2000 for the specified date
/// @param date
/// @param floor
/// @return
SOLARIS_API f64 time_jc(Time* date, b8 floor);

/// Returns the bessel epoch for the given date
/// @param date Date for the calculation
/// @return Bessel epoch
SOLARIS_API f64 time_be(Time* date);

/// Calculates the greenwich mean sidereal time in math_degrees
/// @param utc The relative utc time for the calculation
/// @return Sidereal time in math_degrees
SOLARIS_API f64 time_gmst(Time* utc);

/// Computes the unix timestamp for the date
/// @param date The date
/// @return Unix timestamp
SOLARIS_API time_t time_unix(Time* date);

/// Checks if the specified date is valid
/// @param date The date
/// @return Boolean that states whether the date is valid
SOLARIS_API b8 time_valid(Time* date);

/// Equality check for DateTime
/// @param left The left DateTime
/// @param right The right DateTime
/// @return Boolean that states equality
SOLARIS_API b8 time_equal(Time* left, Time* right);

/// Inequality check for DateTime
/// @param left The left DateTime
/// @param right The right DateTime
/// @return Boolean that states inequality
SOLARIS_API b8 time_not_equal(Time* left, Time* right);

/// Less-than check for DateTime
/// @param left The left DateTime
/// @param right The right DateTime
/// @return Boolean that states if the left DateTime is less than the
///         right DateTime
SOLARIS_API b8 time_less_than(Time* left, Time* right);

/// Greater-than operator for DateTime
/// @param left The left DateTime
/// @param right The right DateTime
/// @return Boolean that states if the left DateTime is greater than
///         the right DateTime
SOLARIS_API b8 time_greater_than(Time* left, Time* right);

#ifdef __cplusplus
}
#endif

#endif// SOLARIS_TIME_H
