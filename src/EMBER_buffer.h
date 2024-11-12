#ifndef EMBER_BUFFER_H_
#define EMBER_BUFFER_H_
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Sung Woo Lee $
   $Notice: (C) Copyright 2024 by Sung Woo Lee. All Rights Reserved. $
   ======================================================================== */


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


#endif // EMBER_BUFFER_H_
