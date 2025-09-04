#ifndef JD297_CC_CODEGEN_H
#define JD297_CC_CODEGEN_H

#include <stdio.h>

#include "ir.h"

typedef int (*codegen_run_func)(IR_CTX *ctx, FILE *output);

extern int codegen_x86_64_run(IR_CTX *ctx, FILE *output);

extern int codegen_aarch64_run(IR_CTX *ctx, FILE *output);

#endif
