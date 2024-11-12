#ifndef VM_MEMORY_H_
#define VM_MEMORY_H_
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Sung Woo Lee $
   $Notice: (C) Copyright 2024 by Sung Woo Lee. All Rights Reserved. $
   ======================================================================== */

struct Memory_Arena
{
    void *base;
    size_t size;
    size_t used;
};

#define push_struct(ARENA, STRUCT) push_size(ARENA, sizeof(STRUCT))
internal void *
push_size(Memory_Arena *arena, size_t size)
{
    ASSERT(arena->used + size <= arena->size);
    void *result = (u8 *)arena->base + arena->used;
    arena->used += size;
    return result;
}


#endif // VM_MEMORY_H_
