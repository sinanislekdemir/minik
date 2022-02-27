#include "drivers.hpp"

#include "bluetooth.hpp"
#include "eeprom.hpp"
#include "wifi.hpp"
#include <Arduino.h>

int init_drivers() {
	Serial.println("Initialize devices");
	// Call bluetooth drivers
	int bt = init_bt_serial();
	int wifi = init_wifi();
	int eeprom = init_eeprom();
	Serial.print("Bluetooth status: ");
	Serial.println(bt);
	Serial.print("Wifi status: ");
	Serial.println(wifi);
	Serial.print("Eeprom status: ");
	Serial.println(eeprom);
	return 0;
}
