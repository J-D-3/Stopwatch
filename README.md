# Stopwatch
C++ implementation of a `Stopwatch` class

## Introduction
This repository is home to a tiny C++ implementation of a stopwatch. It aims at providing an easy-to-use stopwatch. Basically, it's just an API to `std::chrono::high_precision_clock`. But googling for "*std chrono duration_cast*", "*std chrono time_point template*" for the 271st time -rough estimation-, finally convinced me that I need to write this code snippet. Here I'm sharing the result.

## Installation
Download the header `Stopwatch.hpp` from the include folder of this repository, include it, and you're ready to go. No dependencies no nothing.

## Usage
*Info:* In */test/main.cpp* you'll find a program that combines the following three examples into one `main()` function. It should be executable as soon as you adapt the include path to "Stopwatch.hpp".

### Basics
Suppose you have a loop, a function call or a complete program whose execution time you want to measure.  
The `Stopwatch` class allows you to do this with two lines of code:  
`Stopwatch my_watch;` creates an instance of `Stopwatch` and starts it.   
`uint64_t duration_ms = my_watch.elapsed();` returns the elapsed time since the instantiation in milliseconds.

Here's a minimal executable example:
```cpp
#include <iostream>
#include "Stopwatch.hpp"

int main(){
  //Namespace alias
  namespace sw = stopwatch;

  //Create and start a stopwatch
  sw::Stopwatch my_watch;

  //Do something time-consuming here...

  //Take the time
  auto duration_ms = my_watch.elapsed();
  std::cout << "Elapsed: " << duration_ms << " milliseconds." << std::endl;
}
```

### Goodies
Additionally, you can start (actually, re-start) the watch manually at any point by calling `my_watch.start()`.  
Furthermore, you can get the elapsed time formatted as seconds, milliseconds, microseconds or nanoseconds by passing the respective template-argument to the `my_watch.elapsed()` call:

```cpp
#include <iostream>
#include "Stopwatch.hpp"

int main()
{
   //Namespace alias
   namespace sw = stopwatch;

   //Create a stopwatch..
   sw::Stopwatch my_watch;
   //and (re-)start it
   my_watch.start();

   //Do something time-consuming here...

   //Get elapsed time..
   // .. in nanoseconds
   std::uint64_t elapsed_ns = my_watch.elapsed<sw::ns>();
   // .. in microseconds
   std::uint64_t elapsed_mus = my_watch.elapsed<sw::mus>();
   // .. in milliseconds (default template argument, therefore not needed)
   std::uint64_t elapsed_ms = my_watch.elapsed();
   // .. in seconds
   std::uint64_t elapsed_s = my_watch.elapsed<sw::s>();

   return 0;
}
```

### Taking lap times
Lastly, you can use a `Stopwatch` to take lap times. Of course this can be done using several stopwatches, but here's the API to do it with only one.  
Initialize a stopwatch as before. Every time `my_watch.lap();` is called, the watch outputs the elapsed time since the last call to `lap()`. (Or since the last call of `my_watch.start()` for the first lap.) The output can be formatted by passing a format template to the `lap()` call as in the **Goodies** example (see also code below).  
The function `my_watch.elapsed_laps<fmt_total, fmt_laps>()` returns a pair: The first element is the total elapsed time between `my_watch.start()` and the last call of `my_watch.lap()`. The second element is a vector of lap times. The format of the times can be set using the template arguments `fmt_total` and `fmt_laps` respectively. This comes in handy if you want to ouput the total time in seconds, but the lap times in milliseconds. The default is `fmt_total = MILLISECONDS` and `fmt_lap = fmt_total`.  
Finally, some code taking lap times of a loop:

```cpp
#include <iostream>
#include "Stopwatch.hpp"

int main()
{
   //Namespace alias
   namespace sw = stopwatch;

   //Create and start a stopwatch
   sw::Stopwatch my_watch;

   //Do something time-consuming:
   for(std::size_t i = 1; i <= 500000; i++){
      if( i%10 == 0){
         std::cout << i << std::endl;
      }
      //Take lap time every 100000th iteration
      if( i % 100000 == 0){
        auto lap_ms = my_watch.lap<sw::milliseconds>();
        std::cout << "Lap time in ms: " << lap_ms << std::endl;
      }
   }

   //Get all lap times. The total in seconds, single times in milliseconds
   auto laps = my_watch.elapsed_laps<sw::seconds, sw::milliseconds>();

   //Print to console
   std::cout << "---------------" << std::endl;
   std::cout << "Laps Total: " << laps.first << " sec" << std::endl;
   auto lap_times = laps.second;
   std::cout << "Lap Times [ms]: " << sw::show_times(lap_times);

   return 0;
}
```


## License
Distributed under the MIT Software License (X11 license). (See accompanying file LICENSE.)
