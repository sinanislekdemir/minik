#include "cmd_logic.hpp"
#include "helpers.hpp"

extern sub _subs[MAX_SUBS];

int _quick_jump(command c, program *p) {
	if (c.variable_type[0] == TYPE_NUM) {
		int sub = int(c.variable_constant[0]);
		int cmd = 0;
		if (c.arg_count == 2) {
			cmd = int(c.variable_constant[1]);
		}
		p->append_to_history(p->cursor, _subs[p->subs[p->cursor]].cursor);
		p->cursor = sub;
		_subs[p->subs[p->cursor]].cursor = cmd;
		return 1;
	}
	if (c.variable_type[0] != TYPE_LABEL) {
		error_msg(ERR_STR_ADDRESS_NOT_FOUND, p->pid);
		return -1;
	}
	p->append_to_history(p->cursor, _subs[p->subs[p->cursor]].cursor);
	p->cursor = c.variable_index[0];
	_subs[p->subs[p->cursor]].cursor = 0;
	return 1;
}

/**
 * @brief Compare two variables, save result into cmp flag
 *
 * @param c
 * @param p
 * @return int
 */
int command_cmp(command c, program *p) {
	p->set_cmp_flag(CMP_N);
#ifndef DISABLE_EXCEPTIONS
	if (c.arg_count != 2) {
		error_msg(ERR_STR_NOT_ENOUGH_PARAMS, c.pid);
		return -1;
	}
#endif

	if ((c.variable_type[0] == TYPE_ADDRESS && c.variable_type[1] == TYPE_NUM) ||
	    (c.variable_type[1] == TYPE_ADDRESS && c.variable_type[0] == TYPE_NUM)) {
		double d1 = get_double(c, 0);
		double d2 = get_double(c, 1);
		if (d1 == d2) {
			p->set_cmp_flag(CMP_JE);
		}
		if (d1 > d2) {
			p->set_cmp_flag(CMP_JG);
		}
		if (d1 < d2) {
			p->set_cmp_flag(CMP_JL);
		}
		return 0;
	}

	if ((c.variable_type[0] == TYPE_ADDRESS && c.variable_type[1] == TYPE_STR) ||
	    (c.variable_type[1] == TYPE_ADDRESS && c.variable_type[0] == TYPE_STR)) {
		char buffer1[MAX_LINE_LENGTH] = {0};
		char buffer2[MAX_LINE_LENGTH] = {0};
		get_string(c, 0, buffer1, 0);
		get_string(c, 1, buffer2, 0);
		if (strcmp(buffer1, buffer2) == 0) {
			p->set_cmp_flag(CMP_JE);
		} else {
			p->set_cmp_flag(CMP_N);
		}
		return 0;
	}

	if (c.variable_type[0] != c.variable_type[1]) {
		p->set_cmp_flag(CMP_N);
		return 0;
	}

	if (c.variable_index[0] == -1 || c.variable_index[1] == -1) {
		p->set_cmp_flag(CMP_N);
		return 0;
	}

	if (c.variable_type[0] == TYPE_ADDRESS) {
		char type = area_type(c.variable_index[0]);
		if (type == TYPE_NUM) {
			double d1 = get_double(c, 0);
			double d2 = get_double(c, 1);
			if (d1 == d2) {
				p->set_cmp_flag(CMP_JE);
			}
			if (d1 > d2) {
				p->set_cmp_flag(CMP_JG);
			}
			if (d1 < d2) {
				p->set_cmp_flag(CMP_JL);
			}
		}
		if (type == TYPE_BYTE) {
			char c1 = get_byte(c, 0);
			char c2 = get_byte(c, 1);
			if (c1 == c2) {
				p->set_cmp_flag(CMP_JE);
			}
			if (c1 > c2) {
				p->set_cmp_flag(CMP_JG);
			}
			if (c1 < c2) {
				p->set_cmp_flag(CMP_JL);
			}
		}
		if (type == TYPE_STR) {
			char buffer1[MAX_LINE_LENGTH] = {0};
			char buffer2[MAX_LINE_LENGTH] = {0};
			get_string(c, 0, buffer1, 0);
			get_string(c, 1, buffer2, 0);
			if (strcmp(buffer1, buffer2) == 0) {
				p->set_cmp_flag(CMP_JE);
			} else {
				p->set_cmp_flag(CMP_N);
			}
		}

		return 0;
	}

	if (c.variable_type[0] == TYPE_NUM) {
		double d1 = get_double(c, 0);
		double d2 = get_double(c, 1);

		if (d1 == d2) {
			p->set_cmp_flag(CMP_JE);
		}
		if (d1 > d2) {
			p->set_cmp_flag(CMP_JG);
		}
		if (d1 < d2) {
			p->set_cmp_flag(CMP_JL);
		}
		return 0;
	}

	if (c.variable_type[0] == TYPE_STR) {
		char buffer1[MAX_LINE_LENGTH] = {0};
		char buffer2[MAX_LINE_LENGTH] = {0};
		get_string(c, 0, buffer1, 0);
		get_string(c, 1, buffer2, 0);
		if (strcmp(buffer1, buffer2) == 0) {
			p->set_cmp_flag(CMP_JE);
		} else {
			p->set_cmp_flag(CMP_N);
		}
		return 0;
	}

	if (c.variable_type[0] == TYPE_BYTE) {
		char c1 = get_byte(c, 0);
		char c2 = get_byte(c, 1);
		if (c1 == c2) {
			p->set_cmp_flag(CMP_JE);
		}
		if (c1 > c2) {
			p->set_cmp_flag(CMP_JG);
		}
		if (c1 < c2) {
			p->set_cmp_flag(CMP_JL);
		}

		return 0;
	}

	return 0;
}

int command_je(command c, program *p) {
	if (p->_cmp_flag != CMP_JE) {
		return 0;
	}
	return _quick_jump(c, p);
}

int command_jne(command c, program *p) {
	if (p->_cmp_flag == CMP_JE) {
		return 0;
	}
	return _quick_jump(c, p);
}

int command_jg(command c, program *p) {
	if (p->_cmp_flag != CMP_JG) {
		return 0;
	}
	return _quick_jump(c, p);
}

int command_jge(command c, program *p) {
	if (p->_cmp_flag == CMP_JE || p->_cmp_flag == CMP_JG) {
		return _quick_jump(c, p);
	}
	return 0;
}

int command_jl(command c, program *p) {
	if (p->_cmp_flag != CMP_JL) {
		return 0;
	}
	return _quick_jump(c, p);
}

int command_jle(command c, program *p) {
	if (p->_cmp_flag == CMP_JE || p->_cmp_flag == CMP_JL) {
		return _quick_jump(c, p);
	}
	return 0;
}

int _command_validations(command c) {
#ifndef DISABLE_EXCEPTIONS
	if (c.arg_count != 2) {
		return -1;
	}
	if (c.variable_type[0] != TYPE_ADDRESS) {
		error_msg(ERR_STR_VAR_NOT_FOUND, c.pid);
		return -1;
	}

	if (c.variable_type[1] == TYPE_STR || c.variable_type[1] == TYPE_LABEL) {
		error_msg(ERR_STR_INVALID_TYPE, c.pid);
		return -1;
	}
#endif
	return 0;
}

int command_lrotate(command c, program *p) {
	int check = _command_validations(c);
	if (check == -1) {
		return check;
	}

	char byte = get_byte(c, 0);
	int bits = get_int(c, 1);
	byte = (byte << bits) | (byte >> (BITS - bits));
	write_area(c.variable_index[0], byte);
	return 0;
}

int command_rrotate(command c, program *p) {
	int check = _command_validations(c);
	if (check == -1) {
		return check;
	}
	char byte = get_byte(c, 0);
	int bits = get_int(c, 1);
	byte = (byte >> bits) | (byte << (BITS - bits));
	write_area(c.variable_index[0], byte);
	return 0;
}

int command_lshift(command c, program *p) {
	int check = _command_validations(c);
	if (check == -1) {
		return check;
	}
	char byte = get_byte(c, 0);
	int bits = get_int(c, 1);
	byte = byte << bits;
	write_area(c.variable_index[0], byte);
	return 0;
}

int command_rshift(command c, program *p) {
	int check = _command_validations(c);
	if (check == -1) {
		return check;
	}
	char byte = get_byte(c, 0);
	int bits = get_int(c, 1);
	byte = byte >> bits;
	write_area(c.variable_index[0], byte);
	return 0;
}
