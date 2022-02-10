#include "cmd_navigate.hpp"
#include "helpers.hpp"

int command_call(command *c, program *p) {
	sub *s = p->find_sub(c->args[0].name);
#ifndef DISABLE_EXCEPTIONS
	if (s == NULL) {
		c->exception = raise("Call location not found", c->pid, ERR_ADDRESS_NOT_FOUND);
		return -1;
	}
#endif
	p->append_to_history(p->cursor, c);
	p->cursor = s;
	p->cursor->cursor = p->cursor->root_instruction;
	return 1; // don't change cursor
}

int command_goto(command *c, program *p) {
#ifndef DISABLE_EXCEPTIONS
	if (c->argc < 1) {
		c->exception = raise("GOTO target is not defined", c->pid, ERR_ADDRESS_NOT_FOUND);
		return -1;
	}
#endif
	variable *goto_location = get_var(c, 0);
#ifndef DISABLE_EXCEPTIONS
	if (goto_location->type != TYPE_NUM) {
		c->exception = raise("GOTO target is not a valid NUMBER", c->pid, ERR_ADDRESS_NOT_FOUND);
		return -1;
	}
#endif

	int line = int(ctod(goto_location->data));
	int counter = 0;
	command *node = p->cursor->root_instruction;
	while (node != NULL) {
		if (counter == line) {
			p->cursor->cursor = node;
			break;
		}
		counter++;
		node = node->next;
	}

	return 1;
}

int command_halt(command *c, program *p) { return -1; }
