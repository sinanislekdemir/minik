#include "wifi.hpp"
#include "helpers.hpp"

#ifndef BOARD_ESP32
#include <Arduino.h>
#include <WiFi.h>

int wifi(program *_p) {
	unsigned int pid = _p->pid;
	variable *cmd = find_variable("WIFI_CMD", pid);
	if (cmd == NULL) {
		return -1;
	}
	if (cmd->type != TYPE_NUM) {
		return -1;
	}
	int cmd_var = int(ctod(cmd->data));
	if (cmd_var == 1) {
		variable *mode = find_variable("WIFI_MODE", pid);
		if (mode == NULL) {
			return -1;
		}
		wifi_mode_t m = static_cast<wifi_mode_t>(int(ctod(mode->data)));
		WiFi.mode(m);
		return 0;
	}
	if (cmd_var == 2) {
		variable *ssid = find_variable("WIFI_SSID", pid);
		variable *passwd = find_variable("WIFI_PASSWORD", pid);
		if (ssid == NULL || passwd == NULL) {
			return -1;
		}
		WiFi.begin(ssid->data, passwd->data);
		return 0;
	}
	if (cmd_var == 3) {
		int status = WiFi.status();
		new_number((char *)"WIFI_STATUS", double(status), pid);
		return 0;
	}
	if (cmd_var == 4) {
		variable *ssid = find_variable("WIFI_SSID", pid);
		variable *passwd = find_variable("WIFI_PASSWORD", pid);
		int channel = 3;
		variable *ch = find_variable("WIFI_CHANNEL", pid);
		if (ch != NULL) {
			channel = int(ctod(ch->data));
		}
		int ssid_hidden = 0;
		variable *shd = find_variable("WIFI_SSID_HIDDEN", pid);
		if (shd != NULL) {
			ssid_hidden = int(ctod(shd->data));
		}
		int max_conn = 4;
		variable *mc = find_variable("WIFI_MAX_CONNECTION", pid);
		if (mc != NULL) {
			max_conn = int(ctod(mc->data));
		}
		if (ssid == NULL || passwd == NULL) {
			return -1;
		}
		WiFi.softAP(ssid->data, passwd->data, channel, ssid_hidden, max_conn);
		return 0;
	}
	if (cmd_var == 5) {
		int n = WiFi.scanNetworks();
		new_number((char *)"WIFI_NUM_NETWORKS", double(n), pid);
		return 0;
	}
	if (cmd_var == 6) {
		variable *windex = find_variable("WIFI_INDEX", pid);
		if (windex == NULL) {
			return -1;
		}
		uint8_t i = uint8_t(ctod(windex->data));
		int n = WiFi.encryptionType(i);
		new_number((char *)"WIFI_ENCRYPTION_TYPE", double(n), pid);
		return 0;
	}
	if (cmd_var == 7) {
	}
}

void print_vars() {
	Serial.println(WIFI_AUTH_OPEN);
	Serial.println(WIFI_AUTH_WEP);
	Serial.println(WIFI_AUTH_WPA_PSK);
	Serial.println(WIFI_AUTH_WPA2_PSK);
	Serial.println(WIFI_AUTH_WPA_WPA2_PSK);
	Serial.println(WIFI_AUTH_WPA2_ENTERPRISE);
}
#endif
