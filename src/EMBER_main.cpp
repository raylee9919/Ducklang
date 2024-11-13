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
#include "EMBER_buffer.h"
#include "EMBER_utility.cpp"
#include "EMBER_string.cpp"
#include "EMBER_lexer.cpp"

enum Precedence 
{
    PREC_NONE,
};


/*
1. a + b * c = (a + (b * c))

   +
  / \
 a   *
    / \
   b   c

Works only on increasing precedence.

2. a * b + c = ((a * b) + c)
      +
     / \
    *   c
   / \
  a   b

Works only on decreasing precedence.

3. a > b * c + d = (a > ((b * c) + d))
    >
   / \
  a   +
     / \
    *   d
   / \
  b   c

4. a + b > c * d = ((a + b) > (c * d))
     >
    / \
   +   *
  / \ / \
 a  b c  d
parse_binary(cur_prec):
    if identifier or number: return
    
*/

struct Parser
{
    void advance() { ++current_token; }
    Token peek() {
        Token tk = *(current_token + 1);
        return tk;
    }
    Token_List *token_list;
    Token *current_token;
};

static void
parse_expression(Parser &parser) {
}

static void
parse_statement(Parser &parser) {
    switch (parser.current_token->type) {
        case Token_Type::IDENTIFIER: {
        } break;

        case Token_Type::NUMBER: {
        } break;

        case Token_Type::KEYWORD: {
            if (parser.current_token->literal == "s32") {
                if (parser.peek().type == Token_Type::IDENTIFIER) {
                    parser.advance();
                    if (parser.peek().type == Token_Type::EQUAL) {
                        parser.advance();
                        parse_expression(parser);
                    } else {
                        ASSERT(0);
                    }
                } else {
                    ASSERT(0);
                }
            } else {
                ASSERT(0);
            }
        } break;

        INVALID_DEFAULT_CASE;
    }
}

static void
parse(Token_List *token_list)
{
    Parser parser;
    parser.token_list = token_list;
    parser.current_token = token_list->base;

    parse_statement(parser);
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
            parse(&token_list);
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
