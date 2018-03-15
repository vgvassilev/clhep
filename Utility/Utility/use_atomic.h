#ifndef CLHEP_USE_ATOMIC_GUARD_H
#define CLHEP_USE_ATOMIC_GUARD_H

// ======================================================================
//
// Use std::atomic when the compiler declares it uses the C++11 standard
//
// on macOS, atomic is available starting with Sierra (Darwin 16)
// ======================================================================

#if __cplusplus >= 201103L

  #if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 7) || USING_VISUAL
    #include <atomic>
    #define CLHEP_USE_ATOMIC
  #elif __clang__
    #if __has_extension(c_atomic)
      #include <atomic>
      #define CLHEP_USE_ATOMIC
    #endif
  #endif

#endif

#endif
