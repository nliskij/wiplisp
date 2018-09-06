#ifndef BUILTIN_H
#define BUILTIN_H
#include "data.h"

Error builtin_car(Atom args, Atom *result);
Error builtin_cdr(Atom args, Atom *result);
Error builtin_cons(Atom args, Atom *result);
Error builtin_add(Atom args, Atom *result);
Error builtin_subtract(Atom args, Atom *result);
Error builtin_mul(Atom args, Atom *result);
Error builtin_div(Atom args, Atom *result);
Error builtin_mod(Atom args, Atom *result);
Error builtin_numeq(Atom args, Atom *result);
Error builtin_leq(Atom args, Atom *result);
Error builtin_listp(Atom args, Atom *result);
#endif
