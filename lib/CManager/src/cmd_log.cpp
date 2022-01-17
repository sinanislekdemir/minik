#include "cmd_log.hpp"

int output = LOG_SERIAL;

void set_output(int output_type) { output = output_type; }
int command_log(command *c, program *_p) {
	char *typestr = (char *)malloc(12);
	memset(typestr, 0, 12);
	if (output == LOG_SERIAL) {
		Serial.print("Command: ");
		Serial.println(c->cmd);
		for (unsigned int i = 0; i < c->argc; i++) {
			Serial.print("    ");
			Serial.print(c->args[i].data);
			switch (c->args[i].type) {
			case TYPE_STR:
				strcpy(typestr, " (str)");
				break;
			case TYPE_NUM:
				strcpy(typestr, " (num)");
				break;
			case TYPE_CONSTANT:
				strcpy(typestr, " (const)");
				break;
			case TYPE_VARIABLE:
				strcpy(typestr, " (var)");
				break;
			case TYPE_LABEL:
				strcpy(typestr, " (lab)");
				break;
			case TYPE_REGISTER:
				strcpy(typestr, " (reg)");
				break;
			case TYPE_FILE:
				strcpy(typestr, " (file)");
				break;
			case TYPE_BYTE:
				strcpy(typestr, " (byte)");
			}
			Serial.println(typestr);
		}
	}
	free(typestr);
	return 0;
}
