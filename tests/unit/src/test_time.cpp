//#include "catch_amalgamated.hpp"
//
//#include "System/Time/TimeSpan.hpp"
//#include "System/Time/TimePoint.hpp"
//#include "System/Time/Clock.hpp"
//#include "System/Time/FrameTimer.hpp"
//
//// ------------------------------------------------------------
//// TimeSpan
//// ------------------------------------------------------------
//
//TEST_CASE("TimeSpan: construction and accessors")
//{
//    TimeSpan zero;
//    REQUIRE(zero.Nanoseconds() == 0);
//
//    TimeSpan ns = TimeSpan::FromNanoseconds(500);
//    REQUIRE(ns.Nanoseconds() == 500);
//
//    TimeSpan us = TimeSpan::FromMicroseconds(2);
//    REQUIRE(us.Nanoseconds() == 2'000);
//
//    TimeSpan ms = TimeSpan::FromMilliseconds(3);
//    REQUIRE(ms.Nanoseconds() == 3'000'000);
//
//    TimeSpan s = TimeSpan::FromSeconds(1.5);
//    REQUIRE(s.Nanoseconds() == 1'500'000'000);
//}
//
//TEST_CASE("TimeSpan: arithmetic")
//{
//    TimeSpan a = TimeSpan::FromMilliseconds(10);
//    TimeSpan b = TimeSpan::FromMilliseconds(5);
//
//    TimeSpan c = a + b;
//    REQUIRE(c.Nanoseconds() == 15'000'000);
//
//    TimeSpan d = a - b;
//    REQUIRE(d.Nanoseconds() == 5'000'000);
//}
//
//TEST_CASE("TimeSpan: comparisons")
//{
//    TimeSpan a = TimeSpan::FromMilliseconds(1);
//    TimeSpan b = TimeSpan::FromMilliseconds(2);
//
//    REQUIRE(a < b);
//    REQUIRE(b > a);
//    REQUIRE_FALSE(a == b);
//    REQUIRE(a == TimeSpan::FromMilliseconds(1));
//}
//
//// ------------------------------------------------------------
//// TimePoint
//// ------------------------------------------------------------
//
//TEST_CASE("TimePoint: arithmetic")
//{
//    TimePoint t0(TimeSpan::FromSeconds(10));
//    TimeSpan  dt = TimeSpan::FromSeconds(2);
//
//    TimePoint t1 = t0 + dt;
//    REQUIRE(t1.SinceEpoch().Nanoseconds() == 12'000'000'000LL);
//
//    TimeSpan diff = t1 - t0;
//    REQUIRE(diff.Nanoseconds() == 2'000'000'000LL);
//}
//
//TEST_CASE("TimePoint: comparisons")
//{
//    TimePoint a(TimeSpan::FromSeconds(1));
//    TimePoint b(TimeSpan::FromSeconds(2));
//
//    REQUIRE(a < b);
//    REQUIRE_FALSE(b < a);
//}
//
//// ------------------------------------------------------------
//// Clock
//// ------------------------------------------------------------
//
//TEST_CASE("Clock: Now is monotonic")
//{
//    TimePoint a = Clock::Now();
//    TimePoint b = Clock::Now();
//
//    // Pode ser igual em resoluções muito altas, mas nunca menor
//    REQUIRE_FALSE(b < a);
//}
//
//TEST_CASE("Clock: Sleep advances time")
//{
//    TimePoint start = Clock::Now();
//
//    Clock::Sleep(TimeSpan::FromMilliseconds(5));
//
//    TimePoint end = Clock::Now();
//    TimeSpan elapsed = end - start;
//
//    // Não testamos precisão exata — apenas progresso
//    REQUIRE(elapsed.Nanoseconds() > 0);
//}
//
//// ------------------------------------------------------------
//// FrameTimer
//// ------------------------------------------------------------
//
//TEST_CASE("FrameTimer: delta is updated")
//{
//    FrameTimer timer;
//
//    Clock::Sleep(TimeSpan::FromMilliseconds(1));
//    timer.Tick();
//
//    TimeSpan delta = timer.Delta();
//
//    REQUIRE(delta.Nanoseconds() > 0);
//}
//
//TEST_CASE("FrameTimer: delta seconds matches nanoseconds")
//{
//    FrameTimer timer;
//
//    Clock::Sleep(TimeSpan::FromMilliseconds(10));
//    timer.Tick();
//
//    Double seconds = timer.DeltaSeconds();
//    REQUIRE(seconds > 0.0);
//}