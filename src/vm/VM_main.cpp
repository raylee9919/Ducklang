/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Sung Woo Lee $
   $Notice: (C) Copyright 2024 by Sung Woo Lee. All Rights Reserved. $
   ======================================================================== */

#include <stdio.h>
#include <stdlib.h>

#include "VM_types.h"
#include "VM_utility.h"
#include "VM_memory.h"

#include "../EMBER_operation.h"
    
struct Buffer
{
    u8 *data;
    size_t size;
};

static Buffer
read_entire_file_and_null_terminate(Memory_Arena *arena, const char *path)
{
    Buffer result = {};

    FILE *file = fopen(path, "rb");
    if (file)
    {
        fseek(file, 0, SEEK_END);
        size_t file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        result.data = (u8 *)push_size(arena, file_size + 1);
        result.size = file_size + 1;

        fread(result.data, file_size, 1, file);
        result.data[file_size] = 0;
    }

    return result;
}

int main() 
{
    Memory_Arena vm_arena;
    vm_arena.size = GB(1);
    vm_arena.base = malloc(vm_arena.size);
    vm_arena.used = 0;

    const char *src_name = "main.ember";
    Buffer src = read_entire_file_and_null_terminate(&vm_arena, src_name);
    if (src.data) 
    {
        for (u8 *at = src.data;
             at < (src.data + src.size);
             ++at) 
        {
            switch (*at++) {
                case OP_RETURN: {

                } break;

                case OP_ADD: {

                } break;

                case OP_SUB: {

                } break;

                case OP_MUL: {

                } break;





                INVALID_DEFAULT_CASE
            }
        }
    } 
    else 
    {
        fprintf(stderr, "ERROR: Couldn't read file %s.\n", src_name);
    }


    return 0;
}
