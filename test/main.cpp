// Copyright Ingo Proff 2017.
// https://github.com/J-D-3/Stopwatch
// Distributed under the MIT Software License (X11 license).
// (See accompanying file LICENSE)

#include <iostream>
#include "../include/stopwatch/Stopwatch.hpp"


int main()
{
   //Namespace alias
   namespace sw = stopwatch;

   //Create and start a stopwatch
   sw::Stopwatch my_watch;
   my_watch.start();

   //Do something time-consuming
   for(std::size_t i = 1; i <= 500000; i++){
      if( i%10 == 0){
         std::cout << i << std::endl;
      }
      if( i % 100000 == 0){
          my_watch.lap();
      }
   }

   //Get elapsed time..
   // .. in nanoseconds
   std::uint64_t elapsed_ns = my_watch.elapsed<sw::nanoseconds>();
   // .. in microseconds
   std::uint64_t elapsed_mus = my_watch.elapsed<sw::microseconds>();
   // .. in milliseconds (default template argument, therefore not needed)
   std::uint64_t elapsed_ms = my_watch.elapsed();
   // .. in seconds
   std::uint64_t elapsed_s = my_watch.elapsed<sw::seconds>();

   //Get lap times
   auto laps = my_watch.elapsed_laps();

   //Print to screen
   std::cout << "---------------" << std::endl;
   std::cout << elapsed_ns << " ns" << std::endl;
   std::cout << elapsed_mus << " mus" << std::endl;
   std::cout << elapsed_ms << " ms" << std::endl;
   std::cout << elapsed_s << " s" << std::endl;

   std::cout << "---------------" << std::endl;
   std::cout << "Laps Total: " << laps.first << std::endl;
   auto lap_times = laps.second;
   std::cout << "Lap Times: " << sw::show_times(lap_times);

   return 0;
}
