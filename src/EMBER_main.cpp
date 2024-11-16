/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Sung Woo Lee $
   $Notice: (C) Copyright 2024 by Sung Woo Lee. All Rights Reserved. $
   ======================================================================== */

#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <string>
#include <unordered_map>

#include "EMBER_types.h"
#include "EMBER_buffer.h"
#include "EMBER_utility.cpp"
#include "EMBER_string.cpp"
#include "EMBER_platform.cpp"
#include "EMBER_lexer.cpp"
#include "EMBER_compiler.cpp"
#include "EMBER_hashmap.cpp"
#include "EMBER_cli.cpp"

#include "SHARED_operation.h"

struct Exe {
    void push(Operation *op) {
        operations.push_back(op);
    }

    s32 write_to_file(const char *filepath) {
        FILE *file = fopen(filepath, "wb");
        if (file) {
            for (Operation *op : operations) {
                op->write_to_file(file); 
            }
            return 0;
        } else {
            return 1;
        }
    }

    std::vector<Operation *> operations;
};


static Hashmap hashmap;
static u32 stack_used;
static Exe exe;


static Eval_Result
eval(Ast_Node *node) {
    ASSERT(node);
    Eval_Result result;

    switch (node->type) {
        case AST_STATEMENT: {
            if (node->child) eval(node->child);
        } break;

        case AST_PRINT: {
            Eval_Result str  = eval(node->left);
            Eval_Result expr = eval(node->right);

            Operation *op = new Operation_Print(str.offset, expr.type, expr.offset);
            exe.push(op);
        } break;

        case AST_BINARY_PLUS: {
            Eval_Result l = eval(node->left);
            Eval_Result r = eval(node->right);

            if (l.type == EVAL_S32 && l.type == EVAL_S32) {
                Operation *op = new Operation_Add(l.offset, r.offset);
                exe.push(op);

                result.type = EVAL_S32;
                result.offset = stack_used;

                stack_used += 4;
            } else {
                ASSERT(0);
            }
        } break;

        case AST_BINARY_DECLARE: {
            String name = node->left->identifier;
            Eval_Result e = eval(node->right);
            Hashmap_Val val;

            if (e.type == EVAL_S32) {
                val.type = EVAL_S32;
                val.offset = stack_used;
                hashmap.push(name, val);

                Operation *op = new Operation_Push_S32_Off(e.offset);
                exe.push(op);

                result.type = EVAL_S32;
                result.offset = stack_used;

                stack_used += 4;
            } else {
                ASSERT(0);
            }
        } break;

#if 0
        case AST_BLOCK: {
            ++compiler.scope_depth;
            for (st : node->statements) {
                compile(st);
            }
            --compiler.scope_depth;
        } break;
#endif

        case AST_STRING: {
            String str = node->str;
            Operation *op = new Operation_String((u32)str.length, str.data);
            exe.push(op);
        } break;

        case AST_S32: {
            result.type = EVAL_S32;
            result.offset = stack_used;

            Operation *op = new Operation_Push_S32_Imm((s32)node->int_val);
            exe.push(op);

            stack_used += 4;
        } break;

        case AST_IDENTIFIER: {
            Hashmap_Val hash = hashmap.get(node->identifier);
            result.type = hash.type;
            result.offset = hash.offset;
            ASSERT(result.offset != -1);
        } break;

        INVALID_DEFAULT_CASE;
    }

    return result;
}

int main(int argc, char **argv) {
    f32 build_time = 0.0f;
    print_logo();

    const char *file_name = argv[1];
    printf("%s\n", file_name);

    hashmap.init(256);
    Buffer code = read_entire_file_and_null_terminate(file_name);
    if (code.size) {
        Token_List token_list = {};
        token_list.init(&token_list, MB(1));

        if (token_list.base) {
            tokenize(code, &token_list);
#if 0
            DEBUG_print_tokens(&token_list);
#endif
            std::vector<Ast_Node *> statements = parse(&token_list);
            for (Ast_Node *statement : statements) { eval(statement); }


            if (!exe.write_to_file(bytecode)) {
                printf("Build done in %.4fs.\n", build_time);
                return 0;
            } else {
                fprintf(stderr, "ERROR: Couldn't write to file %s.\n", bytecode);
                return 1;
            }
        } else {
            fprintf(stderr, "ERROR: Failed to malloc token list.\n");
            return 1;
        }
    } else {
        fprintf(stderr, "ERROR: Buffer's size is 0. (%s).\n", file_name);
        return 1;
    }
}
