#include "cmd_math.hpp"
#include "constants.hpp"
#include "helpers.hpp"
#include <math.h>

int _validate_command(command c) {
	if (c.arg_count != 3) {
		error_msg(ERR_STR_NOT_ENOUGH_PARAMS, c.pid);
		return -1;
	}
	if (c.variable_type[0] != TYPE_ADDRESS) {
		error_msg(ERR_STR_INVALID_TYPE, c.pid);
		return -1;
	}
	if (c.variable_type[1] == TYPE_LABEL || c.variable_type[1] == TYPE_STR) {
		error_msg(ERR_STR_INVALID_TYPE, c.pid);
		return -1;
	}
	if (c.variable_type[2] == TYPE_LABEL || c.variable_type[2] == TYPE_STR) {
		error_msg(ERR_STR_INVALID_TYPE, c.pid);
		return -1;
	}
	return 0;
}

int command_add(command c, program *p) {
#ifndef DISABLE_EXCEPTIONS
	if (_validate_command(c) != 0) {
		return -1;
	}
#endif

	double v1 = get_double(c, 1);
	double v2 = get_double(c, 2);

	double add = v1 + v2;
	return write_area(c.variable_index[0], add);
}

int command_sub(command c, program *p) {
#ifndef DISABLE_EXCEPTIONS
	if (_validate_command(c) != 0) {
		return -1;
	}
#endif

	double v1 = get_double(c, 1);
	double v2 = get_double(c, 2);

	double sub = v1 - v2;
	return write_area(c.variable_index[0], sub);
}

int command_div(command c, program *p) {
#ifndef DISABLE_EXCEPTIONS
	if (_validate_command(c) != 0) {
		return -1;
	}
#endif

	double v1 = get_double(c, 1);
	double v2 = get_double(c, 2);

#ifndef DISABLE_EXCEPTIONS
	if (v2 == 0) {
		error_msg(ERR_STR_DIVISION_BY_ZERO, c.pid);
		return -1;
	}
#endif

	double div = v1 / v2;
	return write_area(c.variable_index[0], div);
}

int command_mul(command c, program *p) {
#ifndef DISABLE_EXCEPTIONS
	if (_validate_command(c) != 0) {
		return -1;
	}
#endif

	double v1 = get_double(c, 1);
	double v2 = get_double(c, 2);

	double mul = v1 * v2;
	return write_area(c.variable_index[0], mul);
}

int command_xor(command c, program *p) {
#ifndef DISABLE_EXCEPTIONS
	if (_validate_command(c) != 0) {
		return -1;
	}
#endif

	double v1 = get_double(c, 1);
	double v2 = get_double(c, 2);

	double xord = int(v1) % int(v2);
	return write_area(c.variable_index[0], xord);
}

int command_or(command c, program *p) {
#ifndef DISABLE_EXCEPTIONS
	if (_validate_command(c) != 0) {
		return -1;
	}
#endif

	double v1 = get_double(c, 1);
	double v2 = get_double(c, 2);

	double or_r = int(v1) | int(v2);
	return write_area(c.variable_index[0], or_r);
}

int command_and(command c, program *p) {
#ifndef DISABLE_EXCEPTIONS
	if (_validate_command(c) != 0) {
		return -1;
	}
#endif

	double v1 = get_double(c, 1);
	double v2 = get_double(c, 2);

	double and_r = int(v1) & int(v2);
	return write_area(c.variable_index[0], and_r);
}

int command_pow(command c, program *p) {
#ifndef DISABLE_EXCEPTIONS
	if (_validate_command(c) != 0) {
		return -1;
	}
#endif

	double v1 = get_double(c, 1);
	double v2 = get_double(c, 2);

	double pow_r = pow(v1, v2);
	return write_area(c.variable_index[0], pow_r);
}

int command_trigonometry(command c, program *p) {
	if (c.variable_type[0] != TYPE_ADDRESS) {
		error_msg(ERR_STR_INVALID_TYPE, c.pid);
		return -1;
	}

	double val = get_double(c, 1);
	double res = 0;

	switch (c.statement) {
	case STATEMENT_SIN:
		res = sin(val);
		break;
	case STATEMENT_COS:
		res = cos(val);
		break;
	case STATEMENT_TAN:
		res = tan(val);
		break;
	case STATEMENT_COT:
		res = 1.0 / tan(val);
		break;
	case STATEMENT_SINH:
		res = sinh(val);
		break;
	case STATEMENT_COSH:
		res = cosh(val);
		break;
	case STATEMENT_TANH:
		res = tanh(val);
		break;
	case STATEMENT_COTH:
		res = 1.0 / tanh(val);
		break;
	case STATEMENT_ASIN:
		res = asin(val);
		break;
	case STATEMENT_ACOS:
		res = acos(val);
		break;
	case STATEMENT_ATAN:
		res = atan(val);
		break;
	case STATEMENT_ACOT:
		res = 1.0 / atan(val);
		break;
	default:
		res = 0;
		break;
	}

	write_area(c.variable_index[0], res);
	return 0;
}
