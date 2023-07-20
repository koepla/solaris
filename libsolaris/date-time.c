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

#include <time.h>

#include <libsolaris/date-time.h>
#include <libsolaris/math.h>
#include <libsolaris/utility/number-set.h>

/// Retrieves the number of days in the current month of the specified DateTime
s64 date_time_days_in_month(DateTime* date) {
    static u64 days30[] = { 4, 6, 9, 11 };
    static u64 days31[] = { 1, 3, 5, 7, 8, 10, 12 };

    u64 month = date->month;
    if (integer_set_contains(days30, ARRAY_SIZE(days30), month)) {
        return 30;
    }
    if (integer_set_contains(days31, ARRAY_SIZE(days31), month)) {
        return 31;
    }
    if (month == 2) {
        if (date->year % 4 == 0) {
            if (date->year % 100 == 0) {
                if (date->year % 400 == 0) {
                    return 29;
                }
                return 28;
            }
            return 29;
        }
        return 28;
    }
    return 0;
}

/// Adds the number of years to the specified DateTime
void date_time_add_years(DateTime* date, s64 years) {
    date->year += years;
}

/// Adds the number of months to the specified DateTime
void date_time_add_months(DateTime* date, s64 months) {
    s64 potentialYears = months / 12;
    if (math_abs((f64) (potentialYears)) > 0) {
        date_time_add_years(date, potentialYears);
        date_time_add_months(date, months % 12);
        return;
    }
    if (months > 0) {
        if (date->month + months > 12) {
            date_time_add_years(date, 1);
            date_time_add_months(date, months - 12);
        } else {
            date->month += months;
        }
    } else if (months < 0) {
        if (date->month + months < 1) {
            date_time_add_years(date, -1);
            date_time_add_months(date, months + 12);
        } else {
            date->month += months;
        }
    }
}

/// Adds the number of days to the specified DateTime
void date_time_add_days(DateTime* date, s64 days);

/// Adds the number of days to the specified DateTime with respect to the days in month
void date_time_add_days_month_days(DateTime* date, s64 days, s64 monthDays) {
    if (days > 0) {
        if (date->day + days > monthDays) {
            date_time_add_months(date, 1);
            date_time_add_days(date, days - monthDays);
        } else {
            date->day += days;
        }
    } else if (days < 0) {
        if (date->day + days < 1) {
            date_time_add_months(date, -1);
            date_time_add_days(date, days + date_time_days_in_month(date));
        } else {
            date->day += days;
        }
    }
}

/// Adds the number of days to the specified DateTime
void date_time_add_days(DateTime* date, s64 days) {
    date_time_add_days_month_days(date, days, date_time_days_in_month(date));
}

/// Adds the number of hours to the specified DateTime
void date_time_add_hours(DateTime* date, s64 hours) {
    s64 potential_days = hours / 24;
    if (math_abs((f64) (potential_days)) > 0) {
        date_time_add_days(date, potential_days);
        date_time_add_hours(date, hours % 24);
        return;
    }
    if (hours > 0) {
        if (date->hour + hours > 23) {
            date_time_add_days(date, 1);
            date_time_add_hours(date, hours - 24);
        } else {
            date->hour += hours;
        }
    } else if (hours < 0) {
        if (date->hour + hours < 0) {
            date_time_add_days(date, -1);
            date_time_add_hours(date, hours + 24);
        } else {
            date->hour += hours;
        }
    }
}

/// Adds the number of minutes to the specified DateTime
void date_time_add_minutes(DateTime* date, s64 minutes) {
    s64 potential_hours = minutes / 60;
    if (math_abs((f64) (potential_hours)) > 0) {
        date_time_add_hours(date, potential_hours);
        date_time_add_minutes(date, potential_hours % 60);
        return;
    }
    if (minutes > 0) {
        if (date->minute + minutes > 59) {
            date_time_add_hours(date, 1);
            date_time_add_minutes(date, minutes - 60);
        } else {
            date->minute += minutes;
        }
    } else if (minutes < 0) {
        if (date->minute + minutes < 0) {
            date_time_add_hours(date, -1);
            date_time_add_minutes(date, minutes + 60);
        } else {
            date->minute += minutes;
        }
    }
}

/// Adds the number of seconds to the specified DateTime
void date_time_add_seconds(DateTime* date, s64 seconds) {
    s64 potential_minutes = seconds / 60;
    if (math_abs((f64) (potential_minutes)) > 0) {
        date_time_add_minutes(date, potential_minutes);
        date_time_add_seconds(date, seconds % 60);
        return;
    }
    if (seconds > 0) {
        if (date->second + seconds > 59) {
            date_time_add_minutes(date, 1);
            date_time_add_seconds(date, seconds - 60);
        } else {
            date->second += seconds;
        }
    } else if (seconds < 0) {
        if (date->second + seconds < 0) {
            date_time_add_minutes(date, -1);
            date_time_add_seconds(date, seconds + 60);
        } else {
            date->second += seconds;
        }
    }
}

/// Add the specified amount to the DateTime instance
void date_time_add(DateTime* date, s64 amount, DateTimeUnit unit) {
    switch (unit) {
        case UNIT_SECONDS:
            date_time_add_seconds(date, amount);
            break;
        case UNIT_MINUTES:
            date_time_add_minutes(date, amount);
            break;
        case UNIT_HOURS:
            date_time_add_hours(date, amount);
            break;
        case UNIT_DAYS:
            date_time_add_days(date, amount);
            break;
        case UNIT_MONTHS:
            date_time_add_months(date, amount);
            break;
        case UNIT_YEARS:
            date_time_add_years(date, amount);
            break;
    }
}

/// Retrieves the local DateTime
DateTime date_time_now() {
    time_t now = time(NULL);
    struct tm* ltm = localtime(&now);

    DateTime result;
    result.year = (s64) (ltm->tm_year) + 1900;
    result.month = (s64) (ltm->tm_mon) + 1;
    result.day = (s64) (ltm->tm_mday);
    result.hour = (s64) (ltm->tm_hour);
    result.minute = (s64) (ltm->tm_min);
    result.second = (s64) (ltm->tm_sec);
    result.millisecond = 0;
    return result;
}

/// Retrieves the corresponding UTC DateTime
DateTime date_time_utc() {
    time_t now = time(NULL);
    struct tm* ltm = gmtime(&now);

    DateTime result;
    result.year = (s64) (ltm->tm_year) + 1900;
    result.month = (s64) (ltm->tm_mon) + 1;
    result.day = (s64) (ltm->tm_mday);
    result.hour = (s64) (ltm->tm_hour);
    result.minute = (s64) (ltm->tm_min);
    result.second = (s64) (ltm->tm_sec);
    result.millisecond = 0;
    return result;
}

/// Retrieves the UTC DateTime which is relative to the specified local time
DateTime date_time_utc_local(DateTime* local_time) {
    DateTime now = date_time_now();
    DateTime utc = date_time_utc();
    DateTime result = *local_time;
    date_time_add_seconds(&result, date_time_difference(&now, &utc));
    return result;
}

/// Calculates the difference of the two DateTimes in seconds
s64 date_time_difference(DateTime* a, DateTime* b) {
    return date_time_unix(b) - date_time_unix(a);
}

/// Calculates the julian day number for the given date
f64 date_time_jdn(DateTime* date) {
    return date_time_mjdn(date) + 2400000.5;
}

/// Calculates the mean julian day number for the given date
f64 date_time_mjdn(DateTime* date) {
    if (date->month <= 2) {
        date->month += 12;
        --date->year;
    }

    s64 calendar_adjust;
    if (10000 * date->year + 100 * date->month + date->day <= 15821004) {
        // julian calender
        calendar_adjust = (-2 + ((date->year + 4716) / 4) - 1179);
    } else {
        // gregorian calender
        calendar_adjust = (date->year / 400 - date->year / 100 + date->year / 4);
    }

    f64 hour = (f64) date->hour + (f64) date->minute / 60.0;
    hour += (f64) date->second / 3.6e3 + (f64) date->millisecond / 3.6e6;

    f64 mjdn = 365.0 * (f64) date->year - 679004.0 + (f64) calendar_adjust;
    mjdn += math_floor(30.6001 * (f64) (date->month + 1)) + (f64) date->day + (f64) hour / 24.0;
    return mjdn;
}

/// Calculates the julian centuries since J2000 for the specified date
f64 date_time_jc(DateTime* date, b8 floor) {
    f64 jdn = floor ? math_floor(date_time_jdn(date)) : date_time_jdn(date);
    return (jdn - 2451545.0) / 36525.0;
}

/// Returns the bessel epoch for the given date
f64 date_time_be(DateTime* date) {
    return 1900.0 + (date_time_jdn(date) - 2415020.31352) / 365.242198781;
}

/// Calculates the greenwich mean sidereal time in math_degrees
f64 date_time_gmst(DateTime* utc) {
    f64 mjdn = date_time_mjdn(utc);
    f64 mjdn_floor = math_floor(mjdn);
    f64 ut = SECONDS_PER_DAY * (mjdn - mjdn_floor);
    f64 t = (mjdn - 51544.5) / 36525.0;
    f64 t0 = (mjdn_floor - 51544.5) / 36525.0;

    f64 gmst = 24110.54841 + 8640184.812866 * t0 + 1.0027379093 * ut + (0.093104 - 6.2e-6 * t) * t * t;
    return math_modulo(math_degrees((PI2 / SECONDS_PER_DAY) * math_modulo(gmst, SECONDS_PER_DAY)), 360.0);
}

/// Computes the unix timestamp for the date
time_t date_time_unix(DateTime* date) {
    time_t raw_time = 0;
    time(&raw_time);
    struct tm* time_info = localtime(&raw_time);
    time_info->tm_year = (int) (date->year - 1900);
    time_info->tm_mon = (int) (date->month - 1);
    time_info->tm_mday = (int) (date->day);
    time_info->tm_hour = (int) (date->hour);
    time_info->tm_min = (int) (date->minute);
    time_info->tm_sec = (int) (date->second);
    return mktime(time_info);
}

/// Equality check for DateTime
b8 date_time_equal(DateTime* left, DateTime* right) {
    return left->second == right->second && left->minute == right->minute && left->hour == right->hour &&
           left->day == right->day && left->month == right->month && left->year == right->year;
}

/// Inequality check for DateTime
b8 date_time_not_equal(DateTime* left, DateTime* right) {
    return !date_time_equal(left, right);
}

/// Less-than operator for DateTime
b8 date_time_less_than(DateTime* left, DateTime* right) {
    if (left->year < right->year) {
        return true;
    }
    if (left->year > right->year) {
        return false;
    }
    if (left->month < right->month) {
        return true;
    }
    if (left->month > right->month) {
        return false;
    }
    if (left->day < right->day) {
        return true;
    }
    if (left->day > right->day) {
        return false;
    }
    if (left->hour < right->hour) {
        return true;
    }
    if (left->hour > right->hour) {
        return false;
    }
    if (left->minute < right->minute) {
        return true;
    }
    if (left->minute > right->minute) {
        return false;
    }
    if (left->second < right->second) {
        return true;
    }
    if (left->second > right->second) {
        return false;
    }
    return false;
}

/// Greater-than operator for DateTime
b8 date_time_greater_than(DateTime* left, DateTime* right) {
    return date_time_less_than(right, left);
}