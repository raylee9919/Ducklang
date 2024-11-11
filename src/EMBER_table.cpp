#ifndef EMBER_TABLE_H_
#define EMBER_TABLE_H_
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Sung Woo Lee $
   $Notice: (C) Copyright 2024 by Sung Woo Lee. All Rights Reserved. $
   ======================================================================== */

static void
unary()
{
}

static void
binary()
{
}

typedef void Parse_Function();

struct Parse_Rule
{
    Parse_Function *prefix;
    Parse_Function *infix;
};

static Parse_Rule parse_rule_table[TOKEN_TYPE_COUNT];

// @TODO: This ain't done.
static void
init_parse_rule_table()
{
    parse_rule_table[INVALID]           = {0,       0};
    parse_rule_table[END_OF_FILE]       = {0,       0};
                                                    
    parse_rule_table[LPAREN]            = {0,       0};
    parse_rule_table[RPAREN]            = {0,       0};
    parse_rule_table[LBRACE]            = {0,       0};
    parse_rule_table[RBRACE]            = {0,       0};
    parse_rule_table[LBRACKET]          = {0,       0};
    parse_rule_table[RBRACKET]          = {0,       0};
    parse_rule_table[PLUS]              = {0,       binary};
    parse_rule_table[PLUS_PLUS]         = {unary,   0};
    parse_rule_table[PLUS_EQUAL]        = {0,       binary};
    parse_rule_table[MINUS]             = {unary,   binary};
    parse_rule_table[MINUS_MINUS]       = {0,       0};
    parse_rule_table[MINUS_EQUAL]       = {0,       0};
    parse_rule_table[RARROW]            = {0,       0};
    parse_rule_table[STAR]              = {unary,   binary};
    parse_rule_table[STAR_EQUAL]        = {0,       0};
    parse_rule_table[SLASH]             = {0,       binary};
    parse_rule_table[SLASH_EQUAL]       = {0,       0};
    parse_rule_table[PERIOD]            = {0,       0};
    parse_rule_table[COMMA]             = {0,       0};
    parse_rule_table[COLON]             = {0,       0};
    parse_rule_table[SEMICOLON]         = {0,       0};

    parse_rule_table[BANG]              = {0,       0};
    parse_rule_table[BANG_EQUAL]        = {0,       0};
    parse_rule_table[EQUAL]             = {0,       0};
    parse_rule_table[EQUAL_EQUAL]       = {0,       0};
    parse_rule_table[GREATER]           = {0,       0};
    parse_rule_table[GREATER_EQUAL]     = {0,       0};
    parse_rule_table[LESS]              = {0,       0};
    parse_rule_table[LESS_EQUAL]        = {0,       0};
    parse_rule_table[COLON_EQUAL]       = {0,       0};

    parse_rule_table[KEYWORD]           = {0,       0};
    parse_rule_table[IDENTIFIER]        = {0,       0};
    parse_rule_table[STRING]            = {0,       0};
    parse_rule_table[NUMBER]            = {0,       0};
}


#endif // EMBER_TABLE_H_
