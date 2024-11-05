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
    INVALID = -1,
    END_OF_FILE,

    LPAREN, RPAREN,
    LBRACE, RBRACE,
    LBRACKET,
    RBRACKET,
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

inline b32
is_whitespace(char c)
{
    b32 result = (c == ' ' || c == '\t' || c == '\r' || c == '\n');
    return result;
}

inline b32
is_next_line(char c)
{
    b32 result = (c == '\r' || c == '\n');
    return result;
}

#define case_is_alphabet \
    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z': \
    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z'

inline b32
is_alphabet_or_underscore(char c)
{
    b32 result = ( (c >= 'a' && c <= 'z') ||
                   (c >= 'A' && c <= 'Z') ||
                   (c == '_'));
    return result;
}

inline b32
is_digit(char c)
{
    b32 result = (c >= '0' && c<= '9');
    return result;
}

#define push_token_and_advance(LEXER, TYPE) _push_token_and_advance(LEXER, TYPE, #TYPE)
static void
_push_token_and_advance(Lexer *lexer, Token_Type type, const char *type_literal)
{
    size_t literal_len = (lexer->hi - lexer->lo);
    printf("%.*s (%s)\n", (s32)literal_len, lexer->lo, type_literal);
    lexer->lo = lexer->hi++;
}

inline char
peek_safe(Lexer *lexer)
{
    char result;

    if (lexer->hi < lexer->end)
        result = *lexer->hi;
    else
        result = 0;

    return result;
}

inline void
advance(Lexer *lexer)
{
    lexer->lo = lexer->hi++;
}

inline char
peek(Lexer *lexer)
{
    char result;
    result = *lexer->hi;
    return result;
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
                push_token_and_advance(&lx, END_OF_FILE);
                should_continue = 0;
            } break;

            case '(': push_token_and_advance(&lx, LPAREN); break;
            case ')': push_token_and_advance(&lx, RPAREN); break;
            case '[': push_token_and_advance(&lx, LBRACKET); break;
            case ']': push_token_and_advance(&lx, RBRACKET); break;
            case '{': push_token_and_advance(&lx, LBRACE); break;
            case '}': push_token_and_advance(&lx, RBRACE); break;

            case '+':
            {
                switch (peek_safe(&lx))
                {
                    case '+':
                    {
                        ++lx.hi;
                        push_token_and_advance(&lx, PLUS_PLUS);
                    } break;

                    case '=':
                    {
                        ++lx.hi;
                        push_token_and_advance(&lx, PLUS_EQUAL);
                    } break;

                    default:
                    {
                        push_token_and_advance(&lx, PLUS);
                    } break;
                }
            } break;

            case '-':
            {
                switch (peek_safe(&lx))
                {
                    case '-':
                    {
                        ++lx.hi;
                        push_token_and_advance(&lx, MINUS_MINUS);
                    } break;

                    case '=':
                    {
                        ++lx.hi;
                        push_token_and_advance(&lx, MINUS_EQUAL);
                    } break;

                    default:
                    {
                        push_token_and_advance(&lx, MINUS);
                    } break;
                }
            } break;

            case '*':
            {
                switch (peek_safe(&lx))
                {
                    case '=':
                    {
                        ++lx.hi;
                        push_token_and_advance(&lx, STAR_EQUAL);
                    } break;

                    default:
                    {
                        push_token_and_advance(&lx, STAR);
                    } break;
                }
            } break;

            case '/':
            {
                switch (peek_safe(&lx))
                {
                    case '/':
                    {
                        ++lx.hi;
                        while (lx.hi < lx.end &&
                               !is_next_line(*lx.hi))
                        {
                            ++lx.hi;
                        }

#if _INTERNAL
                        push_token_and_advance(&lx, DEBUG_SINGLE_LINE_COMMENT);
#else
                        lx.lo = lx.hi++;
#endif
                    } break;

                    case '*':
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
                                    push_token_and_advance(&lx, DEBUG_MULTI_LINE_COMMENT);
#else
                                    lx.lo = lx.hi++;
#endif
                                    break;
                                }
                            }
                        }
                    } break;

                    case '=':
                    {
                        ++lx.hi;
                        push_token_and_advance(&lx, SLASH_EQUAL);
                    } break;

                    default:
                    {
                        push_token_and_advance(&lx, SLASH);
                    } break;
                }
            } break;

            default:
            {
                if (is_whitespace(c)) // @NOTE: if you want the info of current line, separate this.
                {
                    lx.lo = lx.hi++;
                }
                else if (is_alphabet_or_underscore(c))
                {
                }
                else if (is_digit(c))
                {
                }
                else
                {
                    // @TODO: ERROR
                    lx.lo = lx.hi++;
                }
            } break;
        }
    }
}

int main(void)
{
    const char *file_name = "main.ember";
    Buffer code = read_entire_file_and_null_terminate(file_name);
    if (code.size)
    {
        tokenize(code);
    }
    else
    {
        fprintf(stderr, "ERROR: Couldn't read file %s.\n", file_name);
    }

    return 0;
}
