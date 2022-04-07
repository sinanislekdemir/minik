#include "term.hpp"
#include "constants.hpp"
#include "dos.hpp"
#include "eeprom.hpp"
#include "globals.hpp"
#include "kprint.hpp"
#include "memory.hpp"
#include "program.hpp"
#include "ps.hpp"
#include "tasks.hpp"
#include "term_mem.hpp"

extern program programs[MAX_PROGRAM_COUNT];
extern char active_directory[MAX_PATH_SIZE];
extern KernelGlobals KGlobals;

Term::Term() {
	this->has_client = false;

	this->_is_in_readline = false;
	this->io_cursor = 0;

	this->programming_mode = false;
	memset(this->io_buffer, 0, MAX_LINE_LENGTH);
	this->tmp_pid = 0;
	this->io_cursor = 0;
	this->write_eeprom = false;
	this->boot_check = true;
}

Term::~Term() {}

void Term::set_baud_rate(unsigned long rate) {}

void Term::start_server() {
#if defined(BOARD_ESP32) && defined(WITH_WIFI)
	if (!this->terminal_server) {
		this->terminal_server = WiFiServer(TERM_PORT);
		this->terminal_server.begin(TERM_PORT);
	}
#endif
}

void Term::set_output(int out) { KGlobals.set_out_device(out); }

int Term::readline() {
	int dev = KGlobals.get_out_device();
	if (dev == OUT_SERIAL) {
		if (!KGlobals.lock_acquired(0)) {
			return 0;
		}
		if (!Serial.available()) {
			return 0;
		}
		char b = Serial.read();
		if (b > 31 && b < 127) {
			this->io_buffer[io_cursor++] = b;
			Serial.print(b);
		}
		if (b == '\n') {
			Serial.print('\n');
			return 1;
		}
		return 0;
	}
	if (dev == OUT_WIFI) {
#if defined(BOARD_ESP32) && defined(WITH_WIFI)
		KGlobals.terminal_client.readBytesUntil('\n', this->io_buffer, MAX_LINE_LENGTH);
		return 1;
#endif
	}
	return 0;
}

void Term::append_program_buffer() {
	if (strlen(this->io_buffer) < 2) {
		return;
	}
	if (this->tmp_pid != 0) {
		for (unsigned int i = 0; i < MAX_PROGRAM_COUNT; i++) {
			if (programs[i].pid == this->tmp_pid) {
				programs[i].compile(this->io_buffer);
				if (this->write_eeprom) {
					eeprom_write_line(this->io_buffer);
				}
				break;
			}
		}
	}
}

int Term::process() {
	if (!this->wait_for_client()) {
		return 1;
	}
	if (!this->available()) {
		return 1;
	}
	int r = this->readline();
	if (r == 0) {
		return 1;
	}
	if (this->programming_mode) {
		if (strcmp(this->io_buffer, "eeprom") == 0) {
			kprint("storing in eeprom\n");
			eeprom_start_program();
			this->write_eeprom = true;
			memset(this->io_buffer, 0, MAX_LINE_LENGTH);
			this->io_cursor = 0;
			return 1;
		}
		if (strcmp(this->io_buffer, ".") == 0) {
			for (unsigned int i = 0; i < MAX_PROGRAM_COUNT; i++) {
				if (programs[i].pid == this->tmp_pid) {
					kprint("Program compiled\n");
					programs[i].status_code = PROGRAM_RUNNING;
					break;
				}
			}
			this->programming_mode = false;
			memset(this->io_buffer, 0, MAX_LINE_LENGTH);
			this->io_cursor = 0;
			if (this->write_eeprom) {
				eeprom_end_program();
				kprint("Saved in eeprom\n");
				this->write_eeprom = false;
			}
			return 1;
		}
		this->append_program_buffer();
		memset(this->io_buffer, 0, MAX_LINE_LENGTH);
		this->io_cursor = 0;
		return 1;
	}

	if (strcmp(this->io_buffer, "program") == 0) {
		kprint("Programming mode activated\n");
		kprint("End programming with a dot (.) as a single line\n");
		kprint("ready to receive\n");
		this->programming_mode = true;
		this->tmp_pid = KGlobals.get_next_pid();
		for (unsigned int i = 0; i < MAX_PROGRAM_COUNT; i++) {
			if (programs[i].pid == 0) {
				programs[i].set_pid(this->tmp_pid);
				programs[i].status_code = PROGRAM_COMPILING;
				programs[i].start_time = 0;
				programs[i].end_time = 0;
				break;
			}
		}
		memset(this->io_buffer, 0, MAX_LINE_LENGTH);
		this->io_cursor = 0;
		return 1;
	}
	term_mem(this->io_buffer);
	process_list(this->io_buffer);
	dos_check(this->io_buffer);
	kprint("> ");
	if (strlen(active_directory)) {
		kprint(" [");
		kprint(active_directory);
		kprint("] ");
	}
	memset(this->io_buffer, 0, MAX_LINE_LENGTH);
	this->io_cursor = 0;
	return 1;
}

bool Term::wait_for_client() {
#if defined(BOARD_ESP32) && defined(WITH_WIFI)
	if (KGlobals.terminal_client.connected()) {
		this->set_output(OUT_WIFI);
		return true;
	}
	if (this->terminal_server && this->terminal_server.hasClient()) {
		KGlobals.terminal_client = this->terminal_server.available();
		this->set_output(OUT_WIFI);
		kprint("Minik Terminal\n> ");
		this->has_client = true;
		return true;
	}

#endif
	if (Serial.available() && !this->has_client) {
		this->has_client = true;
		this->set_output(OUT_SERIAL);
		kprint("Minik Terminal\n> ");
		return true;
	}
	if (KGlobals.get_out_device() == OUT_SERIAL && !Serial) {
		this->has_client = false;
	}
	return this->has_client;
}

int Term::available() {
	if (!this->has_client) {
		return 0;
	}
	if (KGlobals.get_out_device() == OUT_WIFI) {
#if defined(BOARD_ESP32) && defined(WITH_WIFI)
		return KGlobals.terminal_client.available();
#endif
	}
	if (KGlobals.get_out_device() == OUT_SERIAL) {
		return Serial.available();
	}
	return 0;
}
