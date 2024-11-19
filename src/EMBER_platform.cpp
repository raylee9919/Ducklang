/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Sung Woo Lee $
   $Notice: (C) Copyright 2024 by Sung Woo Lee. All Rights Reserved. $
   ======================================================================== */



#ifdef _MSC_VER
  #include <windows.h>

static u64
get_os_timer_frequency() {
    LARGE_INTEGER x;
    QueryPerformanceFrequency(&x);
    return (u64)x.QuadPart;
}

static u64
get_os_timer() {
    LARGE_INTEGER x;
    QueryPerformanceCounter(&x);
    return (u64)x.QuadPart;
}

#if 0
static u64
get_wall_clock() {
    return __rdtsc();
}

static u64
get_wall_clock_frequency() {
    u64 os_frequency = get_os_timer_frequency();
    u64 os_begin = get_os_timer();
    u64 os_elapsed = 0;

    u64 cpu_begin = get_wall_clock();

    while (os_elapsed < os_frequency)
        os_elapsed = get_os_timer() - os_begin;

    u64 cpu_elapsed = get_wall_clock() - cpu_begin;
    f32 sec = (f32)os_elapsed / (f32)os_frequency;
    u64 result = (u64)((f32)cpu_elapsed / sec);

    return result;
}
#endif





#else
  #include <linux.h>
#endif


