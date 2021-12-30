#include "pins.hpp"
#include "helpers.hpp"

int *pins_def(command *c, program *_p) {
#ifndef DISABLE_EXCEPTIONS
	if (c->argc != 2) {
		char *msg = (char *)malloc(64);
		memset(msg, 0, 64);
		sprintf(msg, ERR_STR_NOT_ENOUGH_PARAMS, c->cmd, 2, c->argc);
		c->exception = raise(msg, c->pid, ERR_ADDRESS_NOT_FOUND);
		free(msg);
		return NULL;
	}
#endif
	variable *pin = get_var(c, 0);
	unsigned int a_pin = int(ctod(pin->data));
	variable *val = get_var(c, 1);
	int a_val = int(ctod(val->data));

	int *res = (int *)malloc(sizeof(int) * 2);
	res[0] = a_pin;
	res[1] = a_val;
	return res;
}
