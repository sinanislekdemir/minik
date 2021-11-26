#include "tokenizer.hpp"
#include "constants.hpp"
#include "helpers.hpp"

command *parse(const char *cmd, unsigned int pid) {
	command *result = (command *)malloc(sizeof(command));
	unsigned int argument_count = argc(cmd, ' ');
	unsigned int command_size = extract_size(cmd, ' ', 0);
	unsigned int argument_size;
	result->cmd = extract(cmd, ' ', 0, command_size + 1);

	variable *args = new variable[argument_count];

	for (unsigned int i = 0; i < argument_count - 1; i++) {
		argument_size = extract_size(cmd, ' ', i + 1);
		char *temp = extract(cmd, ' ', i + 1, argument_size + 1);

		args[i].type = arg_type(temp);

		args[i].pid = pid;
		args[i].deleted = false;
		args[i].name = (char *)malloc(strlen(temp) + 1);
		args[i].next = NULL;

		memset(args[i].name, 0, strlen(temp) + 1);
		strcpy(args[i].name, temp);

		if (args[i].type == TYPE_NUM) {
			args[i].datasize = sizeof(double);
			args[i].data = dtoc(atof(temp));
		} else {
			int lshift = 0;
			int rshift = 0;
			if (temp[0] == '"') {
				lshift = 1;
				rshift = 2;
			}
			args[i].data = (char *)malloc(argument_size + 1);
			memset(args[i].data, 0, argument_size + 1);
			memcpy(args[i].data, temp + lshift, argument_size - rshift);
			args[i].datasize = argument_size;
		}
		free(temp);
	}

	result->argc = argument_count - 1;
	result->args = args;
	result->pid = pid;
	result->next = NULL;
	result->exception = NULL;
	return result;
}
