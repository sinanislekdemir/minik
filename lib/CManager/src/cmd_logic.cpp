#include "cmd_logic.hpp"
#include "helpers.hpp"

int _quick_jump(command *c, program *_p) {
	sub *s = _p->find_sub(c->args[0].name);
#ifndef DISABLE_EXCEPTIONS
	if (s == NULL) {
		char *msg = (char *)malloc(64);
		memset(msg, 0, 64);
		sprintf(msg, "Jump location [%s] not found", c->args[0].name);
		c->exception = raise(msg, c->pid, ERR_ADDRESS_NOT_FOUND);
		free(msg);
		return -1;
	}
#endif
	_p->cursor = s;
	_p->cursor->cursor = _p->cursor->root_instruction;
	return 1;
}

int command_cmp(command *c, program *_p) {
#ifndef DISABLE_EXCEPTIONS
	if (c->argc != 2) {
		char *msg = (char *)malloc(64);
		memset(msg, 0, 64);
		sprintf(msg, ERR_STR_NOT_ENOUGH_PARAMS, c->cmd, 2, c->argc);
		c->exception = raise(msg, c->pid, ERR_NOT_ENOUGH_ARGUMENTS);
		free(msg);
		return -1;
	}
#endif

	variable *v1 = get_var(c, 0);
	variable *v2 = get_var(c, 1);

#ifndef DISABLE_EXCEPTIONS
	if (v1->type != v2->type) {
		char *msg = (char *)malloc(64);
		memset(msg, 0, 64);
		sprintf(msg, "Invalid type comparison [%s] vs [%s]", type_tostr(v1->type), type_tostr(v2->type));
		c->exception = raise(msg, c->pid, ERR_INVALID_PARAMETER_TYPE);
		free(msg);
		return -1;
	}
#endif

	if (memcmp(v1->data, v2->data, v1->datasize) == 0) {
		_p->set_cmp_flag(CMP_JE);
		return 0;
	}

	if (v1->type == TYPE_BYTE) {
		char i1 = v1->data[0];
		char i2 = v2->data[0];
		if (i1 > i2) {
			_p->set_cmp_flag(CMP_JG);
			return 0;
		}
		if (i1 < i2) {
			_p->set_cmp_flag(CMP_JL);
			return 0;
		}
	}
	if (v1->type == TYPE_NUM) {
		double d1 = ctod(v1->data);
		double d2 = ctod(v2->data);
		if (d1 > d2) {
			_p->set_cmp_flag(CMP_JG);
			return 0;
		}
		if (d1 < d2) {
			_p->set_cmp_flag(CMP_JL);
			return 0;
		}
	}
	_p->set_cmp_flag(CMP_N);
	return 0;
}

int command_je(command *c, program *_p) {
	if (_p->_cmp_flag != CMP_JE) {
		return 0;
	}
	return _quick_jump(c, _p);
}

int command_jne(command *c, program *_p) {
	if (_p->_cmp_flag == CMP_JE) {
		return 0;
	}
	return _quick_jump(c, _p);
}

int command_jg(command *c, program *_p) {
	if (_p->_cmp_flag != CMP_JG) {
		return 0;
	}
	return _quick_jump(c, _p);
}

int command_jge(command *c, program *_p) {
	if (_p->_cmp_flag == CMP_JE || _p->_cmp_flag == CMP_JG) {
		return _quick_jump(c, _p);
	}
	return 0;
}

int command_jl(command *c, program *_p) {
	if (_p->_cmp_flag != CMP_JL) {
		return 0;
	}
	return _quick_jump(c, _p);
}

int command_jle(command *c, program *_p) {
	if (_p->_cmp_flag == CMP_JE || _p->_cmp_flag == CMP_JL) {
		return _quick_jump(c, _p);
	}
	return 0;
}

int _command_validations(command *c, variable *v1, variable *v2) {
#ifndef DISABLE_EXCEPTIONS
	if (v1 == NULL) {
		char *msg = (char *)malloc(64);
		memset(msg, 0, 64);
		sprintf(msg, ERR_STR_VAR_NOT_FOUND, c->args[0].data);
		c->exception = raise(msg, c->pid, ERR_VARIABLE_NOT_FOUND);
		free(msg);
		return -1;
	}

	if (v1->data == NULL) {
		char *msg = (char *)malloc(64);
		memset(msg, 0, 64);
		sprintf(msg, "Variable is NONE [%s]", c->args[0].data);
		c->exception = raise(msg, c->pid, ERR_INVALID_PARAMETER_TYPE);
		free(msg);
		return -1;
	}
	if (v1->type != TYPE_NUM && v1->type != TYPE_BYTE) {
		char *msg = (char *)malloc(64);
		memset(msg, 0, 64);
		sprintf(msg, "Variable 1 type should be NUMBER or BYTE got [%s]", type_tostr(v1->type));
		c->exception = raise(msg, c->pid, ERR_INVALID_PARAMETER_TYPE);
		free(msg);
		return -1;
	}
	if (v2->type != TYPE_NUM && v1->type != TYPE_BYTE) {
		char *msg = (char *)malloc(64);
		memset(msg, 0, 64);
		sprintf(msg, "Variable 2 type should be NUMBER or BYTE got [%s]", type_tostr(v2->type));
		c->exception = raise(msg, c->pid, ERR_INVALID_PARAMETER_TYPE);
		free(msg);
		return -1;
	}
#endif
	return 0;
}

int command_lrotate(command *c, program *_p) {
	variable *v1;
	variable *v2 = get_var(c, 1);
	v1 = find_variable(c->args[0].data, c->pid);
	int check = _command_validations(c, v1, v2);
	if (check == -1) {
		return check;
	}
	char byte = 0;
	if (v1->type == TYPE_NUM)
		byte = char(ctod(v1->data));
	if (v1->type == TYPE_BYTE)
		byte = char(v1->data[0]);

	int bits = int(ctod(v2->data));
	byte = (byte << bits) | (byte >> (BITS - bits));

	if (v1->type == TYPE_NUM)
		v1->data = dtoc(double(byte));
	if (v1->type == TYPE_BYTE)
		v1->data[0] = byte;
	return 0;
}

int command_rrotate(command *c, program *_p) {
	variable *v1;
	variable *v2 = get_var(c, 1);
	v1 = find_variable(c->args[0].data, c->pid);
	int check = _command_validations(c, v1, v2);
	if (check == -1) {
		return check;
	}
	char byte = 0;
	if (v1->type == TYPE_NUM)
		byte = char(ctod(v1->data));
	if (v1->type == TYPE_BYTE)
		byte = char(v1->data[0]);

	int bits = int(ctod(v2->data));
	byte = (byte >> bits) | (byte << (BITS - bits));

	if (v1->type == TYPE_NUM)
		v1->data = dtoc(double(byte));
	if (v1->type == TYPE_BYTE)
		v1->data[0] = byte;
	return 0;
}

int command_lshift(command *c, program *_p) {
	variable *v1;
	variable *v2 = get_var(c, 1);
	v1 = find_variable(c->args[0].data, c->pid);
	int check = _command_validations(c, v1, v2);
	if (check == -1) {
		return check;
	}
	char byte = 0;
	if (v1->type == TYPE_NUM)
		byte = char(ctod(v1->data));
	if (v1->type == TYPE_BYTE)
		byte = char(v1->data[0]);

	int bits = int(ctod(v2->data));
	byte = byte << bits;

	if (v1->type == TYPE_NUM)
		v1->data = dtoc(double(byte));
	if (v1->type == TYPE_BYTE)
		v1->data[0] = byte;
	return 0;
}

int command_rshift(command *c, program *_p) {
	variable *v1;
	variable *v2 = get_var(c, 1);
	v1 = find_variable(c->args[0].data, c->pid);
	int check = _command_validations(c, v1, v2);
	if (check == -1) {
		return check;
	}
	char byte = 0;
	if (v1->type == TYPE_NUM)
		byte = char(ctod(v1->data));
	if (v1->type == TYPE_BYTE)
		byte = char(v1->data[0]);

	int bits = int(ctod(v2->data));
	byte = byte >> bits;

	if (v1->type == TYPE_NUM)
		v1->data = dtoc(double(byte));
	if (v1->type == TYPE_BYTE)
		v1->data[0] = byte;
	return 0;
}
