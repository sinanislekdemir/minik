#include "cmd_log.hpp"

int output = LOG_SERIAL;

void set_output(int output_type) { output = output_type; }
/**
 * @brief We need a better logging mechanism here
 * 
 * @param c 
 * @param p 
 * @return int 
 */
int command_log(command c, program *p) {
	return 0;
}
