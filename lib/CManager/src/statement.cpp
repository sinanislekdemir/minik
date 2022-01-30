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

/**
   This looks like a stupid function in the beginning
   because it returns the same string given BUT
   in-fact it's returning a memory location from
   stack, not heap. So, it's memory efficient when
   commands are repeating.
 */
const char *find_statement(const char *cmd) {
	if (strcmp(cmd, "SPRINT") == 0)
		return STATEMENT_SPRINT;
	if (strcmp(cmd, "SPRINTLN") == 0)
		return STATEMENT_SPRINTLN;
	if (strcmp(cmd, "SREADLN") == 0)
		return STATEMENT_SREADLN;
	if (strcmp(cmd, "GOTO") == 0)
		return STATEMENT_GOTO;
	if (strcmp(cmd, "CALL") == 0)
		return STATEMENT_CALL;
	if (strcmp(cmd, "JE") == 0)
		return STATEMENT_JE;
	if (strcmp(cmd, "JNE") == 0)
		return STATEMENT_JNE;
	if (strcmp(cmd, "JG") == 0)
		return STATEMENT_JG;
	if (strcmp(cmd, "JGE") == 0)
		return STATEMENT_JGE;
	if (strcmp(cmd, "JL") == 0)
		return STATEMENT_JL;
	if (strcmp(cmd, "JLE") == 0)
		return STATEMENT_JLE;
	if (strcmp(cmd, "CMP") == 0)
		return STATEMENT_CMP;
	if (strcmp(cmd, "LOG") == 0)
		return STATEMENT_LOG;
	if (strcmp(cmd, "ADD") == 0)
		return STATEMENT_ADD;
	if (strcmp(cmd, "SUB") == 0)
		return STATEMENT_SUB;
	if (strcmp(cmd, "DIV") == 0)
		return STATEMENT_DIV;
	if (strcmp(cmd, "MUL") == 0)
		return STATEMENT_MUL;
	if (strcmp(cmd, "XOR") == 0)
		return STATEMENT_XOR;
	if (strcmp(cmd, "OR") == 0)
		return STATEMENT_OR;
	if (strcmp(cmd, "AND") == 0)
		return STATEMENT_AND;
	if (strcmp(cmd, "POW") == 0)
		return STATEMENT_POW;
	if (strcmp(cmd, "SIN") == 0)
		return STATEMENT_SIN;
	if (strcmp(cmd, "COS") == 0)
		return STATEMENT_COS;
	if (strcmp(cmd, "TAN") == 0)
		return STATEMENT_TAN;
	if (strcmp(cmd, "COT") == 0)
		return STATEMENT_COT;
	if (strcmp(cmd, "SINH") == 0)
		return STATEMENT_SINH;
	if (strcmp(cmd, "COSH") == 0)
		return STATEMENT_COSH;
	if (strcmp(cmd, "TANH") == 0)
		return STATEMENT_TANH;
	if (strcmp(cmd, "COTH") == 0)
		return STATEMENT_COTH;
	if (strcmp(cmd, "ASIN") == 0)
		return STATEMENT_ASIN;
	if (strcmp(cmd, "ACOS") == 0)
		return STATEMENT_ACOS;
	if (strcmp(cmd, "ATAN") == 0)
		return STATEMENT_ATAN;
	if (strcmp(cmd, "ACOT") == 0)
		return STATEMENT_ACOT;
	if (strcmp(cmd, "ASINH") == 0)
		return STATEMENT_ASINH;
	if (strcmp(cmd, "ACOSH") == 0)
		return STATEMENT_ACOSH;
	if (strcmp(cmd, "ATANH") == 0)
		return STATEMENT_ATANH;
	if (strcmp(cmd, "ACOTH") == 0)
		return STATEMENT_ACOTH;
	if (strcmp(cmd, "HALT") == 0)
		return STATEMENT_HALT;
	if (strcmp(cmd, "ALLOC") == 0)
		return STATEMENT_ALLOC;
	if (strcmp(cmd, "APPEND") == 0)
		return STATEMENT_APPEND;
	if (strcmp(cmd, "SET") == 0)
		return STATEMENT_SET;
	if (strcmp(cmd, "STR") == 0)
		return STATEMENT_STR;
	if (strcmp(cmd, "NUM") == 0)
		return STATEMENT_NUM;
	if (strcmp(cmd, "DEL") == 0)
		return STATEMENT_DEL;
	if (strcmp(cmd, "CPY") == 0)
		return STATEMENT_CPY;
	if (strcmp(cmd, "LROTATE") == 0)
		return STATEMENT_LROTATE;
	if (strcmp(cmd, "RROTATE") == 0)
		return STATEMENT_RROTATE;
	if (strcmp(cmd, "LSHIFT") == 0)
		return STATEMENT_LSHIFT;
	if (strcmp(cmd, "RSHIFT") == 0)
		return STATEMENT_RSHIFT;
	if (strcmp(cmd, "SLEEP") == 0)
		return STATEMENT_SLEEP;
	if (strcmp(cmd, "MILLIS") == 0)
		return STATEMENT_MILLIS;
	if (strcmp(cmd, "INT") == 0)
		return STATEMENT_INT;
	if (strcmp(cmd, "CORE") == 0)
		return STATEMENT_CORE;
	if (strcmp(cmd, "SYS") == 0)
		return STATEMENT_SYS;
	if (strcmp(cmd, "AREAD") == 0)
		return STATEMENT_AREAD;
	if (strcmp(cmd, "AWRITE") == 0)
		return STATEMENT_AWRITE;
	if (strcmp(cmd, "PINMODE") == 0)
		return STATEMENT_PINMODE;
	if (strcmp(cmd, "DWRITE") == 0)
		return STATEMENT_DWRITE;
	if (strcmp(cmd, "DREAD") == 0)
		return STATEMENT_DREAD;
	return NULL;
}

void register_statements() {
	/**
	   ATTENTION!
	   Order of lines below would greatly influence execution speed;
	   Keep the most common commands on top
	 */
	add_statement(STATEMENT_SPRINT, command_serial_print);
	add_statement(STATEMENT_SPRINTLN, command_serial_println);
	add_statement(STATEMENT_SREADLN, command_getln);
	add_statement(STATEMENT_GOTO, command_goto);
	add_statement(STATEMENT_CALL, command_call);

	add_statement(STATEMENT_JE, command_je);
	add_statement(STATEMENT_JNE, command_jne);
	add_statement(STATEMENT_JG, command_jg);
	add_statement(STATEMENT_JGE, command_jge);
	add_statement(STATEMENT_JL, command_jl);
	add_statement(STATEMENT_JLE, command_jle);
	add_statement(STATEMENT_CMP, command_cmp);

	add_statement(STATEMENT_LOG, command_log);

	add_statement(STATEMENT_ADD, command_add);
	add_statement(STATEMENT_SUB, command_sub);
	add_statement(STATEMENT_DIV, command_div);
	add_statement(STATEMENT_MUL, command_mul);
	add_statement(STATEMENT_XOR, command_xor);
	add_statement(STATEMENT_OR, command_or);
	add_statement(STATEMENT_AND, command_and);
	add_statement(STATEMENT_POW, command_pow);
	add_statement(STATEMENT_SIN, command_trigonometry);
	add_statement(STATEMENT_COS, command_trigonometry);
	add_statement(STATEMENT_TAN, command_trigonometry);
	add_statement(STATEMENT_COT, command_trigonometry);
	add_statement(STATEMENT_SINH, command_trigonometry);
	add_statement(STATEMENT_COSH, command_trigonometry);
	add_statement(STATEMENT_TANH, command_trigonometry);
	add_statement(STATEMENT_COTH, command_trigonometry);
	add_statement(STATEMENT_ASIN, command_trigonometry);
	add_statement(STATEMENT_ACOS, command_trigonometry);
	add_statement(STATEMENT_ATAN, command_trigonometry);
	add_statement(STATEMENT_ACOT, command_trigonometry);
#ifdef BOARD_ESP32
	add_statement(STATEMENT_ASINH, command_trigonometry);
	add_statement(STATEMENT_ACOSH, command_trigonometry);
	add_statement(STATEMENT_ATANH, command_trigonometry);
	add_statement(STATEMENT_ACOTH, command_trigonometry);
#endif

	add_statement(STATEMENT_HALT, command_halt);
	add_statement(STATEMENT_ALLOC, command_alloc);
	add_statement(STATEMENT_APPEND, command_append);
	add_statement(STATEMENT_SET, command_set);
	add_statement(STATEMENT_STR, command_str);
	add_statement(STATEMENT_NUM, command_num);
	add_statement(STATEMENT_DEL, command_del);
	add_statement(STATEMENT_CPY, command_cpy);
	add_statement(STATEMENT_LROTATE, command_lrotate);
	add_statement(STATEMENT_RROTATE, command_rrotate);
	add_statement(STATEMENT_LSHIFT, command_lshift);
	add_statement(STATEMENT_RSHIFT, command_rshift);

	add_statement(STATEMENT_SLEEP, command_sleep);
	add_statement(STATEMENT_MILLIS, command_millis);
	add_statement(STATEMENT_INT, command_int);
	add_statement(STATEMENT_CORE, command_core);
	add_statement(STATEMENT_SYS, command_sys);

	add_statement(STATEMENT_AREAD, command_analogread);
	add_statement(STATEMENT_AWRITE, command_analogwrite);
	add_statement(STATEMENT_PINMODE, command_pinmode);
	add_statement(STATEMENT_DWRITE, command_digitalwrite);
	add_statement(STATEMENT_DREAD, command_digitalread);
}
