/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Sung Woo Lee $
   $Notice: (C) Copyright 2024 by Sung Woo Lee. All Rights Reserved. $
   ======================================================================== */

#include <stdio.h>
#include <stdlib.h>

#include "VM_utility.h"
#include "VM_types.h"
#include "VM_memory.h"
    
enum Opcode 
{
    OP_RETURN
};

global_variable Memory_Arena vm_arena;

int main() 
{
    vm_arena.size = GB(1);
    vm_arena.base = malloc(vm_arena.size);
    vm_arena.used = 0;

    return 0;
}
