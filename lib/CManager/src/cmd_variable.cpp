#include "cmd_variable.hpp"
#include "helpers.hpp"
#include <Arduino.h>

extern error err;

int command_set(command *c, program *_p) {
#ifndef DISABLE_EXCEPTIONS
	if (validate_command(c, 2) != 0) {
		return -1;
	}
#endif

#ifndef DISABLE_EXCEPTIONS
	if (c->args[0].type != TYPE_VARIABLE) {
		c->exception = raise(ERR_STR_INVALID_TYPE, c->pid, ERR_INVALID_PARAMETER_TYPE);
		return -1;
	}
#endif

	variable *v_check = find_variable(c->args[0].data, c->pid);
	variable *set = get_var(c, 1);

	if (v_check == NULL) {
		variable *v = (variable *)malloc(sizeof(variable));
		v->next = NULL;
		v->pid = _p->pid;

		unsigned int size = strlen(c->args[0].data) + 1;
		v->name = (char *)malloc(size);
		memset(v->name, 0, size);
		strcpy(v->name, c->args[0].data);

		v->datasize = set->datasize;
		v->deleted = false;
		v->data = (char *)malloc(set->datasize);

		memcpy(v->data, set->data, set->datasize);

		v->pid = c->pid;
		v->type = set->type;
		new_variable(v);
	} else {
		free(v_check->data);
		v_check->datasize = set->datasize;
		v_check->deleted = false;
		v_check->type = set->type;
		v_check->data = (char *)malloc(set->datasize);
		memcpy(v_check->data, set->data, set->datasize);
	}
	return 0;
}

int command_cpy(command *c, program *_p) {
#ifndef DISABLE_EXCEPTIONS
	if (validate_command(c, 4) != 0) {
		return -1;
	}

	if (c->args[0].type != TYPE_VARIABLE || c->args[3].type != TYPE_VARIABLE) {
		char *msg = (char *)malloc(64);
		memset(msg, 0, 64);
		strcat(msg, "COPY arguments 1 or 3 are not variable");
		c->exception = raise(msg, c->pid, ERR_INVALID_PARAMETER_TYPE);
		free(msg);
		return -1;
	}
#endif

	variable *source = find_variable(c->args[3].data, c->pid);
#ifndef DISABLE_EXCEPTIONS
	if (source == NULL) {
		c->exception = raise(ERR_STR_SOURCE_NONE, c->pid, ERR_VARIABLE_NOT_FOUND);
		return -1;
	}
#endif

	variable *dest = find_variable(c->args[0].data, c->pid);
	if (dest == NULL) {
		dest = (variable *)malloc(sizeof(variable));
		dest->next = NULL;
		dest->data = NULL;
		dest->type = TYPE_STR;
		dest->pid = c->pid;
		dest->name = (char *)malloc(strlen(c->args[0].data) + 1);
		memset(dest->name, 0, strlen(c->args[0].data) + 1);
		strcpy(dest->name, c->args[0].data);
		new_variable(dest);
	}

	variable *from = get_var(c, 1);
	variable *size = get_var(c, 2);
#ifndef DISABLE_EXCEPTIONS
	if (from->type != TYPE_NUM || size->type != TYPE_NUM) {
		char *msg = (char *)malloc(64);
		memset(msg, 0, 64);
		strcat(msg, "Copy range is not numeric");
		c->exception = raise(msg, c->pid, ERR_INVALID_PARAMETER_TYPE);
		free(msg);
		return -1;
	}
#endif

	int from_int = int(ctod(from->data));
	int size_int = int(ctod(size->data));
	if (dest->data != NULL) {
		free(dest->data);
	}
	dest->datasize = (size_int + 1);
	dest->data = (char *)malloc(size_int + 1);
	memset(dest->data, 0, size_int + 1);
	memcpy(dest->data, source->data + from_int, size_int);
	return 0;
}

int command_del(command *c, program *_p) {
	variable *dest = find_variable(c->args[0].data, c->pid);
	if (dest == NULL) {
		return 0;
	}
	free_variable(c->args[0].data, c->pid);
	return 0;
}

int command_str(command *c, program *_p) {
	variable *src = get_var(c, 1);
#ifndef DISABLE_EXCEPTIONS
	if (src == NULL) {
		c->exception = raise(ERR_STR_SOURCE_NONE, c->pid, ERR_VARIABLE_NOT_FOUND);
		return -1;
	}
#endif
	double d = ctod(src->data);
	char *data = (char *)malloc(32);
	memset(data, 0, 32);
	dtostrf(d, 32, 4, data);
	new_string(c->args[0].data, data, 32, c->pid);
	return 0;
}

int command_num(command *c, program *_p) {
	variable *src = get_var(c, 1);
#ifndef DISABLE_EXCEPTIONS
	if (src == NULL) {
		c->exception = raise(ERR_STR_SOURCE_NONE, c->pid, ERR_VARIABLE_NOT_FOUND);
		return -1;
	}
#endif
	double val = atof(src->data);
	new_number(c->args[0].data, val, c->pid);

	return 0;
}

int command_alloc(command *c, program *_p) {
#ifndef DISABLE_EXCEPTIONS
	if (c->args[0].type != TYPE_VARIABLE) {
		c->exception = raise(ERR_STR_INVALID_TYPE, c->pid, ERR_VARIABLE_NOT_FOUND);
		return -1;
	}
#endif
	variable *size_var = get_var(c, 1);
#ifndef DISABLE_EXCEPTIONS
	if (size_var->type != TYPE_NUM) {
		c->exception = raise(ERR_STR_INVALID_TYPE, c->pid, ERR_VARIABLE_NOT_FOUND);
		return -1;
	}
#endif
	int size = int(ctod(size_var->data));
	char *data = (char *)malloc(size);
	memset(data, 0, size);
	new_string(c->args[0].data, data, size, c->pid);
	free(data);
	return 0;
}

int command_append(command *c, program *_p) {
#ifndef DISABLE_EXCEPTIONS
	if (c->args[0].type != TYPE_VARIABLE) {
		c->exception = raise(ERR_STR_INVALID_TYPE, c->pid, ERR_VARIABLE_NOT_FOUND);
		return -1;
	}
#endif
	variable *var = find_variable(c->args[0].data, c->pid);
	variable *val = get_var(c, 1);

#ifndef DISABLE_EXCEPTIONS
	if (var == NULL) {
		c->exception = raise(ERR_STR_VAR_NOT_FOUND, c->pid, ERR_VARIABLE_NOT_FOUND);
		return -1;
	}
#endif

	int empty_location = -1;
	for (unsigned int i = 0; i < var->datasize; i++) {
		if (var->data[i] == 0) {
			empty_location = i;
			break;
		}
	}

#ifndef DISABLE_EXCEPTIONS
	if (empty_location == -1) {
		c->exception = raise(ERR_STR_VARIABLE_FULL, c->pid, ERR_VARIABLE_NOT_FOUND);
		return -1;
	}
#endif
	unsigned int free_space = var->datasize - empty_location;
	unsigned int copy_bytes = val->datasize;
	if (copy_bytes > free_space) {
		copy_bytes = free_space;
	}
	for (unsigned int i = 0; i < copy_bytes; i++) {
		if (val->data[i] != 0)
			var->data[empty_location++] = val->data[i];
	}
	return 0;
}
