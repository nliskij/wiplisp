#include "builtin.h"

Error builtin_car(Atom args, Atom *result)
{
    if (nilp(args) || !nilp(cdr(args))) {
        return ER_Args;
    }

    if (nilp(car(args))) {
        *result = nil;
    }
    else if (car(args).type != Type_Pair) {
        return ER_Type;
    }
    else {
        *result = car(car(args));
    }

    return ER_OK;
}

Error builtin_cdr(Atom args, Atom *result)
{
    if (nilp(args) || !nilp(cdr(args))) {
        return ER_Args;
    }

    if (nilp(car(args))) {
        *result = nil;
    }
    else if (car(args).type != Type_Pair) {
        return ER_Type;
    }
    else {
        *result = cdr(car(args));
    }

    return ER_OK;
}

Error builtin_cons(Atom args, Atom *result)
{
    if (nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args)))) {
        return ER_Args;
    }

    *result = cons(car(args), car(cdr(args)));

    return ER_OK;
}

Error builtin_add(Atom args, Atom *result)
{
    Atom a, b;
    if (nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args)))) {
        return ER_Args;
    }

    a = car(args);
    b = car(cdr(args));

    if (a.type != Type_Int || b.type != Type_Int) {
        return ER_Type;
    }

    *result = make_int(a.val.integer + b.val.integer);

    return ER_OK;
}

Error builtin_subtract(Atom args, Atom *result)
{
    Atom a, b;
    if (nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args)))) {
        return ER_Args;
    }

    a = car(args);
    b = car(cdr(args));

    if (a.type != Type_Int || b.type != Type_Int) {
        return ER_Type;
    }

    *result = make_int(a.val.integer - b.val.integer);

    return ER_OK;
}

Error builtin_mul(Atom args, Atom *result)
{
    Atom a, b;
    if (nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args)))) {
        return ER_Args;
    }

    a = car(args);
    b = car(cdr(args));

    if (a.type != Type_Int || b.type != Type_Int) {
        return ER_Type;
    }

    *result = make_int(a.val.integer * b.val.integer);

    return ER_OK;
}

Error builtin_div(Atom args, Atom *result)
{
    Atom a, b;
    if (nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args)))) {
        return ER_Args;
    }

    a = car(args);
    b = car(cdr(args));

    if (a.type != Type_Int || b.type != Type_Int) {
        return ER_Type;
    }

    *result = make_int(a.val.integer / b.val.integer);

    return ER_OK;
}

Error builtin_mod(Atom args, Atom *result)
{
    Atom a, b;
    if (nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args)))) {
        return ER_Args;
    }

    a = car(args);
    b = car(cdr(args));

    if (a.type != Type_Int || b.type != Type_Int) {
        return ER_Type;
    }

    *result = make_int(a.val.integer % b.val.integer);

    return ER_OK;
}

Error builtin_numeq(Atom args, Atom *result)
{
    Atom a, b;

    if (nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args)))) {
        return ER_Args;
    }

    a = car(args);
    b = car(cdr(args));

    if (a.type != Type_Int || b.type != Type_Int) {
        return ER_Type;
    }

    *result = (a.val.integer == b.val.integer) ? make_sym("t") : nil;

    return ER_OK;
}

Error builtin_leq(Atom args, Atom *result)
{
    Atom a, b;

    if (nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args)))) {
        return ER_Args;
    }

    a = car(args);
    b = car(cdr(args));

    if (a.type != Type_Int || b.type != Type_Int) {
        return ER_Type;
    }

    *result = (a.val.integer <= b.val.integer) ? make_sym("t") : nil;

    return ER_OK;
}

Error builtin_listp(Atom args, Atom *result)
{
    if (nilp(args) || !nilp(cdr(args))) {
        return ER_Args;
    }

    *result = listp(car(args)) ? make_sym("t") : nil;

    return ER_OK;
}
