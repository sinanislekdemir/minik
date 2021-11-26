#include "interpreter.hpp"
#include "statement.hpp"

extern statement *root_statement;

int run(command *c, program *_p) {
	statement *node = root_statement;
	while (node != NULL) {
		if (strcmp(c->cmd, node->command) == 0) {
			return node->f(c, _p);
		}
		node = node->next;
	}

#ifndef DISABLE_EXCEPTIONS
	char *msg = (char *)malloc(64);
	memset(msg, 0, 64);
	sprintf(msg, "Invalid command [%s]", c->cmd);
	c->exception = raise(msg, c->pid, ERR_INVALID_COMMAND);
	free(msg);
#endif

	return -1;
}
