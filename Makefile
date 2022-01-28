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

help:
	@echo "Make Minik"
	@echo "make mega          # Upload to ATMega"
	@echo "make esp32         # Upload to ESP32"
	@echo "make clean         # Clean project"
	@echo "make console       # Serial console"
	@echo "make build         # Build project"
