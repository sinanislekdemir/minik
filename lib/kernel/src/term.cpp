#include "term.hpp"

#include "memory.hpp"
#include "status.hpp"

extern variable root_variable;

#ifdef BOARD_ESP32
#ifdef WITH_WIFI
#include <WiFi.h>
WiFiServer *s = NULL;
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
	this->has_client = true;
}

Term::~Term() { free(this->line_bufer); }

void Term::set_baud_rate(unsigned long rate) {
	this->baud_rate = rate;
	if (this->out_device == OUT_SERIAL) {
		Serial.print("new-baud-rate: ");
		Serial.println(rate);
		Serial.end();
		Serial.begin(rate);
	}
}

void Term::set_output(uint8_t out) {
	if (this->out_device == OUT_WIFI && out != OUT_WIFI) {
#ifdef WITH_WIFI
		if (s != NULL) {
			s->end();
			free(s);
		}
#endif
	}
	this->out_device = out;
	if (this->out_device == OUT_WIFI) {
#ifdef WITH_WIFI
		s = new WiFiServer(this->port);
		s->begin();
#endif
	}
}

void Term::set_port(int port) { this->port = port; }

int Term::kprint(char *data) {
	if (this->out_device == OUT_SERIAL) {
		Serial.println((const char *)data);
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
		if (s->hasClient())
			s->write((const char *)data, strlen(data));
#endif
#endif
	}

	return 0;
}

int Term::readline(char *buffer, size_t length) {
	if (this->out_device == OUT_SERIAL) {
		Serial.readBytesUntil('\n', buffer, length);
		return 0;
	}
	if (this->out_device == OUT_BT) {
#ifdef BOARD_ESP32
#ifdef WITH_BLUETOOTH_SERIAL
		SerialBT.readBytesUntil('\n', buffer, length);
#endif
#endif
		return 0;
	}
	if (this->out_device == OUT_WIFI) {
#ifdef BOARD_ESP32
#ifdef WITH_WIFI
		terminal_client.readBytesUntil('\n', buffer, length);
		return 0;
#endif
#endif
	}
	return 0;
}

int Term::process() {
	if (!this->wait_for_client()) {
		return 1;
	}
	if (!this->available()) {
		return 1;
	}
	char *buffer = (char *)malloc(128);
	char buf[64];
	memset(buffer, 0, 128);

	readline(buffer, 128);
	if (strcmp(buffer, "free")) {
		int ram = free_ram();
		memset(buffer, 0, 128);
		sprintf(buffer, "Free ram: %d\n", ram);
		free(buffer);
		return 1;
	}
	if (strcmp(buffer, "memdump")) {
		variable *node = &root_variable;
		while (node != NULL) {
			kprint((char *)"Name: ");
			kprint(node->name);
			kprint((char *)"\n  Type: ");
			memset(buf, 0, 64);
			itoa(node->type, buf, 10);
			kprint(buf);
			kprint((char *)"\n  Size: ");
			memset(buf, 0, 64);
			itoa(node->datasize, buf, 10);
			kprint((char *)"\n  PID: ");
			kprint(buf);
			kprint((char *)"\n");
			node = node->next;
		}
		free(buffer);
		return 1;
	}
	free(buffer);
	return 1;
}

bool Term::wait_for_client() {
#ifdef BOARD_ESP32
#ifdef WITH_WIFI
	if (s != NULL && s->hasClient()) {
		terminal_client = s->available();
		this->set_port(23);
		this->set_output(OUT_WIFI);
		kprint((char *)"Minik Terminal");
		this->has_client = true;
		return true;
	}
#endif
#endif
	return false;
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
