#ifndef EMBER_TYPES_H_
#define EMBER_TYPES_H_
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


struct Buffer
{
    u8 *data;
    size_t size;
};

struct Buffer_List
{
    Buffer *base;
    size_t count;
};

#endif // EMBER_TYPES_H_
