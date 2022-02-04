#ifndef _kernel_term_hpp
#define _kernel_term_hpp

#define OUT_SERIAL 0
#define OUT_BT 1
#define OUT_WIFI 2

#include "daemon.hpp"
#include <Arduino.h>

class Term : public daemon {
      private:
	uint8_t out_device;
	unsigned long baud_rate;
	int port;
	bool has_client;

	int kprint(char *data);
	int readline(char *buffer, size_t length);

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
