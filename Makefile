# targets

.PHONY: default
default: build ;

UNAME := $(shell uname -s)

mega: build
	@pio run -t upload -e atmega

esp32: build
	@pio run -t upload -e main

console:
	@pio device monitor

build:
	@pio run

clean:
	@pio run --target clean

help:
	@echo "Make Minik"
	@echo "make mega          # Upload to ATMega"
	@echo "make esp32         # Upload to ESP32"
	@echo "make clean         # Clean project"
	@echo "make console       # Serial console"
	@echo "make build         # Build project"
