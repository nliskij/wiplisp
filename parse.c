#include "parse.h"

Error lex(const char *str, const char **start, const char **end)
{
    const char *ws = " \t\n";
    const char *delim = "() \t\n";

    str += strspn(str, ws);
    if (*str == '\0') {
        *start = *end = NULL;
        return ER_Syntax;
    }

    *start = str;

    if (*str == '(' || *str == ')') {
        *end = str + 1;
    }
    else {
        *end = str + strcspn(str, delim);
    }

    return ER_OK;
}

Error parse_simple(const char *start, const char *end, Atom *result)
{
    // Parses int
    char *p;
    uint64_t val = strtoll(start, &p, 10);
    if (p == end) {
        result->type = Type_Int;
        result->val.integer = val;
        return ER_OK;
    }

    // Parses symbol or nil
    char *buf = calloc(end - start + 1, sizeof(char));
    p = buf;
    while (start != end) {
        *p++ = *start;
        start++;
    }

    if (strcmp(buf, "NIL") == 0) {
        *result = nil;
    }
    else {
        *result = make_sym(buf);
    }

    free(buf);

    return ER_OK;
}

Error read_list(const char *start, const char **end, Atom *result)
{
    Atom p;

    *end = start;
    p = *result = nil; // empty list () equivalent to nil
    
    while (1) {
        const char *token;
        Atom item;
        Error err;

        err = lex(*end, &token, end);
        if (err) {
            return err;
        }

        if (*token == ')') {
            return ER_OK;
        }

        if (*token == '.' && *end - token == 1) {
            // this is an improper list
            if (nilp(p)) {
                // . never begins a list
                return ER_Syntax;
            }
            err = read_expr(*end, end, &item);
            if (err) {
                return err;
            }
            cdr(p) = item;

            err = lex(*end, &token, end);
            if (!err && *token != ')') {
                err = ER_Syntax;
            }

            return err;
        }

        err = read_expr(token, end, &item);
        if (err) {
            return err;
        }

        if (nilp(p)) {
            // this is the first item
            *result = cons(item, nil);
            p = *result;
        }
        else {
            cdr(p) = cons(item, nil);
            p = cdr(p);
        }
    }
}

Error read_expr(const char *input, const char **end, Atom *result)
{
    const char *token;
    Error err;

    err = lex(input, &token, end);
    if (err) {
        return err;
    }

    if (*token == '(') {
        return read_list(*end, end, result);
    }
    else if (*token == ')') {
        return ER_Syntax;
    }
    else if (*token == '\'') {
        *result = cons(make_sym("quote"), cons(nil, nil));
        return read_expr(*end, end, &car(cdr(*result)));
    }
    else {
        return parse_simple(token, *end, result);
    }
}
