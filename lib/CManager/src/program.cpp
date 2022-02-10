#include "program.hpp"
#include "helpers.hpp"
#include "interpreter.hpp"
#include "tokenizer.hpp"

sub _subs[MAX_SUBS] = {0};
extern command commands[MAX_CMDS];

int next_sub() {
	for (unsigned short i = 0; i < MAX_SUBS; i++) {
		if (_subs[i].pid == 0) {
			return i;
		}
	}
	return -1;
}

int sub_command_count(short sub) {
	for (unsigned int i = 0; i < MAX_SUB_COMMANDS; i++) {
		if (_subs[sub].commands[i] == -1) {
			return i;
		}
	}
	return -1;
}

int next_command_index() {
	for (unsigned int i = 0; i < MAX_CMDS; i++) {
		if (commands[i].pid == 0) {
			return i;
		}
	}
	return -1;
}

int sub_next_command(short sub) {
	for (unsigned int i = 0; i < MAX_SUB_COMMANDS; i++) {
		if (_subs[sub].commands[i] == -1) {
			return i;
		}
	}
	return -1;
}

program::program(char pid) {
	if (pid == 0) {
		pid = 1;
	}
	memset(this->back_sub_history, -1, PROG_SUBS);
	memset(this->subs, -1, PROG_SUBS);

	for (char i = 0; i < 8; i++) {
		this->interrupts[i].pin = 0;
		this->interrupts[i].state = 0;
		this->interrupts[i].triggered = false;
		this->interrupts[i].routine = -1;
	}

	this->pid = pid;
	this->exit_code = 0;
	this->valid_sub_count = 0;

	this->_sleep = false;
	this->_sleep_start = 0;
	this->_sleep_duration = 0;
}

program::~program() { this->destroy(); }

void program::register_interrupt(char pin, unsigned int state, char routine) {
	for (char i = 0; i < 8; i++) {
		if (this->interrupts[i].routine == -1) {
			this->interrupts[i].pin = pin;
			this->interrupts[i].state = state;
			this->interrupts[i].routine = routine;
			this->interrupts[i].triggered = false;
		}
	}
}

void program::destroy() {
	for (char i = 0; i < PROG_SUBS; i++) {
		_subs[this->subs[i]].pid = 0;
		memset(_subs[this->subs[i]].back_history, 0, 16);
		memset(_subs[this->subs[i]].commands, 0, MAX_SUB_COMMANDS);
		memset(_subs[this->subs[i]].name, 0, 24);
		_subs[this->subs[i]].cursor = 0;
		_subs[this->subs[i]].exit = 0;
	}
	for (unsigned int i = 0; i < MAX_CMDS; i++) {
		if (commands[i].pid == this->pid) {
			memset(commands[i].variable_index, 0, 3);
			memset(commands[i].variable_type, 0, 3);
			commands[i].pid = 0;
			commands[i].statement = 0;
		}
	}
	free_program(this->pid);
}

void program::set_cmp_flag(unsigned int flag) { this->_cmp_flag = flag; }

short program::find_sub(char *name) {
	for (char i = 0; i < PROG_SUBS; i++) {
		if (strcmp(_subs[this->subs[i]].name, name) == 0) {
			return this->subs[i];
		}
	}
	return -1;
}

short program::pop_sub() {
	short result;
	for (char i = PROG_SUBS - 1; i >= 0; i--) {
		if (this->back_sub_history[i] != -1) {
			result = this->back_sub_history[i];
			this->back_sub_history[i] = -1;
			return result;
		}
	}
	return -1;
}

void program::append_to_history(unsigned short cursor, unsigned short instruction) {
	for (char i = 0; i < PROG_SUBS; i++) {
		if (this->back_sub_history[i] == -1) {
			this->back_sub_history[i] = cursor;
			break;
		}
	}
	for (char i = 0; i < 16; i++) {
		if (_subs[this->subs[cursor]].back_history[i] == -1) {
			_subs[this->subs[cursor]].back_history[i] = instruction;
			break;
		}
	}
}

int program::step() {
	int int_status = this->check_interrupts();
	int sub_index = this->subs[this->cursor];

	if (int_status == 1) {
		this->_sleep = false;
		this->_sleep_start = 0;
		this->_sleep_duration = 0;
		return RUNNING;
	}

	if (this->_sleep) {
		unsigned long now = millis();
		if (now - this->_sleep_start >= this->_sleep_duration) {
			this->_sleep = false;
			this->_sleep_start = 0;
			this->_sleep_duration = 0;
		} else {
			return RUNNING;
		}
	}

	if (this->valid_sub_count == 0) {
		for (char i = 0; i < PROG_SUBS; i++) {
			if (this->subs[i] == -1) {
				this->valid_sub_count = i;
				break;
			}
		}
	}

	if (this->cursor >= this->valid_sub_count) {
		return PROGRAM_END;
	}

	if (_subs[sub_index].cursor >= _subs[sub_index].command_count) {
		this->cursor = this->pop_sub();
		return RUNNING;
	}

	unsigned short c = _subs[sub_index].cursor;
	unsigned short command_index = _subs[sub_index].commands[c];
	int result = run(command_index, this->pid);

	if (result == -1) {
		// TODO: raise
		if (commands[command_index].exception) {
			// Check if program has an exception handler
			short handle_sub = this->find_sub((char *)"on_exception");
			if (handle_sub > -1) {
				this->cursor = handle_sub;
				sub_index = this->subs[this->cursor];
				_subs[sub_index].cursor = 0;
				return RUNNING;
			}
		}
		return PROGRAM_END;
	}

	if (result == 1) {
		// Cursor is explicitly set by the instruction
		return RUNNING;
	}

	if (_subs[sub_index].command_count == -1) {
		_subs[sub_index].command_count = sub_command_count(sub_index);
	}
	if (c == _subs[sub_index].command_count - 1) {
		// end of the instructions, head back to caller;
		this->cursor = this->pop_sub();
		if (this->cursor == -1) {
			return PROGRAM_END;
		}
	}
	_subs[sub_index].cursor = c + 1;

	return RUNNING;
}

int program::compile(const char *line) {
	size_t l = strlen(line);
	if (l < 3 || line[0] == '#') {
		return 0;
	}
	// Create a new sub
	if (line[l - 1] == ':') {
		int n = next_sub();
		if (n == -1) {
			return -1;
		}
		short free_sub = -1;
		for (unsigned short i = 0; i < PROG_SUBS; i++) {
			if (this->subs[i] == -1) {
				free_sub = i;
				break;
			}
		}
		this->_compile_cursor = free_sub;
		this->subs[free_sub] = n;

		_subs[n].pid = this->pid;
		memset(_subs[n].name, 0, 24);
		strncpy(_subs[n].name, line, l - 1);
		for (unsigned short i = 0; i < MAX_SUB_COMMANDS; i++) {
			_subs[n].commands[i] = -1;
		}
		for (unsigned short i = 0; i < 16; i++) {
			_subs[n].back_history[i] = -1;
		}
		_subs[n].cursor = 0;
		_subs[n].exit = 0;
		_subs[n].command_count = -1;
		return 0;
	}
	// End the existing sub
	int sub_index = this->subs[this->_compile_cursor];
	if (strcmp(line, SUB_END) == 0) {
		_subs[sub_index].command_count = sub_command_count(this->_compile_cursor);
		return 0;
	}
	int nci = next_command_index();
	int snc = sub_next_command(sub_index);
	parse(line, this->pid, nci);
	_subs[sub_index].commands[snc] = nci;
}

int program::check_interrupts() {
	// Return 1 if it jumps
	for (char i = 0; i < 8; i++) {
		if (this->interrupts[i].pin == 0) {
			continue;
		}
		int val = digitalRead(this->interrupts[i].pin);
		if (val == this->interrupts[i].state && this->interrupts[i].triggered == false) {
			this->interrupts[i].triggered = true;
			this->cursor = this->interrupts[i].routine;
			_subs[this->subs[this->cursor]].cursor = 0;
			return 1;
		}
	}

	return 0;
}
