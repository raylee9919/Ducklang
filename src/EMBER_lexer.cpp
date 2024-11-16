/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Sung Woo Lee $
   $Notice: (C) Copyright 2024 by Sung Woo Lee. All Rights Reserved. $
   ======================================================================== */

#include "EMBER_lexer.h"

static void
push_token(Token_List *token_list, Token token)
{
    size_t new_size = sizeof(Token);
    ASSERT(token_list->used + new_size <= token_list->size);
    *(Token *)((u8 *)token_list->base + token_list->used) = token;
    token_list->used += new_size;
    ++token_list->count;
}

#define push_token_and_advance(LEXER, TYPE, CONTAINER) _push_token_and_advance(LEXER, TYPE, #TYPE, CONTAINER)
static void
_push_token_and_advance(Lexer *lexer, Token_Type type, const char *type_literal,
                        Token_List *tokens)
{
    Token token = {};
    token.type = type;
    token.type_literal = _String(type_literal);
    token.literal = _String(lexer->lo, (lexer->hi - lexer->lo));

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
advance(Lexer *lexer) { lexer->lo = lexer->hi++; }

static void
fill_keyword_hashmap(std::unordered_map<std::string, Token_Type> &hashmap)
{
    std::string keywords[] = {
        "and", "or", "for", "if", "else",
        "s8", "u8", "s16", "u16", "s32", "u32", "s64", "u64",
        "fn", "string", 
        "print", "quack"
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
            case ';': push_token_and_advance(&lx, SEMICOLON, tokens); break;

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

            case '>':
            {
                switch (peek_safe(&lx))
                {
                    case '=':
                    {
                        ++lx.hi;
                        push_token_and_advance(&lx, GREATER_EQUAL, tokens);
                    } break;

                    default:
                    {
                        push_token_and_advance(&lx, GREATER, tokens);
                    } break;
                }
            } break;

            case '<':
            {
                switch (peek_safe(&lx))
                {
                    case '=':
                    {
                        ++lx.hi;
                        push_token_and_advance(&lx, LESS_EQUAL, tokens);
                    } break;

                    default:
                    {
                        push_token_and_advance(&lx, LESS, tokens);
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

#if __INTERNAL
                        push_token_and_advance(&lx, DEBUG_SINGLE_LINE_COMMENT, tokens);
#else
                        lx.lo = lx.hi++;
#endif
                    } break;

                    // @TODO: nested multi-line comment.
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
#if __INTERNAL
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
                else if (is_digit(c)) // Number
                {
                    while (is_digit(peek_safe(&lx))) { ++lx.hi; }
                    push_token_and_advance(&lx, NUMBER, tokens);
                }
                else
                {
                    // @TODO: ERROR. But for now, just keep going.
                    lx.lo = lx.hi++;
                }
            } break;
        }
    }
}

void
Token_List::init(Token_List *token_list, size_t token_count)
{ 
    token_list->size = sizeof(Token_List) * token_count;
    token_list->base = (Token *)malloc(token_list->size);
    token_list->used = 0;
    token_list->count = 0;
}

static void
DEBUG_print_tokens(Token_List *token_list)
{
    for (u32 idx = 0;
         idx < token_list->count;
         ++idx)
    {
        Token tk = token_list->base[idx];
        printf("%.*s (%.*s)\n", (s32)tk.literal.length, tk.literal.data,
               (s32)tk.type_literal.length, tk.type_literal.data);
    }
}

static b32
is_binary_operator(Token token) {
    return ((token.type == Token_Type::COLON_EQUAL) ||
            (token.type == Token_Type::PLUS) ||
            (token.type == Token_Type::MINUS) ||
            (token.type == Token_Type::STAR) ||
            (token.type == Token_Type::SLASH) ||
            (token.type == Token_Type::LESS) ||
            (token.type == Token_Type::GREATER));
}

static s32
get_precedence(Token token) {
    s32 result;
    switch (token.type) {
        case Token_Type::COLON_EQUAL: { result = 1; } break;

        case Token_Type::LESS:
        case Token_Type::GREATER: { result = 2; } break;

        case Token_Type::PLUS:
        case Token_Type::MINUS: { result = 3; } break;

        case Token_Type::STAR:
        case Token_Type::SLASH: { result = 4; } break;

        INVALID_DEFAULT_CASE;
    }

    return result;
}

