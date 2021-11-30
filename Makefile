# targets

.PHONY: default
default: help ;

UNAME := $(shell uname -s)

mega:
	@pio run -t upload -e atmega

esp32:
	@pio run -t upload -e main

console:
	@pio device monitor

build:
	@pio run

help:
	@echo "Make Minik"
	@echo "make mega          # Upload to ATMega"
	@echo "make esp32         # Upload to ESP32"
	@echo "make console       # Serial console"
	@echo "make build         # Build project"
