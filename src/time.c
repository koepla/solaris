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

#include <solaris/math.h>
#include <solaris/time.h>

/// Checks if the specified integer set contains the candidate
static b8 integer_set_contains(u64 const *const numbers, usize const count, u64 const candidate) {
    for (usize i = 0; i < count; ++i) {
        if (numbers[i] == candidate) {
            return true;
        }
    }
    return false;
}


/// Retrieves the number of days in the current month of the specified DateTime
s64 time_days_in_month(Time const *const date) {
    static u64 days30[] = { 4, 6, 9, 11 };
    static u64 days31[] = { 1, 3, 5, 7, 8, 10, 12 };

    u64 const month = date->month;
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
void time_add_years(Time *date, s64 const years) {
    date->year += years;
}

/// Adds the number of months to the specified DateTime
void time_add_months(Time *date, s64 const months) {
    s64 const potentialYears = months / 12;
    if (math_abs((f64) (potentialYears)) > 0) {
        time_add_years(date, potentialYears);
        time_add_months(date, months % 12);
        return;
    }
    if (months > 0) {
        if (date->month + months > 12) {
            time_add_years(date, 1);
            time_add_months(date, months - 12);
        } else {
            date->month += months;
        }
    } else if (months < 0) {
        if (date->month + months < 1) {
            time_add_years(date, -1);
            time_add_months(date, months + 12);
        } else {
            date->month += months;
        }
    }
}

/// Adds the number of days to the specified DateTime
void time_add_days(Time *date, s64 days);

/// Adds the number of days to the specified DateTime with respect to the days in month
void time_add_days_with_month_days(Time *date, s64 const days, s64 const monthDays) {
    if (days > 0) {
        if (date->day + days > monthDays) {
            time_add_months(date, 1);
            time_add_days(date, days - monthDays);
        } else {
            date->day += days;
        }
    } else if (days < 0) {
        if (date->day + days < 1) {
            time_add_months(date, -1);
            time_add_days(date, days + time_days_in_month(date));
        } else {
            date->day += days;
        }
    }
}

/// Adds the number of days to the specified DateTime
void time_add_days(Time *date, s64 const days) {
    time_add_days_with_month_days(date, days, time_days_in_month(date));
}

/// Adds the number of hours to the specified DateTime
void time_add_hours(Time *date, s64 const hours) {
    s64 const potential_days = hours / 24;
    if (math_abs((f64) (potential_days)) > 0) {
        time_add_days(date, potential_days);
        time_add_hours(date, hours % 24);
        return;
    }
    if (hours > 0) {
        if (date->hour + hours > 23) {
            time_add_days(date, 1);
            time_add_hours(date, hours - 24);
        } else {
            date->hour += hours;
        }
    } else if (hours < 0) {
        if (date->hour + hours < 0) {
            time_add_days(date, -1);
            time_add_hours(date, hours + 24);
        } else {
            date->hour += hours;
        }
    }
}

/// Adds the number of minutes to the specified DateTime
void time_add_minutes(Time *date, s64 const minutes) {
    s64 const potential_hours = minutes / 60;
    if (math_abs((f64) (potential_hours)) > 0) {
        time_add_hours(date, potential_hours);
        time_add_minutes(date, potential_hours % 60);
        return;
    }
    if (minutes > 0) {
        if (date->minute + minutes > 59) {
            time_add_hours(date, 1);
            time_add_minutes(date, minutes - 60);
        } else {
            date->minute += minutes;
        }
    } else if (minutes < 0) {
        if (date->minute + minutes < 0) {
            time_add_hours(date, -1);
            time_add_minutes(date, minutes + 60);
        } else {
            date->minute += minutes;
        }
    }
}

/// Adds the number of seconds to the specified DateTime
void time_add_seconds(Time *date, s64 const seconds) {
    s64 const potential_minutes = seconds / 60;
    if (math_abs((f64) (potential_minutes)) > 0) {
        time_add_minutes(date, potential_minutes);
        time_add_seconds(date, seconds % 60);
        return;
    }
    if (seconds > 0) {
        if (date->second + seconds > 59) {
            time_add_minutes(date, 1);
            time_add_seconds(date, seconds - 60);
        } else {
            date->second += seconds;
        }
    } else if (seconds < 0) {
        if (date->second + seconds < 0) {
            time_add_minutes(date, -1);
            time_add_seconds(date, seconds + 60);
        } else {
            date->second += seconds;
        }
    }
}

/// Add the specified amount to the DateTime instance
void time_add(Time *date, s64 const amount, TimeUnit const unit) {
    switch (unit) {
        case UNIT_SECONDS:
            time_add_seconds(date, amount);
            break;
        case UNIT_MINUTES:
            time_add_minutes(date, amount);
            break;
        case UNIT_HOURS:
            time_add_hours(date, amount);
            break;
        case UNIT_DAYS:
            time_add_days(date, amount);
            break;
        case UNIT_MONTHS:
            time_add_months(date, amount);
            break;
        case UNIT_YEARS:
            time_add_years(date, amount);
            break;
    }
}

/// Retrieves the local DateTime
Time time_now(void) {
    time_t const now = time(nil);
    struct tm const *ltm = localtime(&now);

    Time result;
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
Time time_utc(void) {
    time_t const now = time(nil);
    struct tm const *ltm = gmtime(&now);

    Time result;
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
Time time_utc_local(Time const *const local_time) {
    Time const now = time_now();
    Time const utc = time_utc();
    Time result = *local_time;
    time_add_seconds(&result, time_difference(&now, &utc));
    return result;
}

/// Calculates the difference of the two DateTimes in seconds
s64 time_difference(Time const *const a, Time const *const b) {
    return time_unix(b) - time_unix(a);
}

/// Calculates the julian day number for the given date
f64 time_jdn(Time const *const date) {
    return time_mjdn(date) + 2400000.5;
}

/// Calculates the mean julian day number for the given date
f64 time_mjdn(Time const *const date) {
    Time it = *date;
    if (it.month <= 2) {
        it.month += 12;
        --it.year;
    }

    s64 calendar_adjust;
    if (10000 * it.year + 100 * it.month + it.day <= 15821004) {
        // julian calender
        calendar_adjust = (-2 + ((it.year + 4716) / 4) - 1179);
    } else {
        // gregorian calender
        calendar_adjust = (it.year / 400 - it.year / 100 + it.year / 4);
    }

    f64 hour = (f64) it.hour + (f64) it.minute / 60.0;
    hour += (f64) it.second / 3.6e3 + (f64) it.millisecond / 3.6e6;

    f64 mjdn = 365.0 * (f64) it.year - 679004.0 + (f64) calendar_adjust;
    mjdn += math_floor(30.6001 * (f64) (it.month + 1)) + (f64) it.day + (f64) hour / 24.0;
    return mjdn;
}

/// Calculates the julian centuries since J2000 for the specified date
f64 time_jc(Time const *const date, b8 const floor) {
    f64 const jdn = floor ? math_floor(time_jdn(date)) : time_jdn(date);
    return (jdn - 2451545.0) / 36525.0;
}

/// Returns the bessel epoch for the given date
f64 time_be(Time const *const date) {
    return 1900.0 + (time_jdn(date) - 2415020.31352) / 365.242198781;
}

/// Calculates the greenwich mean sidereal time in math_degrees
f64 time_gmst(Time const *const utc) {
    f64 const mjdn = time_mjdn(utc);
    f64 const mjdn_floor = math_floor(mjdn);
    f64 const ut = SECONDS_PER_DAY * (mjdn - mjdn_floor);
    f64 const t = (mjdn - 51544.5) / 36525.0;
    f64 const t0 = (mjdn_floor - 51544.5) / 36525.0;

    f64 const gmst = 24110.54841 + 8640184.812866 * t0 + 1.0027379093 * ut + (0.093104 - 6.2e-6 * t) * t * t;
    return math_modulo(math_degrees((PI2 / SECONDS_PER_DAY) * math_modulo(gmst, SECONDS_PER_DAY)), 360.0);
}

/// Computes the unix timestamp for the date
time_t time_unix(Time const *const date) {
    time_t raw_time = 0;
    time(&raw_time);
    struct tm *time_info = localtime(&raw_time);
    time_info->tm_year = (int) (date->year - 1900);
    time_info->tm_mon = (int) (date->month - 1);
    time_info->tm_mday = (int) (date->day);
    time_info->tm_hour = (int) (date->hour);
    time_info->tm_min = (int) (date->minute);
    time_info->tm_sec = (int) (date->second);
    return mktime(time_info);
}

/// Checks if the specified date is valid
b8 time_valid(Time const *const date) {
    if (date->year < 0 || date->year > 9999) {
        return false;
    }
    if (date->month < 0 || date->month > 12) {
        return false;
    }
    if (date->day < 1 || date->day > time_days_in_month(date)) {
        return false;
    }
    if (date->hour < 0 || date->hour > 23) {
        return false;
    }
    if (date->minute < 0 || date->minute > 59) {
        return false;
    }
    if (date->second < 0 || date->second > 59) {
        return false;
    }
    if (date->millisecond < 0) {
        return false;
    }
    return true;
}

/// Compares the two integers
static s64 compare_s64(s64 const a, s64 const b) {
    return a - b;
}

// Compares the two times
s64 time_compare(Time const *const left, Time const *const right) {
    s64 cmp = compare_s64(left->year, right->year);
    if (cmp != 0) {
        return cmp;
    }
    cmp = compare_s64(left->month, right->month);
    if (cmp != 0) {
        return cmp;
    }
    cmp = compare_s64(left->day, right->day);
    if (cmp != 0) {
        return cmp;
    }
    cmp = compare_s64(left->hour, right->hour);
    if (cmp != 0) {
        return cmp;
    }
    cmp = compare_s64(left->minute, right->minute);
    if (cmp != 0) {
        return cmp;
    }
    cmp = compare_s64(left->second, right->second);
    if (cmp != 0) {
        return cmp;
    }
    return compare_s64(left->millisecond, right->millisecond);
}

/// Equality check for DateTime
b8 time_equal(Time const *const left, Time const *const right) {
    return time_compare(left, right) == 0;
}

/// Inequality check for DateTime
b8 time_not_equal(Time const *const left, Time const *const right) {
    return !time_equal(left, right);
}

/// Less-than operator for DateTime
b8 time_lt(Time const *const left, Time const *const right) {
    return time_compare(left, right) < 0;
}

/// Less-than or equal operator for DateTime
b8 time_le(Time const *const left, Time const *const right) {
    return time_compare(left, right) <= 0;
}

/// Greater-than operator for DateTime
b8 time_gt(Time const *const left, Time const *const right) {
    return time_compare(left, right) > 0;
}

/// Greater-than or equal operator for DateTime
b8 time_ge(Time const *const left, Time const *const right) {
    return time_compare(left, right) >= 0;
}
