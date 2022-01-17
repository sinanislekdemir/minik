#ifndef _drivers_wifi_hpp
#define _drivers_wifi_hpp
#define MAX_SERVERS 4
#define BOARD_ESP323

#ifdef BOARD_ESP32

#include "program.hpp"
#include <WiFi.h>
struct n_server {
	WiFiServer *server;
	n_server *next;
	unsigned int id;
	uint16_t port;
	unsigned int pid;
	WiFiClient client;
};

int wifi(program *_p);
int server(program *_p);
void print_vars();

#endif

#endif
