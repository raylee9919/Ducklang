#ifndef VM_UTILITY_H_
#define VM_UTILITY_H_
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Sung Woo Lee $
   $Notice: (C) Copyright 2024 by Sung Woo Lee. All Rights Reserved. $
   ======================================================================== */

static Buffer
read_entire_file(Memory_Arena *arena, const char *path)
{
    Buffer result = {};

    FILE *file = fopen(path, "rb");
    if (file)
    {
        fseek(file, 0, SEEK_END);
        size_t file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        result.data = (u8 *)push_size(arena, file_size);
        result.size = file_size;

        fread(result.data, file_size, 1, file);
    }

    return result;
}



#endif // VM_UTILITY_H_
