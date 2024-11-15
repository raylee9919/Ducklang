/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Sung Woo Lee $
   $Notice: (C) Copyright 2024 by Sung Woo Lee. All Rights Reserved. $
   ======================================================================== */

#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <string>
#include <unordered_map>

#include "EMBER_types.h"
#include "EMBER_buffer.h"
#include "EMBER_utility.cpp"
#include "EMBER_string.cpp"
#include "EMBER_lexer.cpp"
#include "EMBER_parser.cpp"


int main(void) {
    const char *file_name = "main.ember";
    Buffer code = read_entire_file_and_null_terminate(file_name);
    if (code.size) {
        Token_List token_list = {};
        token_list.init(&token_list, MB(1));

        if (token_list.base) {
            tokenize(code, &token_list);
            DEBUG_print_tokens(&token_list);
            std::vector<Ast_Node *> statements = parse(&token_list);
        } else {
            fprintf(stderr, "ERROR: Failed to malloc token list.\n");
        }
    } else {
        fprintf(stderr, "ERROR: Buffer's size is 0. (%s).\n", file_name);
    }

    return 0;
}
