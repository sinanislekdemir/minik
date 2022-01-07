#include "cmd_system.hpp"

int command_core(command *c, program *_p) {
	int core = 0;
#ifdef BOARD_ESP32
	core = xPortGetCoreID();
#endif
	new_number(c->args[0].data, double(core), c->pid);
	return 0;
}

int command_sys(command *c, program *_p){

};
