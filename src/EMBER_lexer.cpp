/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Sung Woo Lee $
   $Notice: (C) Copyright 2024 by Sung Woo Lee. All Rights Reserved. $
   ======================================================================== */

#include "EMBER_lexer.h"

#define push_token_and_advance(LEXER, TYPE, CONTAINER) _push_token_and_advance(LEXER, TYPE, #TYPE, CONTAINER)
static void
_push_token_and_advance(Lexer *lexer, Token_Type type, const char *type_literal,
                        Token_List *tokens)
{
    Token token = {};
    token.type = type;
    token.type_literal = __String__(type_literal, string_length(type_literal));
    token.literal = __String__(lexer->lo, (lexer->hi - lexer->lo));

    push_token(tokens, token);

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
tokenize(Buffer code, Token_List *tokens)
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
                push_token_and_advance(&lx, END_OF_FILE, tokens);
                should_continue = 0;
            } break;

            case '(': push_token_and_advance(&lx, LPAREN, tokens); break;
            case ')': push_token_and_advance(&lx, RPAREN, tokens); break;
            case '[': push_token_and_advance(&lx, LBRACKET, tokens); break;
            case ']': push_token_and_advance(&lx, RBRACKET, tokens); break;
            case '{': push_token_and_advance(&lx, LBRACE, tokens); break;
            case '}': push_token_and_advance(&lx, RBRACE, tokens); break;

            case '+':
            {
                switch (peek_safe(&lx))
                {
                    case '+':
                    {
                        ++lx.hi;
                        push_token_and_advance(&lx, PLUS_PLUS, tokens);
                    } break;

                    case '=':
                    {
                        ++lx.hi;
                        push_token_and_advance(&lx, PLUS_EQUAL, tokens);
                    } break;

                    default:
                    {
                        push_token_and_advance(&lx, PLUS, tokens);
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
                        push_token_and_advance(&lx, MINUS_MINUS, tokens);
                    } break;

                    case '=':
                    {
                        ++lx.hi;
                        push_token_and_advance(&lx, MINUS_EQUAL, tokens);
                    } break;

                    case '>':
                    {
                        ++lx.hi;
                        push_token_and_advance(&lx, RARROW, tokens);
                    } break;
            
                    default:
                    {
                        push_token_and_advance(&lx, MINUS, tokens);
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
                        push_token_and_advance(&lx, STAR_EQUAL, tokens);
                    } break;

                    default:
                    {
                        push_token_and_advance(&lx, STAR, tokens);
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
                        push_token_and_advance(&lx, COLON_EQUAL, tokens);
                    } break;

                    default:
                    {
                        push_token_and_advance(&lx, COLON, tokens);
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
                        push_token_and_advance(&lx, EQUAL_EQUAL, tokens);
                    } break;

                    default:
                    {
                        push_token_and_advance(&lx, EQUAL, tokens);
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
                        push_token_and_advance(&lx, DEBUG_SINGLE_LINE_COMMENT, tokens);
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
                                    push_token_and_advance(&lx, DEBUG_MULTI_LINE_COMMENT, tokens);
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
                        push_token_and_advance(&lx, SLASH_EQUAL, tokens);
                    } break;

                    default:
                    {
                        push_token_and_advance(&lx, SLASH, tokens);
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
                push_token_and_advance(&lx, STRING, tokens);
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
                        push_token_and_advance(&lx, KEYWORD, tokens);
                    }
                    else
                    {
                        push_token_and_advance(&lx, IDENTIFIER, tokens);
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
