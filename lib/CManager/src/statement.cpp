#include "statement.hpp"
#include "cmd_analog.hpp"
#include "cmd_digital.hpp"
#include "cmd_interrupts.hpp"
#include "cmd_log.hpp"
#include "cmd_logic.hpp"
#include "cmd_math.hpp"
#include "cmd_navigate.hpp"
#include "cmd_serial.hpp"
#include "cmd_system.hpp"
#include "cmd_time.hpp"
#include "cmd_variable.hpp"
#include "constants.hpp"
#include "helpers.hpp"

statement *root_statement = NULL;

void add_statement(const char *cmd, int (*f)(command *c, program *_p)) {
	if (root_statement == NULL) {
		root_statement = (statement *)malloc(sizeof(statement));
		root_statement->command = cmd;
		root_statement->f = f;
		root_statement->next = NULL;
		return;
	}
	statement *node = root_statement;
	while (node != NULL) {
		if (node->next == NULL) {
			node->next = (statement *)malloc(sizeof(statement));
			node->next->command = cmd;
			node->next->f = f;
			node->next->next = NULL;
			break;
		}
		node = node->next;
	}
}

void register_statements() {
	/**
	   ATTENTION!
	   Order of lines below would greatly influence execution speed;
	   Keep the most common commands on top
	 */
	add_statement("SPRINT", command_serial_print);
	add_statement("SPRINTLN", command_serial_println);
	add_statement("SREADLN", command_getln);
	add_statement("GOTO", command_goto);
	add_statement("CALL", command_call);

	add_statement("JE", command_je);
	add_statement("JNE", command_jne);
	add_statement("JG", command_jg);
	add_statement("JGE", command_jge);
	add_statement("JL", command_jl);
	add_statement("JLE", command_jle);
	add_statement("CMP", command_cmp);

	add_statement("LOG", command_log);

	add_statement("ADD", command_add);
	add_statement("SUB", command_sub);
	add_statement("DIV", command_div);
	add_statement("MUL", command_mul);
	add_statement("XOR", command_xor);
	add_statement("OR", command_or);
	add_statement("AND", command_and);
	add_statement("POW", command_pow);
	add_statement("SIN", command_trigonometry);
	add_statement("COS", command_trigonometry);
	add_statement("TAN", command_trigonometry);
	add_statement("COT", command_trigonometry);
	add_statement("SINH", command_trigonometry);
	add_statement("COSH", command_trigonometry);
	add_statement("TANH", command_trigonometry);
	add_statement("COTH", command_trigonometry);
	add_statement("ASIN", command_trigonometry);
	add_statement("ACOS", command_trigonometry);
	add_statement("ATAN", command_trigonometry);
	add_statement("ACOT", command_trigonometry);
#ifdef BOARD_ESP32
	add_statement("ASINH", command_trigonometry);
	add_statement("ACOSH", command_trigonometry);
	add_statement("ATANH", command_trigonometry);
	add_statement("ACOTH", command_trigonometry);
#endif

	add_statement("HALT", command_halt);
	add_statement("ALLOC", command_alloc);
	add_statement("APPEND", command_append);
	add_statement("SET", command_set);
	add_statement("STR", command_str);
	add_statement("NUM", command_num);
	add_statement("DEL", command_del);
	add_statement("CPY", command_cpy);
	add_statement("LROTATE", command_lrotate);
	add_statement("RROTATE", command_rrotate);
	add_statement("LSHIFT", command_lshift);
	add_statement("RSHIFT", command_rshift);

	add_statement("SLEEP", command_sleep);
	add_statement("MILLIS", command_millis);
	add_statement("INT", command_int);
	add_statement("CORE", command_core);
	add_statement("SYS", command_sys);

	add_statement("AREAD", command_analogread);
	add_statement("AWRITE", command_analogwrite);
	add_statement("PINMODE", command_pinmode);
	add_statement("DWRITE", command_digitalwrite);
	add_statement("DREAD", command_digitalread);
}
