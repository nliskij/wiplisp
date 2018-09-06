#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "data.h"
#include "parse.h"
#include "expr.h"
#include "builtin.h"

int main(int argc, char **argv)
{
	Atom env = env_create(nil);
	char *input;

	env_set(env, make_sym("car"), make_builtin(builtin_car));
	env_set(env, make_sym("cdr"), make_builtin(builtin_cdr));
	env_set(env, make_sym("cons"), make_builtin(builtin_cons));
	env_set(env, make_sym("+"), make_builtin(builtin_add));
	env_set(env, make_sym("-"), make_builtin(builtin_subtract));
	env_set(env, make_sym("*"), make_builtin(builtin_mul));
	env_set(env, make_sym("/"), make_builtin(builtin_div));
	env_set(env, make_sym("%"), make_builtin(builtin_mod));
	env_set(env, make_sym("="), make_builtin(builtin_numeq));
	env_set(env, make_sym("<="), make_builtin(builtin_leq));
	env_set(env, make_sym("nil"), nil);
	env_set(env, make_sym("t"), make_sym("t"));
	env_set(env, make_sym("list?"), make_builtin(builtin_listp));

	while ((input = readline("> ")) != NULL) {
		const char *p = input;
		Error err;
		Atom expr, result = {0};

		err = read_expr(p, &p, &expr);

		if (!err) {
			err = eval_expr(expr, env, &result);
		}

		switch (err) {
		case ER_OK:
			repr(result);
			putchar('\n');
			break;
		case ER_Syntax:
			printf("Error: Syntax error\n");
			break;
		case ER_Unbound:
			printf("Error: Symbol not bound\n");
			break;
		case ER_Args:
			printf("Error: Wrong number of arguments\n");
			break;
		case ER_Type:
			printf("Error: Wrong type\n");
			break;
		}

		free(input);
	}
	
	return 0;
}
