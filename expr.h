#ifndef EXPR_H
#define EXPR_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "data.h"
#include "parse.h"
#include "error.h"

Atom env_create(Atom parent);
Error env_set(Atom env, Atom sym, Atom val);
Error eval_expr(Atom expr, Atom env, Atom *result);

#endif
