/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Sung Woo Lee $
   $Notice: (C) Copyright 2024 by Sung Woo Lee. All Rights Reserved. $
   ======================================================================== */

struct String {
    const char *data;
    size_t length;
};

static b32
operator == (String &s1, String &s2) {
    b32 result = true;

    if (s1.length == s2.length) {
        for (u32 idx = 0;
             idx < s1.length;
             ++idx)
        {
            if (s1.data[idx] != s2.data[idx]) {
                result = false;
                break;
            }
        }
    } else {
        result = false;
    }

    return result;
}

static b32
operator == (String &str, const char *cstr) {
    b32 result = true;

    size_t cstr_length = string_length(cstr);
    if (str.length == cstr_length) {
        for (u32 i = 0; i < cstr_length; ++i) {
            if (str.data[i] != cstr[i]) {
                result = false;
                break;
            }
        }
    } else  {
        result = false;
    }

    return result;
}

static b32
operator != (String &str, const char *cstr) {
    b32 result = !(str == cstr);
    return result;
}

inline b32
operator != (String &s1, String &s2) {
    b32 result = (s1 == s2) ? false : true;
    return result;
}

static s32
string_to_s32(String str) {
    s32 result = 0;
    if (str.data && str.length) {
        for (s32 idx = 0; idx < str.length; ++idx) {
            char c = str.data[idx];
            if (is_digit(c)) {
                result = result * 10 + (c - '0');
            } else {
                ASSERT(0);
            }
        }
    } else {
        ASSERT(0);
    }
    return result;
}

static String
_String(const char *_data) {
    String str;
    str.data = _data;
    str.length = string_length(_data);
    return str;
}

static String
_String(const char *_data, size_t _length) {
    String str;
    str.data = _data;
    str.length = _length;
    return str;
}
