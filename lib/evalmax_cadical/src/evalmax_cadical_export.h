#pragma once

// Visibility export macros for proper symbol visibility when building with
// -fvisibility=hidden (e.g. as a FetchContent sub-project of arjun).
#if defined(_WIN32) || defined(__CYGWIN__)
  #ifdef evalmax_cadical_EXPORTS
    #define EVALMAX_CADICAL_PUBLIC __declspec(dllexport)
  #else
    #define EVALMAX_CADICAL_PUBLIC __declspec(dllimport)
  #endif
#elif defined(__GNUC__) || defined(__clang__)
  #ifdef evalmax_cadical_EXPORTS
    #define EVALMAX_CADICAL_PUBLIC __attribute__((visibility("default")))
  #else
    #define EVALMAX_CADICAL_PUBLIC
  #endif
#else
  #define EVALMAX_CADICAL_PUBLIC
#endif
