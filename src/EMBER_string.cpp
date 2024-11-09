/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Sung Woo Lee $
   $Notice: (C) Copyright 2024 by Sung Woo Lee. All Rights Reserved. $
   ======================================================================== */

#include "EMBER_types.h"
#include "EMBER_string.h"

static b32
operator == (String &s1, String &s2)
{
    b32 result = true;

    if (s1.length == s2.length)
    {
        for (u32 idx = 0;
             idx < s1.length;
             ++idx)
        {
            if (s1.data[idx] != s2.data[idx])
            {
                result = false;
                break;
            }
        }
    }
    else
    {
        result = false;
    }

    return result;
}

inline b32
operator != (String &s1, String &s2)
{
    b32 result = (s1 == s2) ? false : true;
    return result;
}

inline String
__String__(const char *_data, size_t _length)
{
    String str = {};
    str.data = _data;
    str.length = _length;
    return str;
}

inline String
__String__(char *data, size_t length)
{
    String str = {};
    str.data = data;
    str.length = length;
    return str;
}
