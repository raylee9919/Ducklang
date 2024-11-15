/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Sung Woo Lee $
   $Notice: (C) Copyright 2024 by Sung Woo Lee. All Rights Reserved. $
   ======================================================================== */

#define DEBUG_min_prec -1219

struct Parser {
    Parser(Token_List *token_list) {
        this->token_list = token_list;
        this->next = token_list->base;
    }

    Token_List *token_list;
    Token *next;
};

enum Ast_Node_Type : u8 {
    AST_STATEMENT,

    AST_S32,

    AST_BINARY_PLUS,
    AST_BINARY_MINUS,
    AST_BINARY_MUL,
    AST_BINARY_DIV,
    AST_BINARY_GREATER,
    AST_BINARY_LESS,
};

struct Ast_Node {
    Ast_Node_Type type;
    s32 val;
    Ast_Node *left;
    Ast_Node *right;
};

static Ast_Node *parse_expression(Parser &parser, s32 min_prec);

static Ast_Node *
make_binary(Ast_Node *left, Ast_Node *binary, Ast_Node *right) {
    binary->left = left;
    binary->right = right;
    return binary;
}

static Ast_Node *
to_operator(Token token) {
    Ast_Node *node = new Ast_Node{};
    switch (token.type) {
        case Token_Type::PLUS: {
            node->type = AST_BINARY_PLUS;
        } break;

        case Token_Type::MINUS: {
            node->type = AST_BINARY_MINUS;
        } break;

        case Token_Type::STAR: {
            node->type = AST_BINARY_MUL;
        } break;

        case Token_Type::SLASH: {
            node->type = AST_BINARY_DIV;
        } break;

        case Token_Type::GREATER: {
            node->type = AST_BINARY_GREATER;
        } break;

        case Token_Type::LESS: {
            node->type = AST_BINARY_LESS;
        } break;

        INVALID_DEFAULT_CASE;
    }
    
    return node;
}

static Ast_Node *
parse_leaf(Parser &parser) {
    Ast_Node *node = new Ast_Node{}; // @TODO: mem management

    Token tk = *parser.next++;
    if (tk.type == Token_Type::IDENTIFIER) {
        ASSERT(0); // @TODO
    } else if (tk.type == Token_Type::NUMBER) {
        node->type = AST_S32;
        node->val = string_to_s32(tk.literal);
    } else {
        ASSERT(0);
    }

    return node;
}


///////////////////////////////////////////////////
//
// Pratt Parsing
//
static Ast_Node *
parse_increasing_precedence(Parser &parser, Ast_Node *left, s32 min_prec) {
    Ast_Node *result;

    Token next = *parser.next;

    if (is_binary_operator(next)) {
        s32 next_prec = get_precedence(next);

        if (next_prec <= min_prec) {
            result = left;
        } else {
            ++parser.next;
            Ast_Node *right = parse_expression(parser, next_prec);
            result = make_binary(left, to_operator(next), right);
        }
    } else {
        ASSERT(0);
    }

    return result;
}

static Ast_Node *
parse_expression(Parser &parser, s32 min_prec) {
    Ast_Node *left = parse_leaf(parser);

    for (;;) {
        if (parser.next->type == Token_Type::SEMICOLON) { break; } // @TODO: is it a right place?
        Ast_Node *node = parse_increasing_precedence(parser, left, min_prec);
        if (node == left) { break; }
        left = node;
    }

    return left;
}

static Ast_Node *
parse_statement(Parser &parser) 
{
    Ast_Node *result = new Ast_Node{};
    result->type = AST_STATEMENT;

    switch (parser.next->type) {
        case Token_Type::SEMICOLON: {
            ++parser.next;
        } break;

        case Token_Type::IDENTIFIER: {
            ASSERT(0);
        } break;

        case Token_Type::NUMBER: {
            Ast_Node *DEBUG_node = parse_expression(parser, DEBUG_min_prec);
            ++parser.next;
        } break;

        case Token_Type::KEYWORD: {
            if (parser.next->literal == "s32") {
                ++parser.next;
                if (parser.next->type == Token_Type::IDENTIFIER) {
                    ++parser.next;
                    if (parser.next->type == Token_Type::COLON_EQUAL) {
                        ++parser.next;
                        parse_expression(parser, DEBUG_min_prec);
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

    return result;
}

static std::vector<Ast_Node *>
parse(Token_List *token_list) {
    std::vector<Ast_Node *> statements;
    Parser parser(token_list);
    while (parser.next->type != Token_Type::END_OF_FILE) {
        statements.push_back(parse_statement(parser));
    }
    return statements;
}

    
