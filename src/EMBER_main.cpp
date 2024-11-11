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
#include "EMBER_table.cpp"

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

struct Parser
{
    void init(Token_List *token_list) 
    {
        tokens = token_list;
        cur = token_list->base;
        prev = 0;
    }

    Token_List *tokens;
    Token *cur;
    Token *prev;
};

inline Parse_Rule
get_parse_rule(Token_Type token_type)
{
    Parse_Rule rule = parse_rule_table[token_type];
    return rule;
}

static void
parse(Token_List *token_list)
{
    Parser parser;
    parser.init(token_list);

    for (;;) // @TODO: Proper Termination
    {
        Token_Type token_type = parser.cur->type;
        Parse_Rule rule = get_parse_rule(token_type);
        if (rule.infix)
        {
        }
        else if (rule.prefix)
        {
        }
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
