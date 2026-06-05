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

int main()
{
   namespace sw = stopwatch;

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

   if( failures == 0 ){
      std::cout << "All checks passed.\n";
      return 0;
   }
   std::cerr << failures << " check(s) failed.\n";
   return 1;
}
