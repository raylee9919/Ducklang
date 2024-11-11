#ifndef EMBER_UTILITY_H_
#define EMBER_UTILITY_H_
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Sung Woo Lee $
   $Notice: (C) Copyright 2024 by Sung Woo Lee. All Rights Reserved. $
   ======================================================================== */

#define ASSERT(EXP) if (!(EXP)) { *(volatile int *)0 = 0; }
#define array_length(ARR) (sizeof(ARR)/sizeof(ARR[0]))

static Buffer
read_entire_file_and_null_terminate(const char *path)
{
    Buffer result = {};

    FILE *file = fopen(path, "rb");
    if (file)
    {
        fseek(file, 0, SEEK_END);
        size_t file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        result.data = (u8 *)malloc(file_size + 1);
        result.size = file_size + 1;

        fread(result.data, file_size, 1, file);
        result.data[file_size] = 0;
    }

    return result;
}

static size_t
string_length(const char *str)
{
    size_t len = 0;
    for (const char *c = str;
         *c != 0;
         ++c)
    {
        ++len;
    }
    return len;
}

inline b32
is_whitespace(char c)
{
    b32 result = (c == ' ' || c == '\t' || c == '\r' || c == '\n');
    return result;
}

inline b32
is_next_line(char c)
{
    b32 result = (c == '\r' || c == '\n');
    return result;
}

inline b32
is_alphabet_or_underscore(char c)
{
    b32 result = ( (c >= 'a' && c <= 'z') ||
                   (c >= 'A' && c <= 'Z') ||
                   (c == '_'));
    return result;
}

inline b32
is_digit(char c)
{
    b32 result = (c >= '0' && c<= '9');
    return result;
}

inline b32
is_alphabet_or_underscore_or_digit(char c)
{
    b32 result = ( (c >= 'a' && c <= 'z') ||
                   (c >= 'A' && c <= 'Z') ||
                   (c == '_') ||
                   (c >= '0' && c <= '9'));
    return result;
}




#endif // EMBER_UTILITY_H_
