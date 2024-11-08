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
#include "EMBER_token.h"

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

inline b32
is_alphabet_or_underscore_or_digit(char c)
{
    b32 result = ( (c >= 'a' && c <= 'z') ||
                   (c >= 'A' && c <= 'Z') ||
                   (c == '_') ||
                   (c >= '0' && c <= '9'));
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
peek(Lexer *lexer)
{
    char result;
    result = *lexer->hi;
    return result;
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

static void
fill_keyword_hashmap(std::unordered_map<std::string, Token_Type> &hashmap)
{
    std::string keywords[] = {
        "and", "or", "for", "if", "else",
        "s8", "u8", "s16", "u16", "s32", "u32", "s64", "u64",
        "fn", "string"
    };
    for (std::string keyword : keywords) 
        hashmap[keyword] = Token_Type::KEYWORD;
}

static void
tokenize(Buffer code)
{
    Lexer lx = {};
    lx.lo = (char *)code.data;
    lx.hi = (lx.lo + 1);
    lx.end = ( (char *)code.data + code.size );

    // @TODO: Not a big fan of STL... but for now, it'll do.
    std::unordered_map<std::string, Token_Type> keyword_hashmap;
    fill_keyword_hashmap(keyword_hashmap);

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

                    case '>':
                    {
                        ++lx.hi;
                        push_token_and_advance(&lx, RARROW);
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

            case ':':
            {
                switch (peek_safe(&lx))
                {
                    case '=':
                    {
                        ++lx.hi;
                        push_token_and_advance(&lx, COLON_EQUAL);
                    } break;

                    default:
                    {
                        push_token_and_advance(&lx, COLON);
                    } break;
                }
            } break;

            case '=':
            {
                switch (peek_safe(&lx))
                {
                    case '=':
                    {
                        ++lx.hi;
                        push_token_and_advance(&lx, EQUAL_EQUAL);
                    } break;

                    default:
                    {
                        push_token_and_advance(&lx, EQUAL);
                    } break;
                }
            } break;

            case '/':
            {
                switch (peek_safe(&lx))
                {
                    case '/': // Single-Line Comment
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

                    case '*': // Multi-Line Comment
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

            case '\"':
            {
                while (peek_safe(&lx) != '\"' || *(lx.hi - 1) == '\\')
                {
                    ++lx.hi;
                }
                ++lx.hi;
                push_token_and_advance(&lx, STRING);
            } break;

            default:
            {
                if (is_whitespace(c)) // @NOTE: if you want the info of current line, separate this.
                {
                    lx.lo = lx.hi++;
                }
                else if (is_alphabet_or_underscore(c)) // Identifier
                {
                    while (is_alphabet_or_underscore_or_digit(peek(&lx)))
                    {
                        ++lx.hi;
                    }

                    std::string literal(lx.lo, lx.hi - lx.lo);
                    if (keyword_hashmap.find(literal) != keyword_hashmap.end())
                    {
                        push_token_and_advance(&lx, KEYWORD);
                    }
                    else
                    {
                        push_token_and_advance(&lx, IDENTIFIER);
                    }
                }
                else if (is_digit(c)) // Literal
                {
                    lx.lo = lx.hi++;
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
