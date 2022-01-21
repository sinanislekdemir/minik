#ifndef _kernel_source_hpp
#define _kernel_source_hpp
#define BUFFER_SIZE 128
#define EEPROM_DEFINED 9
#define EEPROM_CLEAR 10

#include "daemon.hpp"
#include "kernel_const.hpp"
#include <Arduino.h>

class SourceEngine : public daemon {
      private:
	bool serial_open;
	int boud_rate;
	char *buffer;
	char *source;
	unsigned int buffer_cursor;
	void append_to_source();
	void create_task();

      public:
	SourceEngine();
	~SourceEngine();
	int process();
};

#endif
