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

#ifndef SOLARIS_DATETIME_H
#define SOLARIS_DATETIME_H

#include <libsolaris/types.h>
#include <time.h>

typedef struct DateTime {
    s64 year;
    s64 month;
    s64 day;
    s64 hour;
    s64 minute;
    s64 second;
    s64 millisecond;
} DateTime;

typedef enum DateTimeUnit { UNIT_SECONDS, UNIT_MINUTES, UNIT_HOURS, UNIT_DAYS, UNIT_MONTHS, UNIT_YEARS } DateTimeUnit;

/// Add the specified amount to the DateTime instance
/// @param date The dateTime instance that is modified
/// @param amount The amount to add
/// @param unit The unit of time that is added
void date_time_add(DateTime* date, s64 amount, DateTimeUnit unit);

/// Retrieves the local DateTime
/// @return Local DateTime
DateTime date_time_now();

/// Retrieves the corresponding UTC DateTime
/// @return UTC DateTime
DateTime date_time_utc();

/// Retrieves the UTC DateTime which is relative to the specified local time
/// @param local_time The local time
/// @return UTC DateTime relative to the specified local time
///
/// @note There is no guarantee that the relative UTC DateTime is fully correct.
///       Internally, the current UTC offset is used, so there can be differences
///       when there would be a time shift.
DateTime date_time_utc_local(DateTime* local_time);

/// Calculates the difference of the two DateTimes in seconds
/// @param a First DateTime
/// @param b Second DateTime
/// @return Difference in seconds (b - a)
s64 date_time_difference(DateTime* a, DateTime* b);

/// Calculates the julian day number for the given date
/// @param date The date which is used for the calculation
/// @return Julian day number
f64 date_time_jdn(DateTime* date);

/// Calculates the mean julian day number for the given date
/// @param date The date which is used for the calculation
/// @return Mean julian day number
f64 date_time_mjdn(DateTime* date);

/// Calculates the julian centuries since J2000 for the specified date
/// @param date
/// @param floor
/// @return
f64 date_time_jc(DateTime* date, b8 floor);

/// Returns the bessel epoch for the given date
/// @param date Date for the calculation
/// @return Bessel epoch
f64 date_time_be(DateTime* date);

/// Calculates the greenwich mean sidereal time in math_degrees
/// @param utc The relative utc time for the calculation
/// @return Sidereal time in math_degrees
f64 date_time_gmst(DateTime* utc);

/// Computes the unix timestamp for the date
/// @param date The date
/// @return Unix timestamp
time_t date_time_unix(DateTime* date);

/// Equality check for DateTime
/// @param left The left DateTime
/// @param right The right DateTime
/// @return b8ean that states equality
b8 date_time_equal(DateTime* left, DateTime* right);

/// Inequality check for DateTime
/// @param left The left DateTime
/// @param right The right DateTime
/// @return b8ean that states inequality
b8 date_time_not_equal(DateTime* left, DateTime* right);

/// Less-than check for DateTime
/// @param left The left DateTime
/// @param right The right DateTime
/// @return b8ean that states if the left DateTime is less than the
///         right DateTime
b8 date_time_less_than(DateTime* left, DateTime* right);

/// Greater-than operator for DateTime
/// @param left The left DateTime
/// @param right The right DateTime
/// @return b8ean that states if the left DateTime is greater than
///         the right DateTime
b8 date_time_greater_than(DateTime* left, DateTime* right);

#endif// SOLARIS_DATETIME_H
