#pragma once

// Visibility export macros for proper symbol visibility when building with
// -fvisibility=hidden (e.g. as a FetchContent sub-project of arjun).
#if defined(_WIN32) || defined(__CYGWIN__)
  #ifdef EvalMaxSAT_EXPORTS
    #define EVALMAXSAT_PUBLIC __declspec(dllexport)
  #else
    #define EVALMAXSAT_PUBLIC __declspec(dllimport)
  #endif
#elif defined(__GNUC__) || defined(__clang__)
  #ifdef EvalMaxSAT_EXPORTS
    #define EVALMAXSAT_PUBLIC __attribute__((visibility("default")))
  #else
    #define EVALMAXSAT_PUBLIC
  #endif
#else
  #define EVALMAXSAT_PUBLIC
#endif
