#ifndef _drivers_wifi_hpp
#define _drivers_wifi_hpp
#define MAX_SERVERS 4

#ifdef BOARD_ESP32
#ifdef WITH_WIFI

#define WIFI_CMD_ADDRESS 10
#define WIFI_DATA_ADDRESS 16

#ifndef WIFI_MODE
#define WIFI_MODE "AP"
#endif

#ifndef WIFI_SSID
#define WIFI_SSID "MinikESP32"
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "MyWifiPassword"
#endif

#include "program.hpp"
#include <WiFi.h>

int wifi(program *_p);
int server(program *_p);
void print_vars();

#endif
#endif

int init_wifi();
#endif
