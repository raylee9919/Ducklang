#ifndef VM_UTILITY_H_
#define VM_UTILITY_H_
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Sung Woo Lee $
   $Notice: (C) Copyright 2024 by Sung Woo Lee. All Rights Reserved. $
   ======================================================================== */

#define ASSERT(EXP) if (!(EXP)) { *(volatile int *)0 = 0; }
#define INVALID_DEFAULT_CASE default: { ASSERT(0); } break;



#endif // VM_UTILITY_H_
