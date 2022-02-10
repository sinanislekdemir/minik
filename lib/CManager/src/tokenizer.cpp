#include "tokenizer.hpp"
#include "constants.hpp"
#include "helpers.hpp"
#include "statement.hpp"

extern command commands[MAX_CMDS];

int parse(const char *cmd, unsigned int pid, int index) {
	unsigned int argument_count = argc(cmd, ' ') - 1;
	char temp_buffer[MAX_LINE_LENGTH] = {0};
	extract(cmd, ' ', 0, temp_buffer);
	if (argument_count > 3) {
		argument_count = 3;
	}

	commands[index].statement = find_statement((const char *)temp_buffer);
	commands[index].pid = pid;
	commands[index].exception = false;
	commands[index].arg_count = argument_count;

	for (unsigned int i = 0; i < argument_count; i++) {
		memset(temp_buffer, 0, MAX_LINE_LENGTH);
		int check = extract(cmd, ' ', i + 1, temp_buffer);
		if (check == 0) {
			break;
		}
		commands[index].variable_type[i] = arg_type(temp_buffer);
		if (commands[index].variable_type[i] == TYPE_NUM) {
			commands[index].variable_index[i] = new_number((char *)"", atof(temp_buffer), pid);
		}
		if (commands[i].variable_type[i] == TYPE_STR) {
			char sbuffer[MAX_LINE_LENGTH] = {0};
			memcpy(sbuffer, temp_buffer + 1, strlen(temp_buffer) - 2);
			commands[index].variable_index[i] = new_string((char *)"", sbuffer, pid);
		}
		if (commands[index].variable_type[i] == TYPE_BYTE) {
			char sbuffer[1] = {0};
			sbuffer[0] = (char)strtol(temp_buffer, NULL, 0);
			commands[index].variable_index[i] = new_string((char *)"", sbuffer, pid);
		}
		if (commands[index].variable_type[i] == TYPE_VARIABLE) {
			commands[index].variable_index[i] = -1; // unknown yet
		}
	}
	return 0;
}
