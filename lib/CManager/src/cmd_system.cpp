#include "cmd_system.hpp"
#include "display_ssd1306.hpp"
#include "helpers.hpp"
#include "wifi.hpp"

int command_core(command *c, program *_p) {
	int core = 0;
#ifdef BOARD_ESP32
	core = xPortGetCoreID();
#endif
	new_number(c->args[0].data, double(core), c->pid);
	return 0;
}

int command_sys(command *c, program *_p) {
	variable *call = get_var(c, 0);
	int call_num = int(ctod(call->data));
#ifdef WITH_WIFI
	if (call_num == 10) {
		return wifi(_p);
	}
	if (call_num == 11) {
		return server(_p);
	}
#endif
#ifdef WITH_DISPLAY_SSD1306
	if (call_num == 12) {
		return display_ssd1306(_p);
	}
#endif
	return 0;
};
