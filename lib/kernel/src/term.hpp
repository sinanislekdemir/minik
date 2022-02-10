#ifndef _kernel_term_hpp
#define _kernel_term_hpp

#define OUT_SERIAL 0
#define OUT_BT 1
#define OUT_WIFI 2
#define BUFFER_SIZE 128
#define MODE_TERM 0
#define MODE_PROGRAM 1

#include "daemon.hpp"
#include <Arduino.h>

class Term : public daemon {
      private:
	uint8_t out_device;
	unsigned long baud_rate;
	int port;
	bool has_client;
	bool _is_in_readline;
	char *io_buffer;
	char *program_buffer;
	int program_buffer_size;
	bool programming_mode;
	int io_cursor;

	int allocate_program_buffer();
	void append_program_buffer();
	int kprint(char *data);
	int readline();
	int receive_source();
#ifdef BOARD_ESP32
#ifdef WITH_WIFI
	void start_server();
#endif
#endif

      public:
	Term();
	~Term();
	int process();
	void set_baud_rate(unsigned long rate);
	void set_output(uint8_t out);
	void set_port(int port);
	// wait for client to connect;
	bool wait_for_client();
	int available();
};

#endif