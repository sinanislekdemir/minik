#ifndef _kernel_term_hpp
#define _kernel_term_hpp

#define OUT_SERIAL 0
#define OUT_BT 1
#define OUT_WIFI 2

#define MODE_TERM 0
#define MODE_PROGRAM 1
#define TERM_PORT 23

#include "constants.hpp"
#include <Arduino.h>

#ifdef BOARD_ESP32
#ifdef WITH_WIFI
#include <WiFi.h>
#endif
#endif

class Term {
      private:
	unsigned long baud_rate;
	int port;
	bool has_client;
	bool _is_in_readline;
	bool boot_check;
	char io_buffer[MAX_LINE_LENGTH];

	bool programming_mode;
	bool write_eeprom;
	int io_cursor;
	int tmp_pid;

	void append_program_buffer();

	int readline();
	int receive_source();
#ifdef BOARD_ESP32
#ifdef WITH_WIFI
	WiFiServer terminal_server;
#endif
#endif

      public:
	Term();
	~Term();
	int process();
	void set_baud_rate(unsigned long rate);
	void set_output(uint8_t out);
	void set_port(int port);
	void start_server();
	// wait for client to connect;
	bool wait_for_client();
	int available();
};

int kprint(const char *data);

#endif
