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

const char *bytecode = "output.eb";

#define WRITE_MEMBER(MEM) fwrite(&MEM, sizeof(MEM), 1, file)

#pragma pack(push, 1)

enum Opcode : u8 {
    OP_RETURN,
    OP_ADD,
    OP_PUSH_S32_IMM,
    OP_PUSH_S32_OFF,

    OP_STRING,
    OP_PRINT,
};

struct Operation {
    Opcode opcode;
    virtual void write_to_file(FILE *file) {}
};

struct Operation_Print : Operation {
    Operation_Print(s32 _str_offset, s32 _expr_type, s32 _expr_offset) {
        opcode = OP_PRINT;
        str_offset  = _str_offset;
        expr_type   = _expr_type;
        expr_offset = _expr_offset;
    }
    void write_to_file(FILE *file) override {
        WRITE_MEMBER(opcode);
        WRITE_MEMBER(str_offset);
        WRITE_MEMBER(expr_type);
        WRITE_MEMBER(expr_offset);
    }

    s32 str_offset;
    s32 expr_type;
    s32 expr_offset;
};

struct Operation_String : Operation {
    Operation_String(u32 _length, const char *_str) {
        opcode  = OP_STRING;
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

struct Operation_Add : Operation {
    Operation_Add(s32 l, s32 r) {
        opcode = OP_ADD;
        loff = l;
        roff = r;
    }
    void write_to_file(FILE *file) override {
        WRITE_MEMBER(opcode);
        WRITE_MEMBER(loff);
        WRITE_MEMBER(roff);
    }

    s32 loff;
    s32 roff;
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



#endif // EMBER_OPERATION_H_
