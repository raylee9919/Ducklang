/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Sung Woo Lee $
   $Notice: (C) Copyright 2024 by Sung Woo Lee. All Rights Reserved. $
   ======================================================================== */

#define DEBUG_min_prec -1219


struct Parser {
    Parser(Token_List *_token_list) {
        token_list = _token_list;
        next = token_list->base;
    }

    Token_List *token_list;
    Token *next;
};

enum Ast_Type : u8 {
    AST_STATEMENT,
    AST_BLOCK,

    AST_TYPE_Print,

    AST_IDENTIFIER,
    AST_TYPE_String,
    AST_S32,

    AST_BINARY_DECLARE,
    AST_BINARY_ADD,
    AST_BINARY_SUB,
    AST_BINARY_MUL,
    AST_BINARY_DIV,
    AST_BINARY_GREATER,
    AST_BINARY_LESS,
};

////////////////////////////////
// Ast Nodes
//

struct Ast_Node {
    Ast_Type type;
    void *data;

    // @TODO: fuck OOP.
    union {
        s64 int_val;
        f32 f32_val;
        f64 f64_val;
        String identifier;
    };
    union {
        Ast_Node *child;
        struct {
            Ast_Node *left;
            Ast_Node *right;
        };
    };
};

struct Ast_String {
    String str;
};

struct Ast_Print {
    Ast_Node *str;
    std::vector<Ast_Node *> expressions;
};

#define new_ast(TYPE) _new_ast(AST_TYPE_##TYPE, sizeof(Ast_##TYPE))
Ast_Node *_new_ast(Ast_Type type, size_t size) {
    Ast_Node *result = (Ast_Node *)malloc(sizeof(Ast_Node));
    result->type = type;
    result->data = malloc(size);
    memset(result->data, 0, size);
    return result;
}

////////////////////////////////////////

struct Local { 
    Token name;
    s32 depth;
};

struct Compiler {
    s32 scope_depth; // zero is global scope.
};

static Compiler compiler;

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
        case Token_Type::COLON_EQUAL: { node->type = AST_BINARY_DECLARE; } break;
        case Token_Type::PLUS: { node->type = AST_BINARY_ADD; } break;
        case Token_Type::MINUS: { node->type = AST_BINARY_SUB; } break;
        case Token_Type::STAR: { node->type = AST_BINARY_MUL; } break;
        case Token_Type::SLASH: { node->type = AST_BINARY_DIV; } break;
        case Token_Type::GREATER: { node->type = AST_BINARY_GREATER; } break;
        case Token_Type::LESS: { node->type = AST_BINARY_LESS; } break;

        INVALID_DEFAULT_CASE;
    }
    
    return node;
}

static s32
begin_block() {
    return ++compiler.scope_depth;
}

static void
end_block() {
    --compiler.scope_depth;
}

static Ast_Node *
parse_leaf(Parser &parser) {
    Ast_Node *node = new Ast_Node{}; // @TODO: mem management

    Token tk = *parser.next++;
    switch (tk.type) {
        case Token_Type::IDENTIFIER: {
            node->type = AST_IDENTIFIER;
            node->identifier = tk.literal;
        } break;

        case Token_Type::NUMBER: {
            node->type = AST_S32;
            node->int_val = string_to_s32(tk.literal);
        } break;

        case Token_Type::STRING: {
            node = new_ast(String);
            Ast_String *data= (Ast_String *)node->data;
            data->str = tk.literal;
        } break;

        INVALID_DEFAULT_CASE;
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
        if (parser.next->type == Token_Type::SEMICOLON ||
            parser.next->type == Token_Type::COMMA ||
            parser.next->type == Token_Type::RPAREN) { break; } // @TODO: is it a right place?
        Ast_Node *node = parse_increasing_precedence(parser, left, min_prec);
        if (node == left) { break; }
        left = node;
    }

    return left;
}

static Ast_Node *parse_statement(Parser &parser);

static Ast_Node *
parse_block(Parser &parser) {
    Ast_Node *block_node = new Ast_Node{};
    block_node->type = AST_BLOCK;


    ++parser.next;
    s32 depth = begin_block();
    while (!(compiler.scope_depth == depth &&
             parser.next->type == Token_Type::RBRACE)) 
    {
        block_node->child = parse_statement(parser); // @TODO: need dynamic array...?
    }
    end_block();
    ++parser.next;

    return block_node;
}

static Ast_Node *
parse_print(Parser &parser) {
    Ast_Node *node = new_ast(Print);
    Ast_Print *data = (Ast_Print *)node->data;

    ++parser.next;
    if (parser.next->type == Token_Type::LPAREN) {
        ++parser.next;
        if (parser.next->type == Token_Type::STRING) {
            // @TODO
            data->str = parse_leaf(parser);
            for (;;) {
                if (parser.next->type == Token_Type::COMMA) {
                    ++parser.next;
                    data->expressions.push_back(parse_expression(parser, DEBUG_min_prec));
                } else break;
            }
        } else if (parser.next->type == Token_Type::NUMBER) {
            Ast_Node *l = new_ast(String);
            Ast_String * s = (Ast_String *)l->data;
            s->str = _String("%");
            data->str = l;
            data->expressions.push_back(parse_expression(parser, DEBUG_min_prec));
        } else if (parser.next->type == Token_Type::IDENTIFIER) {
            Ast_Node *l = new_ast(String);
            Ast_String * s = (Ast_String *)l->data;
            s->str = _String("%");
            data->str = l;
            data->expressions.push_back(parse_expression(parser, DEBUG_min_prec));
        } else {
            ASSERT(0);
        }

        if (parser.next->type == Token_Type::RPAREN) {
            ++parser.next;
        } else {
            ASSERT(0);
        }
    } else {
        ASSERT(0);
    }

    return node;
}

static Ast_Node *
parse_statement(Parser &parser) 
{
    Ast_Node *statement = new Ast_Node{};
    statement->type = AST_STATEMENT;

    switch (parser.next->type) {
        case Token_Type::SEMICOLON: {
            ++parser.next;
        } break;

        case Token_Type::IDENTIFIER: {
            Token peek = *(parser.next + 1);
            if (peek.type == Token_Type::COLON_EQUAL) {
                statement->child = parse_expression(parser, DEBUG_min_prec);
            } else if (peek.type == Token_Type::EQUAL) {
                ASSERT(0);
            } else {
                ASSERT(0);
            }
        } break;

        case Token_Type::NUMBER: {
            statement->child = parse_expression(parser, DEBUG_min_prec);
            ++parser.next;
        } break;

        case Token_Type::KEYWORD: {
            if (parser.next->literal == "s32") {
                ++parser.next;
                statement->child = parse_expression(parser, DEBUG_min_prec);
            } else if(parser.next->literal == "print" ||
                      parser.next->literal == "quack" ) { // @NOTE: quack isn't a good idea.
                statement->child = parse_print(parser);
            } else {
                ASSERT(0);
            }
        } break;

        case Token_Type::LBRACE: {
            statement->child = parse_block(parser);
        } break;

        INVALID_DEFAULT_CASE;
    }

    return statement;
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

struct Eval_Result {
    Eval_Type type;
    s32 offset;
    u32 length;
#if 0
    union {
        s64 int_val;
        f32 f32_val;
        f64 f64_val;
    };
#endif
};

