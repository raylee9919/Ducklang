#ifndef EMBER_TOKEN_H_
#define EMBER_TOKEN_H_
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

    TOKEN_COUNT,
};

struct Token
{
    Token_Type type;
    const char *type_literal;

    const char *literal;
};

#endif // EMBER_TOKEN_H_
