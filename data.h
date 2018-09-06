#ifndef DATA_H
#define DATA_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "error.h"

#define car(p) ((p).val.pair->atoms[0])
#define cdr(p) ((p).val.pair->atoms[1])
#define nilp(atom) ((atom).type == Type_Nil)

struct Atom;
typedef Error (*Builtin)(struct Atom args, struct Atom *result);
typedef struct Atom {
    enum {
        Type_Nil,
        Type_Pair,
        Type_Sym,
        Type_Int,
        Type_Builtin,
        Type_Closure
    } type;

    union {
        struct Pair *pair;
        const char *sym;
        int64_t integer;
        Builtin builtin;
    } val;
} Atom;

typedef struct Pair {
    Atom atoms[2];
} Pair;

static Atom sym_table;
const Atom nil;

Atom cons(Atom car_val, Atom cdr_val);
Atom make_int(int64_t x);
Atom make_sym(const char *s);
Atom make_builtin(Builtin fun);
Error make_closure(Atom env, Atom args, Atom body, Atom *result);
Atom copy_list(Atom list);
bool listp(Atom expr);
Error apply(Atom fun, Atom args, Atom *result);
void repr(Atom atom);
Atom env_create(Atom parent);
Error eval_expr(Atom expr, Atom env, Atom *result);
#endif
