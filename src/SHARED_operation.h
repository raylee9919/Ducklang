#ifndef EMBER_OPERATION_H_
#define EMBER_OPERATION_H_
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Sung Woo Lee $
   $Notice: (C) Copyright 2024 by Sung Woo Lee. All Rights Reserved. $
   ======================================================================== */

#include <stdint.h>

const char *bytecode = "../data/output.eb";

#define WRITE_MEMBER(MEM) fwrite(&MEM, sizeof(MEM), 1, file)
#define DEFINE_BINARY_OPERATION(OP) \
    struct Operation_##OP : Operation { \
        Operation_##OP(s32 l, s32 r) { \
            opcode = OP_##OP; \
            loff = l; \
            roff = r; \
        } \
        void write_to_file(FILE *file) override { \
            WRITE_MEMBER(opcode); \
            WRITE_MEMBER(loff); \
            WRITE_MEMBER(roff); \
        } \
        \
        s32 loff; \
        s32 roff; \
    };


#pragma pack(push, 1)

enum Opcode : u8 {
    OP_RETURN,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_PUSH_S32_IMM,
    OP_PUSH_S32_OFF,

    OP_PUSH_STRING,
    OP_PRINT,
};

struct Operation {
    Opcode opcode;
    virtual void write_to_file(FILE *file) {}
};

DEFINE_BINARY_OPERATION(ADD);
DEFINE_BINARY_OPERATION(SUB);
DEFINE_BINARY_OPERATION(MUL);
DEFINE_BINARY_OPERATION(DIV);

struct Type_Offset {
    s32 type;
    s32 offset;
};
struct Operation_Print : Operation {
    Operation_Print(s32 _str_offset, u32 _str_length, u32 _expr_count, Type_Offset *_type_offset) {
        opcode = OP_PRINT;
        str_offset  = _str_offset;
        str_length  = _str_length;
        expr_count  = _expr_count;
        type_offset = _type_offset;
    }
    void write_to_file(FILE *file) override {
        WRITE_MEMBER(opcode);
        WRITE_MEMBER(str_offset);
        WRITE_MEMBER(str_length);
        WRITE_MEMBER(expr_count);

        for (u32 i = 0; i < expr_count; ++i) {
            fwrite(type_offset + i, sizeof(Type_Offset), 1, file);
        }
    }

    s32 str_offset;
    u32 str_length;
    u32 expr_count;
    Type_Offset *type_offset;
};

struct Operation_Push_String : Operation {
    Operation_Push_String(u32 _length, const char *_str) {
        opcode  = OP_PUSH_STRING;
        length  = _length;
        str     = _str;
    }
    void write_to_file(FILE *file) override {
        WRITE_MEMBER(opcode);
        WRITE_MEMBER(length);
        fwrite(str, length, 1, file);
    }

    u32 length;
    const char *str;
};

struct Operation_Push_S32_Imm : Operation {
    Operation_Push_S32_Imm(s32 _val) {
        opcode = OP_PUSH_S32_IMM;
        val = _val;
    }
    void write_to_file(FILE *file) override {
        WRITE_MEMBER(opcode);
        WRITE_MEMBER(val);
    }

    s32 val;
};

struct Operation_Push_S32_Off : Operation {
    Operation_Push_S32_Off(s32 _offset) {
        opcode = OP_PUSH_S32_OFF;
        offset = _offset;
    }
    void write_to_file(FILE *file) override {
        WRITE_MEMBER(opcode);
        WRITE_MEMBER(offset);
    }

    s32 offset;
};

#pragma pack(pop)

enum Eval_Type : s32 {
    EVAL_ERROR = -1,

    EVAL_S32,
    EVAL_STRING,
    EVAL_VAR,
};

#endif // EMBER_OPERATION_H_
