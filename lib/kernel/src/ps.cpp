#include "ps.hpp"
#include "kprint.hpp"
#include "program.hpp"
#include "tasks.hpp"

extern program programs[MAX_PROGRAM_COUNT];
int process_list(const char *io_buffer) {
	if (strcmp(io_buffer, "ps") == 0) {
		char tmp[32] = {0};
		for (unsigned int i = 0; i < MAX_PROGRAM_COUNT; i++) {
			if (programs[i].pid > 0) {
				kprint("------\n");
				kprint("PID: ");
				ltoa(programs[i].pid, tmp, DEC);
				kprint(tmp);
				kprint("\nStart: ");
				memset(tmp, 0, 32);
				ltoa(programs[i].start_time, tmp, DEC);
				kprint(tmp);
				kprint("\nUp: ");
				unsigned long now = millis();
				unsigned long diff = now - programs[i].start_time;
				memset(tmp, 0, 32);
				ltoa(diff, tmp, DEC);
				kprint(tmp);
				kprint("\n");
			}
		}
	}
	if (strncmp(io_buffer, "kill ", 5) == 0) {
		char tmp[32] = {0};
		strncpy(tmp, io_buffer + 5, strlen(io_buffer) - 5);
		long pid = atol(tmp);
		if (pid == 0) {
			kprint("Not a valid PID\n");
		}
		for (unsigned int i = 0; i < MAX_PROGRAM_COUNT; i++) {
			if (programs[i].pid == pid) {
				programs[i].status_code = PROGRAM_FREE;
				programs[i].end_time = millis();
				programs[i].pid = 0;
				programs[i].destroy();
				kprint("killed\n");
			}
		}
	}
	return 0;
}
