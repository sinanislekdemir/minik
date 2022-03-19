#include "kprint.hpp"

uint8_t out_device;

#ifdef BOARD_ESP32
#ifdef WITH_WIFI
#include <WiFi.h>
extern WiFiClient terminal_client;
#endif
#endif

int kprint(const char *data) {
	if (out_device == 0) {
		Serial.print(data);
	}

	if (out_device == 2) {
#ifdef BOARD_ESP32
#ifdef WITH_WIFI
		if (terminal_client.connected())
			terminal_client.write(data, strlen(data));
#endif
#endif
	}
#ifdef BOARD_ESP32
	vTaskDelay(1);
#endif
	return 0;
}
