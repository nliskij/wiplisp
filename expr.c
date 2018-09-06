#include "expr.h"

Atom env_create(Atom parent)
{
    return cons(parent, nil);
}

Error env_get(Atom env, Atom sym, Atom *result)
{
    Atom parent = car(env);
    Atom binds = cdr(env);

    while(!nilp(binds)) {
        Atom bind = car(binds);
        // test equality of pointer to string since duplicates all have the same adr
        if (car(bind).val.sym == sym.val.sym) {
            *result = cdr(bind);
            return ER_OK;
        }
        binds = cdr(binds);
    }

    if (nilp(parent)) {
        return ER_Unbound;
    }

    return env_get(parent, sym, result);
}

Error env_set(Atom env, Atom sym, Atom val)
{
    Atom binds = cdr(env);
    Atom bind = nil;

    while(!nilp(binds)) {
        bind = car(binds);
        if (car(bind).val.sym == sym.val.sym) {
            cdr(bind) = val;
            return ER_OK;
        }
        binds = cdr(binds);
    }

    bind = cons(sym, val);
    cdr(env) = cons(bind, cdr(env));

    return ER_OK;
}

Error eval_expr(Atom expr, Atom env, Atom *result)
{
    Atom op, args;
    Error err;

    if (expr.type == Type_Sym) {
        return env_get(env, expr, result);
    }
    else if (expr.type != Type_Pair) {
        *result = expr;
        return ER_OK;
    }

    if (!listp(expr)) {
        return ER_Syntax;
    }

    op = car(expr);
    args = cdr(expr);

    if (op.type == Type_Sym) {
        if (strcmp(op.val.sym, "quote") == 0) {
            if (nilp(args) || !nilp(cdr(args))) {
                return ER_Args;
            }

            *result = car(args);
            return ER_OK;
        }
        else if (strcmp(op.val.sym, "define") == 0) {
            Atom sym, val;

            if (nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args)))) {
                return ER_Args;
            }

            sym = car(args);
            if (sym.type != Type_Sym) {
                return ER_Type;
            }

            err = eval_expr(car(cdr(args)), env, &val);
            if (err) {
                return err;
            }

            *result = sym;
            return env_set(env, sym, val);
        }
        else if (strcmp(op.val.sym, "lambda") == 0) {
            if (nilp(args) || nilp(cdr(args))) {
                return ER_Args;
            }

            return make_closure(env, car(args), cdr(args), result);
        }
        else if (strcmp(op.val.sym, "if") == 0) {
            Atom cond, val;
            if (nilp(args) || nilp(cdr(args)) || nilp(cdr(cdr(args)))
                || !nilp(cdr(cdr(cdr(args))))) {
                return ER_Args;
            }

            err = eval_expr(car(args), env, &cond);
            if (err) {
                return err;
            }

            val = nilp(cond) ? car(cdr(cdr(args))) : car(cdr(args));

            return eval_expr(val, env, result);
        }
    }

    err = eval_expr(op, env, &op);
    if (err) {
        return err;
    }

    args = copy_list(args);
    Atom p = args;
    while (!nilp(p)) {
        err = eval_expr(car(p), env, &car(p));
        if (err) {
            return err;
        }

        p = cdr(p);
    }

    return apply(op, args, result);
}
