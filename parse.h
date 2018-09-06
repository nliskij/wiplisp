#ifndef PARSE_H
#define PARSE_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data.h"
#include "error.h"


Error lex(const char *str, const char **start, const char **end);
Error parse_simple(const char *start, const char *end, Atom *result);
Error parse_list(const char *start, const char **end, Atom *result);
Error read_expr(const char *input, const char **end, Atom *result);
#endif
