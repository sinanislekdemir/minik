#ifndef _kernel_globals_hpp
#define _kernel_globals_hpp

#include "status.hpp"
#include "term.hpp"

#define OUT_SERIAL 0
#define OUT_BT 1
#define OUT_WIFI 2

#if defined(BOARD_ESP32) && defined(WITH_WIFI)
#include <WiFi.h>
#endif

class KernelGlobals {
      private:
	int kernel_next_pid;
	int serial_lock;
	int out_device;

      public:
	KernelGlobals();
#if defined(BOARD_ESP32) && defined(WITH_WIFI)
	WiFiClient terminal_client;
#endif
	Term main_terminal;
	StatusEngine status_engine;
	int get_next_pid();
	int acquire_serial_lock(int pid);
	void release_serial_lock();
	bool lock_acquired(int pid);
	int get_out_device();
	void set_out_device(int device);
};

#endif
