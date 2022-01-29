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
	@mkdir reports
	@echo "Bluetooth test"
	@echo "BLUETOOTH" > drivers.txt
	@echo "BLUETOOTH_SERIAL" >> drivers.txt
	@platformio run > reports/bt_test.txt
	@echo "Wifi test"
	@echo "WIFI" > drivers.txt
	@platformio run > reports/wifi_test.txt
	@echo "Display test"
	@echo "DISPLAY_SSD1306" > drivers.txt
	@platformio run > reports/display_test.txt

help:
	@echo "Make Minik"
	@echo "make mega          # Upload to ATMega"
	@echo "make esp32         # Upload to ESP32"
	@echo "make clean         # Clean project"
	@echo "make console       # Serial console"
	@echo "make build         # Build project"
