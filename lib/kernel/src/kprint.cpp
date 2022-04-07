#include "kprint.hpp"
#include "globals.hpp"
#include <Arduino.h>

extern KernelGlobals KGlobals;

int kprint(const char *data) {
	int dev = KGlobals.get_out_device();
	if (dev == OUT_SERIAL) {
		Serial.print(data);
	}

	if (dev == OUT_WIFI) {
#if defined(BOARD_ESP32) && defined(WITH_WIFI)
		if (KGlobals.terminal_client.connected())
			KGlobals.terminal_client.write(data, strlen(data));
#endif
	}
#ifdef BOARD_ESP32
	vTaskDelay(1);
#endif
	return 0;
}

int kprint(double data) {
	char buffer[24] = {0};
	dtostrf(data, 11, 8, buffer);
	return kprint(buffer);
}

int kprint(int data) {
	char buffer[24] = {0};
	itoa(data, buffer, DEC);
	return kprint(data);
}
