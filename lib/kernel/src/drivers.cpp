#include "drivers.hpp"

#include "bluetooth.hpp"
#include "wifi.hpp"
#include <Arduino.h>

int init_drivers() {
	Serial.println("Initialize devices");
	// Call bluetooth drivers
	int bt = init_bt_serial();
	int wifi = init_wifi();
	Serial.print("Bluetooth status: ");
	Serial.println(bt);
	Serial.print("Wifi status: ");
	Serial.println(wifi);
	return 0;
}
