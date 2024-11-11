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

enum Precedance
{
  PREC_NONE,

  PREC_ASSIGNMENT,  // =
  PREC_OR,          // or
  PREC_AND,         // and
  PREC_EQUALITY,    // == !=
  PREC_COMPARISON,  // < > <= >=
  PREC_TERM,        // + -
  PREC_FACTOR,      // * /
  PREC_UNARY,       // ! -
  PREC_CALL,        // . ()

  PREC_PRIMARY
};

static void
parse_precedance(Precedance precedance)
{
}

static void
expression()
{
    parse_precedance(PREC_ASSIGNMENT);
}

struct Parser
{
    void init(Token_List *token_list) {
        tokens = token_list;
        next_idx = 0;
    }

    Token eat() {
        if (next_idx < tokens->count) {
            return tokens->base[++next_idx];
        } else {
            ASSERT(!"Next index exceeded token count!");
            return {};
        }
    }

    Token_List *tokens;
    s32 next_idx;
};

static void
parse(Token_List *token_list)
{
    Parser parser;
    parser.init(token_list);

    while (parser.next_idx< token_list->count)
    {
    }
}

int main(void)
{
    const char *file_name = "main.ember";
    Buffer code = read_entire_file_and_null_terminate(file_name);
    if (code.size)
    {
        Token_List token_list = {};
        token_list.init(&token_list, MB(1));

        if (token_list.base)
        {
            tokenize(code, &token_list);
#if 0
            for (u32 idx = 0;
                 idx < token_list.count;
                 ++idx)
            {
                Token tk = token_list.base[idx];
                printf("%.*s (%.*s)\n", (s32)tk.literal.length, tk.literal.data,
                       (s32)tk.type_literal.length, tk.type_literal.data);
            }
#endif
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
