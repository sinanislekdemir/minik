#include "term.hpp"

#include "constants.hpp"
#include "memory.hpp"
#include "program.hpp"
#include "status.hpp"
#include "tasks.hpp"

extern int kernel_next_pid;
extern char _memory_area[MAX_MEM];
extern program programs[MAX_PROGRAM_COUNT];
extern int serial_lock;

Term main_term;

#ifdef BOARD_ESP32
#ifdef WITH_WIFI
#include <WiFi.h>
WiFiServer s = WiFiServer(23);
WiFiClient terminal_client;
#endif
#endif

Term::Term() {
	this->out_device = OUT_SERIAL;
	this->baud_rate = 9600;
	this->port = 23;
	this->has_client = false;

	this->_is_in_readline = false;
	this->io_cursor = 0;

	this->programming_mode = false;
	memset(this->io_buffer, 0, MAX_LINE_LENGTH);
	this->tmp_pid = 0;
	this->io_cursor = 0;
#ifdef BOARD_ESP32
#ifdef WITH_WIFI
	this->set_port(this->port);
	this->start_server();
#endif
#endif
}

Term::~Term() {}

void Term::set_baud_rate(unsigned long rate) {
	this->baud_rate = rate;
	if (this->out_device == OUT_SERIAL) {
		Serial.print("new-baud-rate: ");
		Serial.println(rate);
		Serial.end();
		Serial.begin(rate);
	}
}

#ifdef BOARD_ESP32
#ifdef WITH_WIFI
void Term::start_server() {}
#endif
#endif

void Term::set_output(uint8_t out) { this->out_device = out; }

void Term::set_port(int port) { this->port = port; }

int Term::kprint(const char *data) {
	if (this->out_device == OUT_SERIAL) {
		Serial.print((const char *)data);
	}

	if (this->out_device == OUT_WIFI) {
#ifdef BOARD_ESP32
#ifdef WITH_WIFI
		if (terminal_client.connected())
			terminal_client.write((const char *)data, strlen(data));
#endif
#endif
	}
#ifdef BOARD_ESP32
	vTaskDelay(1);
#endif
	return 0;
}

int Term::readline() {
	if (this->out_device == OUT_SERIAL) {
		if (serial_lock > 0) {
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
	if (this->out_device == OUT_WIFI) {
#ifdef BOARD_ESP32
#ifdef WITH_WIFI
		terminal_client.readBytesUntil('\n', this->io_buffer, MAX_LINE_LENGTH);
		return 1;
#endif
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
		if (strcmp(this->io_buffer, ".") == 0) {
			for (unsigned int i = 0; i < MAX_PROGRAM_COUNT; i++) {
				if (programs[i].pid == this->tmp_pid) {
					Serial.println("Program compiled");
					programs[i].status_code = PROGRAM_RUNNING;
					break;
				}
			}
			this->programming_mode = false;
			memset(this->io_buffer, 0, MAX_LINE_LENGTH);
			this->io_cursor = 0;
			return 1;
		}
		this->append_program_buffer();
		memset(this->io_buffer, 0, MAX_LINE_LENGTH);
		this->io_cursor = 0;
		return 1;
	}
	if (strcmp(this->io_buffer, "free") == 0) {
		char tmp[16] = {0};
		int ram = free_ram();
		itoa(ram, tmp, 10);
		kprint("Free ram size: ");
		kprint(tmp);
		kprint("\n");
	}
	if (strcmp(this->io_buffer, "memdump") == 0) {
		char tmp[8] = {0};
		for (unsigned int i = 0; i < MAX_MEM; i++) {
			if (i % 16 == 0 && i > 0) {
				kprint("\n");
			}
			char c = read_area_char(i);
			itoa(c, tmp, HEX);
			if (strlen(tmp) > 2) {
				memset(tmp + 2, 0, 6);
			}
			if (strlen(tmp) == 1) {
				kprint(" ");
			}

			kprint(tmp);
			kprint(" ");

			memset(tmp, 0, 8);
		}
		kprint("\n");
	}
	if (strcmp(this->io_buffer, "program") == 0) {
		kprint("Programming mode activated\n");
		kprint("End programming with a dot (.) as a single line\n");
		kprint("ready to receive\n");
		this->programming_mode = true;
		this->tmp_pid = kernel_next_pid++;
		for (unsigned int i = 0; i < MAX_PROGRAM_COUNT; i++) {
			if (programs[i].pid == 0) {
				programs[i].set_pid(this->tmp_pid);
				programs[i].status_code = PROGRAM_COMPILING;
				programs[i].start_time = 0;
				programs[i].end_time = 0;
				Serial.print("Compiling PID:");
				Serial.println(this->tmp_pid);
				break;
			}
		}
		memset(this->io_buffer, 0, MAX_LINE_LENGTH);
		this->io_cursor = 0;
		return 1;
	}
	if (strcmp(this->io_buffer, "ps") == 0) {
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
	if (strncmp(this->io_buffer, "kill ", 5) == 0) {
		char tmp[32] = {0};
		strncpy(tmp, this->io_buffer + 5, strlen(this->io_buffer) - 5);
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
	kprint("> ");
	memset(this->io_buffer, 0, MAX_LINE_LENGTH);
	this->io_cursor = 0;
	return 1;
}

bool Term::wait_for_client() {
#ifdef BOARD_ESP32
#ifdef WITH_WIFI
	if (terminal_client.connected()) {
		return true;
	}
	if (s && s.hasClient()) {
		terminal_client = s.available();
		this->set_port(23);
		this->set_output(OUT_WIFI);
		kprint("Minik Terminal\n> ");
		this->has_client = true;
		return true;
	}
#endif
#endif
	if (Serial.available() && !this->has_client) {
		this->has_client = true;
		this->set_output(OUT_SERIAL);
		kprint("Minik Terminal\n> ");
		return true;
	}
	if (this->out_device == OUT_SERIAL && !Serial) {
		this->has_client = false;
	}
	return this->has_client;
}

int Term::available() {
	if (!this->has_client) {
		return false;
	}
	if (this->out_device == OUT_WIFI) {
#ifdef BOARD_ESP32
#ifdef WITH_WIFI
		return terminal_client.available();
#endif
#endif
	}
	if (this->out_device == OUT_SERIAL) {
		return Serial.available();
	}
	return 0;
}
