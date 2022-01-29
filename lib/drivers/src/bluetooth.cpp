#include "bluetooth.hpp"
#include <Arduino.h>

#ifdef BOARD_ESP32

#ifdef WITH_BLUETOOTH_SERIAL
#include <BluetoothSerial.h>
BluetoothSerial SerialBT;

int bluetooth_serial(program *_p) {
	unsigned int pid = _p->pid;
	int cmd_var = find_number("BTSERIAL_CMD", pid);
	if (cmd_var == 0) {
		error_msg("BTSERIAL_CMD not defined", pid);
		return -1;
	}
	if (cmd_var == 1) {
		char *device_name = find_string("BTSERIAL_NAME", pid);
		if (device_name == NULL) {
			error_msg("BTSERIAL_NAME is not defined", pid);
			return -1;
		}
		String localName = String(device_name);
		SerialBT.begin(localName);
		return 0;
	}
	// Server mode
	if (cmd_var == 2) {
		int data_available = SerialBT.available();
		new_number((char *)"BTSERIAL_AVAILABLE", double(data_available), pid);
		return 0;
	}
	if (cmd_var == 3) {
		char b = SerialBT.read();
                new_number((char *)"BTSERIAL_READ", double(b), pid);
		return 0;
	}
	if (cmd_var == 4) {
		char *buffer = (char *)malloc(128);
		memset(buffer, 0, 128);
		SerialBT.readBytesUntil('\n', buffer, 128);
		new_string((char *)"BTSERIAL_READLN", buffer, strlen(buffer) + 1, pid);
		free(buffer);
		return 0;
	}
	if (cmd_var == 5) {
		char *buffer = find_string("BTSERIAL_WRITE", pid);
		SerialBT.write((const uint8_t *)buffer, strlen(buffer));
		return 0;
	}
	if (cmd_var == 6) {
		char *device_name = find_string("BLE_DEVICE_NAME", pid);
		SerialBT.connect(String(device_name));
		return 0;
	}
	if (cmd_var == 7) {
		double connected = 0;
		if (SerialBT.connected()) {
			connected = 1;
		}
		new_number((char *)"BLE_DEVICE_CONNECTED", connected, pid);
		return 0;
	}
	return 0;
}
#endif

#ifdef WITH_BLUETOOTH
#include <BLEAdvertisedDevice.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEUtils.h>

BLEScan *_btScanner = NULL;
BLEScanResults foundDevices;

int scanTime = 5;
int counter = 0;

int bluetooth(program *_p) {
	unsigned int pid = _p->pid;
	int cmd_var = find_number("BLE_CMD", pid);
	if (cmd_var == 1) {
		if (_btScanner != NULL) {
			_btScanner->clearResults();
			free(_btScanner);
		}
		BLEDevice::init("");
		_btScanner = BLEDevice::getScan();
		_btScanner->setActiveScan(true); // more power but faster
		_btScanner->setInterval(100);
		_btScanner->setWindow(99);
		return 0;
	}
	if (cmd_var == 2) {
		if (_btScanner == NULL) {
			error_msg("No active scan", pid);
			return -1;
		}
		foundDevices = _btScanner->start(scanTime, false);
		new_number((char *)"BLE_DEVICE_COUNT", double(foundDevices.getCount()), pid);
		return 0;
	}
	if (cmd_var == 3) {
		if (_btScanner == NULL) {
			error_msg("No active scan", pid);
			return -1;
		}
		int ble_index = find_number("BLE_INDEX", pid);
		BLEAdvertisedDevice device = foundDevices.getDevice(uint32_t(ble_index));
		char *temp = (char *)malloc(128);
		memset(temp, 0, 128);
		if (device.getName().length() > 0) {
			strcpy(temp, device.getName().c_str());
		} else {
			strcpy(temp, device.getAddress().toString().c_str());
		}
		new_string((char *)"BLE_NAME", temp, strlen(temp) + 1, pid);
		free(temp);
		return 0;
	}
	if (cmd_var == 4) {
		_btScanner->clearResults();
		free(_btScanner);
		return 0;
	}
	return 0;
}

#endif

#endif
