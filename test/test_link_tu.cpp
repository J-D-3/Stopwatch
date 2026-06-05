// Copyright Ingo Proff 2017.
// https://github.com/J-D-3/Stopwatch
// Distributed under the MIT Software License (X11 license).
// (See accompanying file LICENSE)

// Second translation unit that includes the header. Its sole purpose is to
// be linked alongside test_stopwatch.cpp: if any header-scope function has
// external linkage (i.e. is not inline), linking the two objects fails with
// a duplicate-symbol error. This guards the header-only contract.

#include <string>
#include "../include/stopwatch/Stopwatch.hpp"

std::string link_probe()
{
   return stopwatch::show_times({1, 2, 3});
}
