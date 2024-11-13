#ifndef EMBER_OPERATION_H_
#define EMBER_OPERATION_H_
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Sung Woo Lee $
   $Notice: (C) Copyright 2024 by Sung Woo Lee. All Rights Reserved. $
   ======================================================================== */

enum Opcode : unsigned char
{
    OP_RETURN,
    OP_ADD,
    OP_SUB,
    OP_MUL,
};

struct Operation
{
    Opcode opcode;
};



#endif // EMBER_OPERATION_H_
