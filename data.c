#include "data.h"

static Atom sym_table = {Type_Nil};
const Atom nil = {Type_Nil};

Atom cons(Atom car_val, Atom cdr_val)
{
    Atom p;

    p.type = Type_Pair;
    p.val.pair = malloc(sizeof(Pair));

    car(p) = car_val;
    cdr(p) = cdr_val;

    return p;
}

Atom make_int(int64_t x)
{
    Atom a;
    a.type = Type_Int;
    a.val.integer = x;

    return a;
}

Atom make_sym(const char *s)
{
    Atom a, p;

    p = sym_table;
    while (!nilp(p)) {
        a = car(p);
        if (strcmp(a.val.sym, s) == 0) {
            return a;
        }
        p = cdr(p);
    }
    
    a.type = Type_Sym;
    a.val.sym = strdup(s);
    sym_table = cons(a, sym_table);

    return a;
}

Atom make_builtin(Builtin fun)
{
    Atom a;
    a.type = Type_Builtin;
    a.val.builtin = fun;
    return a;
}

Error make_closure(Atom env, Atom args, Atom body, Atom *result)
{
    Atom p;

    if (!listp(args) || !listp(body)) {
        return ER_Syntax;
    }

    p = args;
    // every arg is a symbol
    while (!nilp(p)) {
        if (car(p).type != Type_Sym) {
            return ER_Type;
        }
        p = cdr(p);
    }

    *result = cons(env, cons(args, body));
    result->type = Type_Closure;

    return ER_OK;
}

Atom copy_list(Atom list)
{
    Atom a, p;

    if (nilp(list)) {
        return nil;
    }

    a = cons(car(list), nil);
    p = a;
    list = cdr(list);

    while (!nilp(list)) {
        cdr(p) = cons(car(list), nil);
        p = cdr(p);
        list = cdr(list);
    }

    return a;
}

bool listp(Atom expr)
{
    while (!nilp(expr)) {
        if (expr.type != Type_Pair) {
            return false;
        }
        expr = cdr(expr);
    }

    return true;
}


Error apply(Atom fun, Atom args, Atom *result)
{
    if (fun.type == Type_Builtin) {
        return (*fun.val.builtin)(args, result);
    }
    else if (fun.type != Type_Closure) {
        return ER_Type;
    }

    Atom env = env_create(car(fun));
    Atom arg_names = car(cdr(fun));
    Atom body = cdr(cdr(fun));

    while (!nilp(arg_names)) {
        if (nilp(args)) {
            return ER_Args;
        }

        env_set(env, car(arg_names), car(args));
        arg_names = cdr(arg_names);
        args = cdr(args);
    }

    if (!nilp(args)) {
        return ER_Args;
    }

    while (!nilp(body)) {
        Error err = eval_expr(car(body), env, result);
        if (err) {
            return err;
        }
        body = cdr(body);
    }

    return ER_OK;
}

void repr(Atom atom)
{
    switch (atom.type) {
    case Type_Nil:
        printf("NIL");
        break;
    case Type_Pair:
        putchar('(');
        repr(car(atom));
        atom = cdr(atom);
        while (!nilp(atom)) {
            if (atom.type == Type_Pair) {
                putchar(' ');
                repr(car(atom));
                atom = cdr(atom);
            }
            else {
                printf(" . ");
                repr(atom);
                break;
            }
        }
        putchar(')');
        break;
    case Type_Sym:
        printf("%s", atom.val.sym);
        break;
    case Type_Int:
        printf("%lld", atom.val.integer);
        break;
    case Type_Builtin:
        printf("#<BUILTIN:%p>", atom.val.builtin);
        break;
    }
}
