#ifndef VM_TYPES_H_
#define VM_TYPES_H_
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Sung Woo Lee $
   $Notice: (C) Copyright 2024 by Sung Woo Lee. All Rights Reserved. $
   ======================================================================== */

typedef char                s8;  
typedef short               s16; 
typedef int                 s32; 
typedef long long           s64; 

typedef int                 b32;

typedef unsigned char       u8;
typedef unsigned short      u16; 
typedef unsigned int        u32; 
typedef unsigned long long  u64; 

typedef float               f32; 
typedef double              f64; 

#define KB(X) (1 << 10LL)
#define MB(X) (KB(X) << 10LL)
#define GB(X) (MB(X) << 10LL)

#define global_variable     static
#define internal            static
#define local_persistent    static

#define ASSERT(EXP) if (!(EXP)) { *(volatile int *)0 = 0; }
#define INVALID_DEFAULT_CASE default: { ASSERT(0); } break;

#endif // VM_TYPES_H_
