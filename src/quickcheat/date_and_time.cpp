
// https://en.cppreference.com/w/cpp/chrono
// https://en.cppreference.com/w/cpp/chrono#Calendar
// https://github.com/HowardHinnant/date
// https://meetingcpp.com/mcpp/slides/2019/Hinnant.pdf

#include <catch2/catch.hpp>
#include <date/date.h>
#include <date/tz.h>
#include <chrono>

namespace {

    namespace stdnext {

        namespace chrono {

            using namespace std::chrono;

#if __cpp_lib_chrono < 201907
            using namespace date;
#endif

        } // namespace chrono

        namespace chrono_literals {

            using namespace std::chrono_literals;

#if __cpp_lib_chrono < 201907
            using namespace date::literals;
#endif

        } // namespace chrono_literals

    } // namespace stdnext

    using namespace stdnext::chrono_literals;

    TEST_CASE("dt points and durations", "[dt]")
    {
        namespace dt = stdnext::chrono;

        const auto timepoint_now = dt::system_clock::now();

        const auto timepoint_in_the_future = timepoint_now + 1h + 5min + 35s;

        const auto timepoint_in_the_past = timepoint_now - (2h + 20min + 10s);

        const auto duration_between_past_and_future = timepoint_in_the_future - timepoint_in_the_past;

        REQUIRE(duration_between_past_and_future == (3h + 25min + 45s));

        const auto duration_with_microsecond_precision = 5h + 12min + 47s + 100us;

        REQUIRE(duration_with_microsecond_precision.count() == (5 * 60 * 60 + 12 * 60 + 47) * 1'000'000LL + 100LL);

        // No explicit cast needed because
        // duration_with_nanosecond_precision has a greater precision than duration_with_microsecond_precision
        const dt::nanoseconds duration_with_nanosecond_precision = duration_with_microsecond_precision;

        REQUIRE(duration_with_nanosecond_precision.count() ==
                ((5 * 60 * 60 + 12 * 60 + 47) * 1'000'000LL + 100LL) * 1000LL);

        // Explicit duration_cast needed because
        // duration_with_minute_precision has a lower precision than duration_with_microsecond_precision
        const dt::minutes duration_with_minute_precision =
            dt::duration_cast<dt::minutes>(duration_with_microsecond_precision);

        REQUIRE(duration_with_minute_precision.count() == 5 * 60 + 12);
    }

    TEST_CASE("calendar dates", "[date]")
    {
        namespace dt = stdnext::chrono;

        // Get utc date&time with precision of the OS clock (maybe millisecond, or microsecond, or...)
        const auto utc_datetimepoint_today = dt::system_clock::now();

        // Get utc date with day precision (lose time information, but useful for day-based date computations)
        const auto utc_datepoint_today_serial = dt::floor<dt::days>(utc_datetimepoint_today);

        // Get ymd fields of utc date
        const dt::year_month_day utc_datepoint_today_fields{utc_datepoint_today_serial};

        REQUIRE(utc_datepoint_today_fields.year() == 2020_y);

        // Get utc date of 365 days in the future
        const auto utc_datepoint_today_plus365_serial = utc_datepoint_today_serial + dt::days{365};

        // Get ymd fields of utc date of 365 days in the future
        const dt::year_month_day utc_datepoint_today_plus365_fields{utc_datepoint_today_plus365_serial};

        REQUIRE(utc_datepoint_today_plus365_fields.year() == 2021_y);

        // Can construct a bad date
        const date::year_month_day bad_date = 2020_y / dt::February / 30_d;

        // But can check whether it is bad or not
        REQUIRE(!bad_date.ok());

        // Can still adjust the bad date to overflow to the first day of next month
        const date::year_month_day good_date = date::year_month_day{date::sys_days{bad_date}};

        REQUIRE(good_date == 2020_y / dt::March / 1);

        // Can infer the day of the week from a date
        const dt::weekday good_date_weekday = dt::year_month_weekday{good_date}.weekday();

        REQUIRE(good_date_weekday == dt::Sunday);
    }

#if !defined(DISABLED_DATE_TZ_ON_LINUX)
    TEST_CASE("time zones", "[date][time][!hide]")
    {
        namespace dt = stdnext::chrono;

        // Can obtain a zoned date-time from a utc date-time and a time zone pointer
        try
        {
            const auto utc_datetimepoint_today = dt::system_clock::now();

            const dt::zoned_time currentzone_datetimepoint_today{dt::current_zone(), utc_datetimepoint_today};

            const auto local_datetimepoint_today = currentzone_datetimepoint_today.get_local_time();

            const auto hours_between_utc_and_local = dt::duration_cast<dt::hours>(
                local_datetimepoint_today.time_since_epoch() - utc_datetimepoint_today.time_since_epoch());

            REQUIRE((hours_between_utc_and_local == 1h /*winter*/ || hours_between_utc_and_local == 2h /*summer*/));
        }
        catch (const std::exception& e)
        {
            WARN(
                "You should download a timezone database from \"http://www.iana.org/time-zones\" and uncompress it "
                "here: \"%homedrive%\\%homepath%\\downloads\\tzdata\"");
        }
    }
#endif

} // namespace
