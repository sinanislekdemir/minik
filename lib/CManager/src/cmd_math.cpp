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
	if (validate_command(c, 3) != 0) {
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
	if (validate_command(c, 3) != 0) {
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
	if (validate_command(c, 3) != 0) {
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
	double v1d = ctod(v1->data);
	double v2d = ctod(v2->data);
#ifndef DISABLE_EXCEPTIONS
	if (v2d == 0) {
		c->exception = raise((char *)ERR_STR_DIVISION_BY_ZERO, c->pid, ERR_INVALID_PARAMETER_TYPE);
		return -1;
	}
#endif

	double div = v1d / v2d;
	return save_result(c, div);
}

int command_mul(command *c, program *_p) {
#ifndef DISABLE_EXCEPTIONS
	if (validate_command(c, 3) != 0) {
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
	if (validate_command(c, 3) != 0) {
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
	if (validate_command(c, 3) != 0) {
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
	if (validate_command(c, 3) != 0) {
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
	if (validate_command(c, 3) != 0) {
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

int command_trigonometry(command *c, program *_p) {
#ifndef DISABLE_EXCEPTIONS
	if (validate_command(c, 2) != 0) {
		return -1;
	}
#endif

	variable *val = get_var(c, 1);

#ifndef DISABLE_EXCEPTIONS
	if (val == NULL) {
		char *msg = (char *)malloc(64);
		memset(msg, 0, 64);
		sprintf(msg, "Invalid Parameter(s)");
		c->exception = raise(msg, c->pid, ERR_VARIABLE_NOT_FOUND);
		free(msg);
		return -1;
	}
#endif

	double res = 0;
	double vald = ctod(val->data);
	if (strcmp(c->cmd, "SIN") == 0) {
		res = sin(vald);
	}
	if (strcmp(c->cmd, "COS") == 0) {
		res = cos(vald);
	}
	if (strcmp(c->cmd, "TAN") == 0) {
		res = tan(vald);
	}
	if (strcmp(c->cmd, "COT") == 0) {
		res = 1.0 / tan(vald);
	}
	if (strcmp(c->cmd, "SINH") == 0) {
		res = sinh(vald);
	}
	if (strcmp(c->cmd, "COSH") == 0) {
		res = cosh(vald);
	}
	if (strcmp(c->cmd, "TANH") == 0) {
		res = tanh(vald);
	}
	if (strcmp(c->cmd, "COTH") == 0) {
		res = 1.0 / tanh(vald);
	}
	if (strcmp(c->cmd, "ASIN") == 0) {
		res = asin(vald);
	}
	if (strcmp(c->cmd, "ACOS") == 0) {
		res = acos(vald);
	}
	if (strcmp(c->cmd, "ATAN") == 0) {
		res = atan(vald);
	}
	if (strcmp(c->cmd, "ACOT") == 0) {
		res = 1.0 / atan(vald);
	}
#ifdef BOARD_ESP32
	if (strcmp(c->cmd, "ASINH") == 0) {
		res = asinh(vald);
	}
	if (strcmp(c->cmd, "ACOSH") == 0) {
		res = acosh(vald);
	}
	if (strcmp(c->cmd, "ATANH") == 0) {
		res = atanh(vald);
	}
	if (strcmp(c->cmd, "ACOTH") == 0) {
		res = 1.0 / atanh(vald);
	}
#endif
	new_number(c->args[0].data, res, c->pid);
	return 0;
}
