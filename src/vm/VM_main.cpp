/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Sung Woo Lee $
   $Notice: (C) Copyright 2024 by Sung Woo Lee. All Rights Reserved. $
   ======================================================================== */

/* @TODO:
 *
 * formatting string
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "VM_types.h"
#include "VM_buffer.h"
#include "VM_memory.h"
#include "VM_utility.h"

#include "../SHARED_operation.h"

#define TOTAL_MEMORY GB(1)
#define read_and_advance(TYPE) *(TYPE *)at; at += sizeof(TYPE);

#define BINARY_OP_ADD +
#define BINARY_OP_SUB -
#define BINARY_OP_MUL *
#define BINARY_OP_DIV /
#define OP_CASE(OPCODE) \
    case (OP_##OPCODE): \
{ \
    s32 loff = read_and_advance(s32); \
    s32 roff = read_and_advance(s32); \
    stack.push_s32(stack.at(loff) BINARY_OP_##OPCODE stack.at(roff)); \
} break;



struct Stack {
    Stack(u32 _cap, Memory_Arena *arena) {
        base = (u8 *)push_size(arena, _cap);
        cap = _cap;
        used = 0;
    }

    void push_s32(s32 val) {
        ASSERT(used + sizeof(val) <= cap);
        *(u32 *)(base + used) = val;
        used += sizeof(val);
    }

    void push_string(const char *str, u32 length) {
        ASSERT(used + length <= cap);
        for (u32 i = 0; i < length; ++i) {
            ((char *)(base + used))[i] = str[i];
        }
        used += length;
    }

    s32 at(s32 offset) {
        ASSERT(offset <= (s32)used);
        return *(s32 *)(base + offset);
    }

    u8 *base;
    u32 cap;
    u32 used;
};

inline void
print_string(const char *str, u32 length) {
    for (u32 i = 0; i < length; ++i) {
        const char c = str[i];
        fputc(c, stdout);
    }
}

int main() 
{
    void *vm_memory = malloc(TOTAL_MEMORY);
    Memory_Arena vm_arena(TOTAL_MEMORY, vm_memory);
    Stack stack( MB(256), &vm_arena );

    Buffer src = read_entire_file(&vm_arena, bytecode);
    if (src.data) {
        for (u8 *at = src.data; at < (src.data + src.size);) {
            Opcode opcode = *(Opcode *)at;
            at += sizeof(Opcode);
            switch (opcode) {
                case OP_PUSH_S32_IMM: {
                    s32 val = read_and_advance(s32);

                    stack.push_s32(val);
                } break;

                case OP_PUSH_S32_OFF: {
                    s32 offset = read_and_advance(s32);

                    stack.push_s32(stack.at(offset));
                } break;

                OP_CASE(ADD);
                OP_CASE(SUB);
                OP_CASE(MUL);
                OP_CASE(DIV);

                case OP_PUSH_STRING: {
                    u32 length = read_and_advance(u32);
                    const char *str = (const char *)at;
                    at += length;

                    stack.push_string(str, length);
                } break;

                case OP_PRINT: {
                    s32 str_offset  = read_and_advance(s32);
                    u32 str_length  = read_and_advance(u32);
                    u32 expr_count  = read_and_advance(u32);

                    if (expr_count == 0) {
                        print_string((const char *)(stack.base + str_offset), str_length);
                    } else {
                        for (u32 i = 0; i < expr_count; ++i) {
                            Type_Offset to = *(Type_Offset *)at;
                            if (to.type == EVAL_S32) {

                            } else {
                                ASSERT(0);
                            }
                            at += sizeof(Type_Offset);
                        }
                    }

                } break;

                INVALID_DEFAULT_CASE;
            }
        }
    } else {
        fprintf(stderr, "ERROR: Couldn't read file %s.\n", bytecode);
        return 1;
    }

    return 0;
}
