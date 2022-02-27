#include "interpreter.hpp"
#include "statement.hpp"

extern statement statements[55];
extern command commands[MAX_CMDS];

int run(unsigned short c, program *_p) {
	for (unsigned int i = 0; i < 56; i++) {
		if (commands[c].statement == statements[i].command) {
			return statements[i].f(commands[c], _p);
		}
	}

#ifndef DISABLE_EXCEPTIONS
	commands[c].exception = true;
	error_msg("Unknown command", _p->pid);
#endif

	return -1;
}
