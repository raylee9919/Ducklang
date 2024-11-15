#ifndef EMBER_LEXER_H_
#define EMBER_LEXER_H_
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Sung Woo Lee $
   $Notice: (C) Copyright 2024 by Sung Woo Lee. All Rights Reserved. $
   ======================================================================== */

enum Token_Type
{
    INVALID,
    END_OF_FILE,

    LPAREN, RPAREN,
    LBRACE, RBRACE,
    LBRACKET,
    RBRACKET,
    PLUS, PLUS_PLUS, PLUS_EQUAL,
    MINUS, MINUS_MINUS, MINUS_EQUAL, RARROW,
    STAR, STAR_EQUAL,
    SLASH, SLASH_EQUAL,
    PERIOD, COMMA, COLON, SEMICOLON,

    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,
    COLON_EQUAL,

    KEYWORD, IDENTIFIER, STRING, NUMBER,

#if __INTERNAL
    DEBUG_SINGLE_LINE_COMMENT,
    DEBUG_MULTI_LINE_COMMENT,
#endif

    TOKEN_TYPE_COUNT,
};

struct Token
{
    Token_Type type;
    String type_literal;
    String literal;
};

struct Token_List
{
    void init(Token_List *token_list, size_t token_count);
    Token *base;
    size_t size; // full bytes.
    size_t used;
    size_t count;
};

struct Lexer
{
    char *lo;
    char *hi;
    char *end;
};


#endif // EMBER_LEXER_H_
