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
#include "SHARED_operation.h"
#include "EMBER_compiler.cpp"
#include "EMBER_hashmap.cpp"
#include "EMBER_cli.cpp"


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

        case AST_TYPE_Print: {
            Ast_Print *data = (Ast_Print *)node->data;
            Eval_Result str = eval(data->str);
            u32 expr_count = (u32)data->expressions.size();

            Type_Offset *type_offset = (Type_Offset *)malloc(expr_count*sizeof(Type_Offset));
            Type_Offset *at = type_offset;
            for (Ast_Node *expr : data->expressions) {
                Eval_Result res = eval(expr);
                at->type    = res.type;
                at->offset  = res.offset;
                ++at;
            }

            Operation *op = new Operation_Print(str.offset, str.length, expr_count, type_offset);
            exe.push(op);
        } break;

        case AST_BINARY_ADD: {
            Eval_Result l = eval(node->left);
            Eval_Result r = eval(node->right);

            if (l.type == EVAL_S32 && l.type == EVAL_S32) {
                Operation *op = new Operation_ADD(l.offset, r.offset);
                exe.push(op);

                result.type     = EVAL_S32;
                result.offset   = stack_used;
                result.length   = 4;

                stack_used += 4;
            } else {
                ASSERT(0);
            }
        } break;

        case AST_BINARY_SUB: {
            Eval_Result l = eval(node->left);
            Eval_Result r = eval(node->right);

            if (l.type == EVAL_S32 && l.type == EVAL_S32) {
                Operation *op = new Operation_SUB(l.offset, r.offset);
                exe.push(op);

                result.type     = EVAL_S32;
                result.offset   = stack_used;
                result.length   = 4;

                stack_used += 4;
            } else {
                ASSERT(0);
            }
        } break;

        case AST_BINARY_MUL: {
            Eval_Result l = eval(node->left);
            Eval_Result r = eval(node->right);

            if (l.type == EVAL_S32 && l.type == EVAL_S32) {
                Operation *op = new Operation_MUL(l.offset, r.offset);
                exe.push(op);

                result.type     = EVAL_S32;
                result.offset   = stack_used;
                result.length   = 4;

                stack_used += 4;
            } else {
                ASSERT(0);
            }
        } break;

        case AST_BINARY_DIV: {
            Eval_Result l = eval(node->left);
            Eval_Result r = eval(node->right);

            if (l.type == EVAL_S32 && l.type == EVAL_S32) {
                Operation *op = new Operation_DIV(l.offset, r.offset);
                exe.push(op);

                result.type     = EVAL_S32;
                result.offset   = stack_used;
                result.length   = 4;

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
                val.type    = EVAL_S32;
                val.offset  = stack_used;
                val.length  = 4;
                hashmap.push(name, val);

                Operation *op = new Operation_Push_S32_Off(e.offset);
                exe.push(op);

                result.type     = EVAL_S32;
                result.offset   = stack_used;
                result.length   = 4;

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

        case AST_TYPE_String: {
            Ast_String *data = (Ast_String *)node->data;
            String str = data->str;
            result.type = EVAL_STRING;
            result.offset = stack_used;
            result.length = (u32)str.length;

            Operation *op = new Operation_Push_String((u32)str.length, str.data);
            exe.push(op);

            stack_used += (u32)str.length;
        } break;

        case AST_S32: {
            result.type     = EVAL_S32;
            result.offset   = stack_used;
            result.length   = 4;

            Operation *op = new Operation_Push_S32_Imm((s32)node->int_val);
            exe.push(op);

            stack_used += 4;
        } break;

        case AST_IDENTIFIER: {
            Hashmap_Val hash = hashmap.get(node->identifier);
            result.type     = hash.type;
            result.offset   = hash.offset;
            result.length   = hash.length;
            ASSERT(result.offset != -1);
        } break;

        INVALID_DEFAULT_CASE;
    }

    return result;
}

int main(int argc, char **argv) {
    u64 begin_timer = get_os_timer();
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
                u64 end_timer = get_os_timer();
                f32 build_time = ((f32)(end_timer - begin_timer) / (f32)get_os_timer_frequency());
                printf("\nBuild done in %.5f seconds.\n", build_time);
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
