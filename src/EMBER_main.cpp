/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Sung Woo Lee $
   $Notice: (C) Copyright 2024 by Sung Woo Lee. All Rights Reserved. $
   ======================================================================== */

#include <stdio.h>
#include <stdlib.h>

#include "EMBER_types.h"
#include "EMBER_utility.cpp"


enum Token_Type
{
    END_OF_FILE,

    OPEN_PAREN, CLOSE_PAREN, OPEN_BRACE, CLOSE_BRACE, OPEN_BRACKET, CLOSE_BRACKET,
    PLUS, PLUS_PLUS, PLUS_EQUAL,
    MINUS, MINUS_MINUS, MINUS_EQUAL,
    STAR, STAR_EQUAL,
    SLASH, SLASH_EQUAL,
    PERIOD, COMMA, COLON, SEMICOLON,

    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    IDENTIFIER, STRING, NUMBER,

    AND, CLASS, ELSE, FALSE, FN, FOR, IF, NUL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

#if _INTERNAL
    DEBUG_SINGLE_LINE_COMMENT,
    DEBUG_MULTI_LINE_COMMENT,
#endif
};

struct Token
{
    Token_Type type;
    const char *type_literal;

    const char *literal;
};

struct Lexer
{
    char *lo;
    char *hi;
    char *end;
};

static b32
is_next_line(char c)
{
    b32 result = (c == '\r' || c == '\n');
    return result;
}

#define push_token(LEXER, TYPE) _push_token(LEXER, TYPE, #TYPE)
static void
_push_token(Lexer *lexer, Token_Type type, const char *type_literal)
{
    size_t literal_len = (lexer->hi - lexer->lo);
    printf("%.*s (%s)\n", (s32)literal_len, lexer->lo, type_literal);
}

static void
tokenize(Buffer code)
{
    Lexer lx = {};
    lx.lo = (char *)code.data;
    lx.hi = (lx.lo + 1);
    lx.end = ( (char *)code.data + code.size );

    b32 should_continue = 1;

    while (should_continue)
    {
        char c = *lx.lo;
        switch (c)
        {
            case '\0': // EOF
            {
                push_token(&lx, END_OF_FILE);
                should_continue = 0;
            } break;

            case '(':
            {
                push_token(&lx, OPEN_PAREN);
                lx.lo = lx.hi++;
            } break;
            case ')':
            {
                push_token(&lx, CLOSE_PAREN);
                lx.lo = lx.hi++;
            } break;
            case '[':
            {
                push_token(&lx, OPEN_BRACKET);
                lx.lo = lx.hi++;
            } break;
            case ']':
            {
                push_token(&lx, CLOSE_BRACKET);
                lx.lo = lx.hi++;
            } break;
            case '{':
            {
                push_token(&lx, OPEN_BRACE);
                lx.lo = lx.hi++;
            } break;
            case '}':
            {
                push_token(&lx, CLOSE_BRACE);
                lx.lo = lx.hi++;
            } break;

            case '+':
            {
                if (lx.hi < lx.end)
                {
                    if (*lx.hi == '+')
                    {
                        ++lx.hi;
                        push_token(&lx, PLUS_PLUS);
                        lx.lo = lx.hi++;
                    }
                    else if (*lx.hi == '=')
                    {
                        ++lx.hi;
                        push_token(&lx, PLUS_EQUAL);
                        lx.lo = lx.hi++;
                    }
                    else
                    {
                        push_token(&lx, PLUS);
                        lx.lo = lx.hi++;
                    }
                }
            } break;

            case '-':
            {
                if (lx.hi < lx.end)
                {
                    if (*lx.hi == '-')
                    {
                        ++lx.hi;
                        push_token(&lx, MINUS_MINUS);
                        lx.lo = lx.hi++;
                    }
                    else if (*lx.hi == '=')
                    {
                        ++lx.hi;
                        push_token(&lx, MINUS_EQUAL);
                        lx.lo = lx.hi++;
                    }
                    else
                    {
                        push_token(&lx, MINUS);
                        lx.lo = lx.hi++;
                    }
                }
            } break;

            case '*':
            {
                if (lx.hi < lx.end)
                {
                    if (*lx.hi == '=')
                    {
                        ++lx.hi;
                        push_token(&lx, STAR_EQUAL);
                        lx.lo = lx.hi++;
                    }
                    else
                    {
                        push_token(&lx, STAR);
                        lx.lo = lx.hi++;
                    }
                }
            } break;

            case '/':
            {
                if (lx.hi < lx.end)
                {
                    if (*lx.hi == '/') // Single-line comment
                    {
                        ++lx.hi;
                        while (lx.hi < lx.end &&
                               !is_next_line(*lx.hi))
                        {
                            ++lx.hi;
                        }

#if _INTERNAL
                        push_token(&lx, DEBUG_SINGLE_LINE_COMMENT);
#endif
                        lx.lo = lx.hi++;
                    }
                    else if (*lx.hi == '*') // Multi-line comment
                    {
                        ++lx.hi;
                        for (;;)
                        {
                            if (lx.hi < lx.end &&
                                *lx.hi++ == '*')
                            {
                                if (lx.hi < lx.end &&
                                    *lx.hi == '/')
                                {
                                    ++lx.hi;
#if _INTERNAL
                                    push_token(&lx, DEBUG_MULTI_LINE_COMMENT);
#endif
                                    lx.lo = lx.hi++;
                                    break;
                                }
                            }
                        }
                    }
                    else if (*lx.hi == '=')
                    {
                        ++lx.hi;
                        push_token(&lx, SLASH_EQUAL);
                        lx.lo = lx.hi++;
                    }
                    else
                    {
                        push_token(&lx, SLASH);
                        lx.lo = lx.hi++;
                    }
                }
            } break;

            default:
            {
                lx.lo = lx.hi++;
            } break;
        }
    }
}

int main(void)
{
    Buffer code = read_entire_file_and_null_terminate("main.ember");
    if (code.size)
    {
        tokenize(code);
    }
    else
    {
        fprintf(stderr, "ERROR\n");
    }

    return 0;
}
