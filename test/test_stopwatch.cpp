// Copyright Ingo Proff 2017.
// https://github.com/J-D-3/Stopwatch
// Distributed under the MIT Software License (X11 license).
// (See accompanying file LICENSE)

// Minimal, dependency-free assertion test. Returns 0 on success and a
// non-zero exit code on the first-or-more failures, so CTest detects it.

#include <cstdint>
#include <iostream>
#include <string>
#include "../include/stopwatch/Stopwatch.hpp"

static int failures = 0;

#define CHECK(cond)                                                        \
   do {                                                                    \
      if(!(cond)){                                                         \
         ++failures;                                                       \
         std::cerr << "FAILED: " << #cond << " (line " << __LINE__ << ")\n"; \
      }                                                                    \
   } while(0)

// Defined in test_link_tu.cpp. Calling it forces that second translation
// unit to be linked, so the build fails if a header-scope symbol is not
// inline (duplicate-definition guard for the header-only contract).
std::string link_probe();

int main()
{
   namespace sw = stopwatch;

   // The header is included in two translation units (here and in
   // test_link_tu.cpp); this must link cleanly.
   CHECK( link_probe() == "{1,2,3}" );

   // show_times() formatting, including the empty edge case.
   CHECK( sw::show_times({}) == "{}" );
   CHECK( sw::show_times({42}) == "{42}" );
   CHECK( sw::show_times({1, 2, 3}) == "{1,2,3}" );

   // elapsed_laps() with no recorded laps must not underflow and yields
   // no lap times.
   {
      sw::Stopwatch w;
      auto r = w.elapsed_laps();
      CHECK( r.second.empty() );
   }

   // Each lap() is reflected in elapsed_laps().
   {
      sw::Stopwatch w;
      w.lap();
      w.lap();
      w.lap();
      auto r = w.elapsed_laps();
      CHECK( r.second.size() == 3 );
   }

   // start() restarts the watch and clears recorded laps.
   {
      sw::Stopwatch w;
      w.lap();
      w.lap();
      w.start();
      auto r = w.elapsed_laps();
      CHECK( r.second.empty() );
   }

   // For one interval, coarser units are never numerically larger than
   // finer ones (ns count >= mus count >= ms count >= s count).
   {
      sw::Stopwatch w;
      volatile std::uint64_t sink = 0;
      for(std::size_t i = 1; i <= 2000000; i++){ sink += i; }
      const auto ns  = w.elapsed<sw::ns>();
      const auto mus = w.elapsed<sw::mus>();
      const auto ms  = w.elapsed<sw::ms>();
      const auto s   = w.elapsed<sw::s>();
      CHECK( ns >= mus );
      CHECK( mus >= ms );
      CHECK( ms >= s );
   }

   // Telescoping invariant: laps are contiguous and the first one starts at
   // the watch's start, so in nanoseconds (no rounding) the lap times sum
   // exactly to the reported total. This holds regardless of timing.
   {
      sw::Stopwatch w;
      volatile std::uint64_t sink = 0;
      for(int rep = 0; rep < 4; ++rep){
         for(std::size_t i = 1; i <= 500000; i++){ sink += i; }
         w.lap<sw::ns>();
      }
      const auto laps = w.elapsed_laps<sw::ns, sw::ns>();
      std::uint64_t sum = 0;
      for(const auto& t : laps.second){ sum += t; }
      CHECK( laps.second.size() == 4 );
      CHECK( sum == laps.first );
   }

   // const-correctness: the non-mutating accessors must be callable through
   // a const reference. This is a compile-time guard as much as a runtime
   // one - it would fail to build if elapsed()/elapsed_laps() lost const.
   {
      sw::Stopwatch w;
      w.lap();
      const sw::Stopwatch& cw = w;
      const auto ms    = cw.elapsed<sw::ms>();
      const auto laps  = cw.elapsed_laps<sw::ms, sw::ms>();
      CHECK( ms >= laps.first );          // total-since-start <= elapsed-now
      CHECK( laps.second.size() == 1 );
   }

   if( failures == 0 ){
      std::cout << "All checks passed.\n";
      return 0;
   }
   std::cerr << failures << " check(s) failed.\n";
   return 1;
}
