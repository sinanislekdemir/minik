#include "cmd_math.hpp"
#include "helpers.hpp"
#include <math.h>

extern error err;

int save_result(command *c, double result) {
	new_number(c->args[0].data, result, c->pid);
	return 0;
}

int command_add(command *c, program *_p) {
#ifndef DISABLE_EXCEPTIONS
	if (validate_command(c, (const char *)"add", 3) != 0) {
		return -1;
	}
#endif
	variable *v1 = get_var(c, 1);
	variable *v2 = get_var(c, 2);

#ifndef DISABLE_EXCEPTIONS
	if (v1 == NULL || v2 == NULL) {
		char *msg = (char *)malloc(64);
		memset(msg, 0, 64);
		sprintf(msg, "Invalid Parameter(s)");
		c->exception = raise(msg, c->pid, ERR_VARIABLE_NOT_FOUND);
		free(msg);
		return -1;
	}
#endif

	double add = ctod(v1->data) + ctod(v2->data);
	return save_result(c, add);
}

int command_sub(command *c, program *_p) {
	if (validate_command(c, (const char *)"sub", 3) != 0) {
		return -1;
	}

	variable *v1 = get_var(c, 1);
	variable *v2 = get_var(c, 2);

#ifndef DISABLE_EXCEPTIONS
	if (v1 == NULL || v2 == NULL) {
		char *msg = (char *)malloc(64);
		memset(msg, 0, 64);
		sprintf(msg, "Invalid Parameter(s)");
		c->exception = raise(msg, c->pid, ERR_VARIABLE_NOT_FOUND);
		free(msg);
		return -1;
	}
#endif

	double sub = ctod(v1->data) - ctod(v2->data);
	return save_result(c, sub);
}

int command_div(command *c, program *_p) {
#ifndef DISABLE_EXCEPTIONS
	if (validate_command(c, (const char *)"div", 3) != 0) {
		return -1;
	}
#endif

	variable *v1 = get_var(c, 1);
	variable *v2 = get_var(c, 2);

#ifndef DISABLE_EXCEPTIONS
	if (v1 == NULL || v2 == NULL) {
		char *msg = (char *)malloc(64);
		memset(msg, 0, 64);
		sprintf(msg, "Invalid Parameter(s)");
		c->exception = raise(msg, c->pid, ERR_VARIABLE_NOT_FOUND);
		free(msg);
		return -1;
	}
#endif

	double div = ctod(v1->data) / ctod(v2->data);
	return save_result(c, div);
}

int command_mul(command *c, program *_p) {
#ifndef DISABLE_EXCEPTIONS
	if (validate_command(c, (const char *)"mul", 3) != 0) {
		return -1;
	}
#endif

	variable *v1 = get_var(c, 1);
	variable *v2 = get_var(c, 2);

#ifndef DISABLE_EXCEPTIONS
	if (v1 == NULL || v2 == NULL) {
		char *msg = (char *)malloc(64);
		memset(msg, 0, 64);
		sprintf(msg, "Invalid Parameter(s)");
		c->exception = raise(msg, c->pid, ERR_VARIABLE_NOT_FOUND);
		free(msg);
		return -1;
	}
#endif

	double mul = ctod(v1->data) * ctod(v2->data);
	return save_result(c, mul);
}

int command_xor(command *c, program *_p) {
#ifndef DISABLE_EXCEPTIONS
	if (validate_command(c, (const char *)"xor", 3) != 0) {
		return -1;
	}
#endif

	variable *v1 = get_var(c, 1);
	variable *v2 = get_var(c, 2);

#ifndef DISABLE_EXCEPTIONS
	if (v1 == NULL || v2 == NULL) {
		char *msg = (char *)malloc(64);
		memset(msg, 0, 64);
		sprintf(msg, "Invalid Parameter(s)");
		c->exception = raise(msg, c->pid, ERR_VARIABLE_NOT_FOUND);
		free(msg);
		return -1;
	}
#endif

	double xord = int(ctod(v1->data)) % int(ctod(v2->data));
	return save_result(c, xord);
}

int command_or(command *c, program *_p) {
#ifndef DISABLE_EXCEPTIONS
	if (validate_command(c, (const char *)"or", 3) != 0) {
		return -1;
	}
#endif

	variable *v1 = get_var(c, 1);
	variable *v2 = get_var(c, 2);

#ifndef DISABLE_EXCEPTIONS
	if (v1 == NULL || v2 == NULL) {
		char *msg = (char *)malloc(64);
		memset(msg, 0, 64);
		sprintf(msg, "Invalid Parameter(s)");
		c->exception = raise(msg, c->pid, ERR_VARIABLE_NOT_FOUND);
		free(msg);
		return -1;
	}
#endif

	double or_r = int(ctod(v1->data)) | int(ctod(v2->data));
	return save_result(c, or_r);
}

int command_and(command *c, program *_p) {
#ifndef DISABLE_EXCEPTIONS
	if (validate_command(c, (const char *)"and", 3) != 0) {
		return -1;
	}
#endif

	variable *v1 = get_var(c, 1);
	variable *v2 = get_var(c, 2);

#ifndef DISABLE_EXCEPTIONS
	if (v1 == NULL || v2 == NULL) {
		char *msg = (char *)malloc(64);
		memset(msg, 0, 64);
		sprintf(msg, "Invalid Parameter(s)");
		c->exception = raise(msg, c->pid, ERR_VARIABLE_NOT_FOUND);
		free(msg);
		return -1;
	}
#endif

	double and_r = int(ctod(v1->data)) & int(ctod(v2->data));
	return save_result(c, and_r);
}

int command_pow(command *c, program *_p) {
#ifndef DISABLE_EXCEPTIONS
	if (validate_command(c, (const char *)"pow", 3) != 0) {
		return -1;
	}
#endif

	variable *v1 = get_var(c, 1);
	variable *v2 = get_var(c, 2);

#ifndef DISABLE_EXCEPTIONS
	if (v1 == NULL || v2 == NULL) {
		char *msg = (char *)malloc(64);
		memset(msg, 0, 64);
		sprintf(msg, "Invalid Parameter(s)");
		c->exception = raise(msg, c->pid, ERR_VARIABLE_NOT_FOUND);
		free(msg);
		return -1;
	}
#endif

	double pow_r = pow(ctod(v1->data), ctod(v2->data));
	return save_result(c, pow_r);
}
