//#include "time/Clock.hpp"
//
//#include <time.h>
//#include <unistd.h>
//
//TimePoint Clock::Now() noexcept
//{
//    timespec ts;
//    clock_gettime(CLOCK_MONOTONIC, &ts);
//
//    Int64 ns =
//        static_cast<Int64>(ts.tv_sec) * 1'000'000'000LL +
//        static_cast<Int64>(ts.tv_nsec);
//
//    return TimePoint(TimeSpan::FromNanoseconds(ns));
//}
//
//void Clock::Sleep(TimeSpan duration) noexcept
//{
//    if (duration.Nanoseconds() <= 0)
//        return;
//
//    timespec req;
//    req.tv_sec = duration.Nanoseconds() / 1'000'000'000LL;
//    req.tv_nsec = duration.Nanoseconds() % 1'000'000'000LL;
//
//    // nanosleep may be interrupted → loop until done
//    while (nanosleep(&req, &req) == -1)
//        ;
//}