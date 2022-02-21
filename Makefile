# targets

.PHONY: default
default: help ;

UNAME := $(shell uname -s)

mega:
	@platformio run -t upload -e atmega

esp32:
	@platformio run -t upload -e main

console:
	@platformio device monitor

build:
	@platformio run

clean:
	@platformio run --target clean

build_test:
	@mkdir reports -p
	@echo "Bluetooth test"
	@echo "BLUETOOTH" > config.txt
	@echo "BLUETOOTH_SERIAL" >> config.txt
	@echo 'BLUETOOTH_SERIAL_NAME="MyMinikDevice"' >> config.txt
	@platformio run > reports/bt_test.txt
	@echo "Wifi test"
	@echo "WIFI" > config.txt
	@echo 'WIFI_MODE="AP"' >> config.txt
	@echo 'WIFI_SSID="MinikESP32"' >> config.txt
	@echo 'WIFI_PASSWORD="MyWifiPassword"' >> config.txt
	@platformio run > reports/wifi_test.txt
	@echo "Display test"
	@echo "DISPLAY_SSD1306" > config.txt
	@platformio run > reports/display_test.txt
	@echo "SDCard test"
	@echo "SDCARD" > config.txt
	@platformio run > reports/sdcard-test.txt

help:
	@echo "Make Minik"
	@echo "make mega          # Upload to ATMega"
	@echo "make esp32         # Upload to ESP32"
	@echo "make clean         # Clean project"
	@echo "make console       # Serial console"
	@echo "make build         # Build project"
