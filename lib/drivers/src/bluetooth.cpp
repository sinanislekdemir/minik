#include "bluetooth.hpp"
#include <Arduino.h>

#ifdef BOARD_ESP32
#ifdef WITH_BLUETOOTH_SERIAL
#include <BluetoothSerial.h>
BluetoothSerial SerialBT;
#endif
#endif

int init_bt_serial() {
#ifdef BOARD_ESP32
#ifdef WITH_BLUETOOTH_SERIAL
	bool start = SerialBT.begin(String(BLUETOOTH_NAME));
	return start ? 1 : 0;
#endif
#endif
	return 0;
}

#ifdef BOARD_ESP32

#ifdef WITH_BLUETOOTH_SERIAL

int bluetooth_serial(program *_p) {
	unsigned int pid = _p->pid;
	int cmd_var = int(read_area_double(BT_SERIAL_ADDRESS));
	if (cmd_var == 0) {
		error_msg("BTSERIAL_CMD not defined", pid);
		return -1;
	}
	if (cmd_var == 1) {
		int data_available = SerialBT.available();
		free_area(BT_DATA_ADDRESS, sizeof(double));
		write_area(BT_DATA_ADDRESS, double(data_available));
		return 0;
	}
	if (cmd_var == 2) {
		char b = SerialBT.read();
		free_area(BT_DATA_ADDRESS, 2);
		write_area(BT_DATA_ADDRESS, b);
		return 0;
	}
	if (cmd_var == 3) {
		char buffer[MAX_LINE_LENGTH] = {0};
		SerialBT.readBytesUntil('\n', buffer, 128);
		free_area(BT_DATA_ADDRESS, MAX_LINE_LENGTH);
		write_area(BT_DATA_ADDRESS, buffer, MAX_LINE_LENGTH);
		return 0;
	}
	if (cmd_var == 4) {
		char buffer[MAX_LINE_LENGTH] = {0};
		read_area_str(BT_DATA_ADDRESS, MAX_LINE_LENGTH, buffer);
		SerialBT.write((const uint8_t *)buffer, strlen(buffer));
		return 0;
	}
	if (cmd_var == 5) {
		char buffer[MAX_LINE_LENGTH] = {0};
		read_area_str(BT_DATA_ADDRESS, MAX_LINE_LENGTH, buffer);
		SerialBT.connect(String(buffer));
		return 0;
	}
	if (cmd_var == 6) {
		double connected = 0;
		if (SerialBT.connected()) {
			connected = 1;
		}
		free_area(BT_DATA_ADDRESS, sizeof(double));
		write_area(BT_DATA_ADDRESS, connected);
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
	int cmd_var = int(read_area_double(BT_SERIAL_ADDRESS));
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
		free_area(BT_DATA_ADDRESS, sizeof(double) + 1);
		write_area(BT_DATA_ADDRESS, double(foundDevices.getCount()));
		return 0;
	}
	if (cmd_var == 3) {
		if (_btScanner == NULL) {
			error_msg("No active scan", pid);
			return -1;
		}
		int ble_index = int(read_area_double(BT_DATA_ADDRESS));
		BLEAdvertisedDevice device = foundDevices.getDevice(uint32_t(ble_index));
		char temp[MAX_LINE_LENGTH] = {0};
		if (device.getName().length() > 0) {
			strcpy(temp, device.getName().c_str());
		} else {
			strcpy(temp, device.getAddress().toString().c_str());
		}
		free_area(BT_DATA_ADDRESS, MAX_LINE_LENGTH + 1);
		write_area(BT_DATA_ADDRESS, temp, MAX_LINE_LENGTH);
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
