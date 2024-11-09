/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Sung Woo Lee $
   $Notice: (C) Copyright 2024 by Sung Woo Lee. All Rights Reserved. $
   ======================================================================== */

#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <unordered_map>

#include "EMBER_types.h"
#include "EMBER_utility.cpp"
#include "EMBER_string.cpp"
#include "EMBER_lexer.cpp"

int main(void)
{
    const char *file_name = "main.ember";
    Buffer code = read_entire_file_and_null_terminate(file_name);
    if (code.size)
    {
        Token_List token_list = {};
        token_list.size = sizeof(Token_List) * MB(1);
        token_list.base = (Token *)malloc(token_list.size);
        token_list.used = 0;
        token_list.count = 0;
        if (token_list.base)
        {
            tokenize(code, &token_list);
            for (u32 idx = 0; idx < token_list.count; ++idx)
            {
                Token tk = token_list.base[idx];
                printf("%.*s (%.*s)\n", (s32)tk.literal.length, tk.literal.data, (s32)tk.type_literal.length, tk.type_literal.data);
            }
        }
        else
        {
            fprintf(stderr, "ERROR: Failed to malloc token list.\n");
        }
    }
    else
    {
        fprintf(stderr, "ERROR: Buffer's size is 0. (%s).\n", file_name);
    }

    return 0;
}
