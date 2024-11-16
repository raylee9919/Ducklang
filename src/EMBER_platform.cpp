/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Sung Woo Lee $
   $Notice: (C) Copyright 2024 by Sung Woo Lee. All Rights Reserved. $
   ======================================================================== */



#ifdef _MSC_VER
  #include <windows.h>
#else
  #include <linux.h>
#endif


static int
get_wall_clock() {
#ifdef _MSC_VER
    LARGE_INTEGER val;
    QueryPerformanceCounter(&val);
    return val.LowPart;
#else
    ASSERT(0);
#endif
}
