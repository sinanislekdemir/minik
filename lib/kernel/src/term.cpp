#include "term.hpp"

#include "memory.hpp"
#include "program.hpp"
#include "status.hpp"
#include "tasks.hpp"

extern int kernel_next_pid;
extern char _memory_area[MAX_MEM];
extern program programs[MAX_PROGRAM_COUNT];

Term main_term;

#ifdef BOARD_ESP32
#ifdef WITH_WIFI
#include <WiFi.h>
WiFiServer s;
WiFiClient terminal_client;
#endif
#endif

#ifdef BOARD_ESP32
#ifdef WITH_BLUETOOTH_SERIAL
#include <BluetoothSerial.h>
extern BluetoothSerial SerialBT;
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

void to_hex(char *buf, char *str) {
	char *pin = buf;
	const char *hex = "0123456789ABCDEF";
	char *pout = str;
	int i = 0;
	for (; i < sizeof(buf) - 1; ++i) {
		*pout++ = hex[(*pin >> 4) & 0xF];
		*pout++ = hex[(*pin++) & 0xF];
		*pout++ = ':';
	}
	*pout++ = hex[(*pin >> 4) & 0xF];
	*pout++ = hex[(*pin) & 0xF];
	*pout = 0;
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
void Term::start_server() { s.begin(this->port); }
#endif
#endif

void Term::set_output(uint8_t out) { this->out_device = out; }

void Term::set_port(int port) { this->port = port; }

int Term::kprint(char *data) {
	if (this->out_device == OUT_SERIAL) {
		Serial.print((const char *)data);
		return 0;
	}
	if (this->out_device == OUT_BT) {
#ifdef BOARD_ESP32
#ifdef WITH_BLUETOOTH_SERIAL
		SerialBT.write((const uint8_t *)data, strlen(data));
#endif
#endif
		return 0;
	}

	if (this->out_device == OUT_WIFI) {
#ifdef BOARD_ESP32
#ifdef WITH_WIFI
		if (terminal_client.connected())
			terminal_client.write((const char *)data, strlen(data));
#endif
#endif
	}

	return 0;
}

int Term::readline() {
	if (this->out_device == OUT_SERIAL) {
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
	if (this->out_device == OUT_BT) {
#ifdef BOARD_ESP32
#ifdef WITH_BLUETOOTH_SERIAL
		SerialBT.readBytesUntil('\n', this->io_buffer, MAX_LINE_LENGTH);
#endif
#endif
		return 1;
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
					programs[i].compiling = false;
					programs[i].exit_code = RUNNING;
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
		kprint((char *)"Free ram size: ");
		kprint(tmp);
		kprint((char *)"\n");
	}
	if (strcmp(this->io_buffer, "memdump") == 0) {
		char chunk[32] = {0};
		char out[100] = {0};
		for (unsigned int i = 0; i < MAX_MEM; i += 32) {
			read_area_str(i, 32, chunk);
			to_hex(chunk, out);
			kprint(out);
			kprint("\n");
			memset(out, 0, 100);
			memset(chunk, 0, 32);
		}
	}
	if (strcmp(this->io_buffer, "program") == 0) {
		kprint((char *)"Programming mode activated\n");
		kprint((char *)"End programming with a dot (.) as a single line\n");
		kprint((char *)"ready to receive\n");
		this->programming_mode = true;
		this->tmp_pid = kernel_next_pid++;
		for (unsigned int i = 0; i < MAX_PROGRAM_COUNT; i++) {
			if (programs[i].pid == 0) {
				programs[i].set_pid(this->tmp_pid);
				programs[i].compiling = true;
			}
		}
		memset(this->io_buffer, 0, MAX_LINE_LENGTH);
		this->io_cursor = 0;
		return 1;
	}
	kprint((char *)"> ");
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
		kprint((char *)"Minik Terminal\n> ");
		this->has_client = true;
		return true;
	}
#endif
#endif
	if (Serial.available() && !this->has_client) {
		this->has_client = true;
		this->set_output(OUT_SERIAL);
		kprint((char *)"Minik Terminal\n> ");
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
	if (this->out_device == OUT_BT) {
#ifdef BOARD_ESP32
#ifdef WITH_BLUETOOTH_SERIAL
		return SerialBT.available();
#endif
#endif
	}
	if (this->out_device == OUT_SERIAL) {
		return Serial.available();
	}
	return 0;
}
