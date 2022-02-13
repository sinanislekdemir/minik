#include "wifi.hpp"
#include "helpers.hpp"

#ifdef BOARD_ESP32
#ifdef WITH_WIFI
#include <Arduino.h>
#include <WiFi.h>

char _local_ip[16] = {0};

WiFiServer _server[4];
WiFiClient _client[4];
uint16_t _ports[4] = {0};

#endif
#endif

int init_wifi() {
#ifdef BOARD_ESP32
#ifdef WITH_WIFI
	Serial.printf("[%s] - [%s] - [%s]\n", XSTR(WIFI_SSID), XSTR(WIFI_PASSWORD), XSTR(WIFI_MODE));
	if (strcmp(XSTR(WIFI_MODE), "AP") == 0) {
		WiFi.mode(WIFI_AP);
		WiFi.softAP(XSTR(WIFI_SSID), XSTR(WIFI_PASSWORD));
		IPAddress ip = WiFi.softAPIP();
		String ips = ip.toString();
		memset(_local_ip, 0, 16);
		strcpy(_local_ip, ips.c_str());
		Serial.printf("  IP: [%s]\n", _local_ip);
		return 1;
	}
	if (strcmp(XSTR(WIFI_MODE), "STA") == 0) {
		WiFi.mode(WIFI_STA);
		WiFi.begin(XSTR(WIFI_SSID), XSTR(WIFI_PASSWORD));
		int timeout = 0;
		while (!WiFi.isConnected()) {
			delay(1000);
			timeout++;
			if (timeout >= 20) {
				Serial.println("WIFI Timeout");
				return 0;
			}
		}
		IPAddress ip = WiFi.localIP();
		String ips = ip.toString();
		memset(_local_ip, 0, 16);
		strcpy(_local_ip, ips.c_str());
		Serial.printf("  IP: [%s]\n", _local_ip);
		return 1;
	}

#endif
#endif
	return 0;
}

#ifdef BOARD_ESP32
#ifdef WITH_WIFI

int wifi(program *_p) {
	unsigned int pid = _p->pid;
	int cmd_var = int(read_area_double(WIFI_CMD_ADDRESS));
	if (cmd_var == 0) {
		error_msg("WIFI_CMD not defined", pid);
		return -1;
	}

	if (cmd_var == 1) {
		int status = WiFi.status();
		write_area(WIFI_DATA_ADDRESS, double(status));
		return 0;
	}
	if (cmd_var == 2) {
		int n = WiFi.scanNetworks();
		write_area(WIFI_DATA_ADDRESS, double(n));
		return 0;
	}
	if (cmd_var == 3) {
		int windex = int(read_area_double(WIFI_DATA_ADDRESS));
		uint8_t i = uint8_t(windex);
		int n = WiFi.encryptionType(i);
		write_area(WIFI_DATA_ADDRESS, double(n));
		return 0;
	}
	if (cmd_var == 4) {
		int windex = int(read_area_double(WIFI_DATA_ADDRESS));
		uint8_t i = uint8_t(windex);
		String s = WiFi.SSID(i);
		int l = s.length() + 1;
		char sid[MAX_LINE_LENGTH]= {0};
		s.toCharArray(sid, l - 1);
		free_area(WIFI_DATA_ADDRESS, MAX_LINE_LENGTH);
		write_area(WIFI_DATA_ADDRESS, sid, MAX_LINE_LENGTH);
		return 0;
	}
	if (cmd_var == 5) {
		int windex = int(read_area_double(WIFI_DATA_ADDRESS));
		uint8_t i = uint8_t(windex);
		int strength = WiFi.RSSI(i);
		write_area(WIFI_DATA_ADDRESS, double(strength));
		return 0;
	}
	if (cmd_var == 6) {
		IPAddress ip = WiFi.localIP();
		String ips = ip.toString();
		char ip_c[16] = {0};
		strcpy(ip_c, ips.c_str());
		free_area(WIFI_DATA_ADDRESS, 16);
		write_area(WIFI_DATA_ADDRESS, ip_c, 16);
		return 0;
	}
	if (cmd_var == 7) {
		bool success = WiFi.reconnect();
		if (success) {
			write_area(WIFI_DATA_ADDRESS, double(1.0));
		} else {
			write_area(WIFI_DATA_ADDRESS, double(0));
		}
		return 0;
	}
	if (cmd_var == 8) {
		char hostname[MAX_LINE_LENGTH] = {0};
		read_area_str(WIFI_DATA_ADDRESS, MAX_LINE_LENGTH, hostname);
		WiFi.setHostname(hostname);
		return 0;
	}
	if (cmd_var == 9) {
		IPAddress ip = WiFi.softAPIP();
		String ips = ip.toString();
		char ip_c[16] = {0};
		strcpy(ip_c, ips.c_str());
		write_area(WIFI_DATA_ADDRESS, ip_c, 16);
		return 0;
	}
	return 0;
}

void print_vars() {
	Serial.println(WIFI_AUTH_OPEN);
	Serial.println(WIFI_AUTH_WEP);
	Serial.println(WIFI_AUTH_WPA_PSK);
	Serial.println(WIFI_AUTH_WPA2_PSK);
	Serial.println(WIFI_AUTH_WPA_WPA2_PSK);
	Serial.println(WIFI_AUTH_WPA2_ENTERPRISE);
}

int server(program *_p) {
	unsigned int pid = _p->pid;
	int cmd_var = int(read_area_double(WIFI_CMD_ADDRESS));
	if (cmd_var == 0) {
		error_msg("WIFI_SERVER_CMD not defined", pid);
		return -1;
	}

	if (cmd_var == 1) {
		int port = int(read_area_double(WIFI_DATA_ADDRESS));
		if (port == 0) {
			error_msg("WIFI_SERVER_PORT not defined", pid);
			return -1;
		}
		uint16_t iport = uint16_t(port);

		for (char i = 0; i < 4; i++) {
			if (_ports[i] == 0) {
				_ports[i] = iport;
				_server[i] = WiFiServer(iport);
				_server[i].begin();
				write_area(WIFI_DATA_ADDRESS, double(i));
				return 0;
			}
		}
		error_msg("No empty server slots", _p->pid);
		return 0;
	}
	if (cmd_var == 2) {
		int server = int(read_area_double(WIFI_DATA_ADDRESS));
		if (!_server[server].hasClient()) {
			write_area(WIFI_DATA_ADDRESS, double(0));
			return 0;
		}
		_client[server] = _server->available();
		if (_client[server]) {
			write_area(WIFI_DATA_ADDRESS, double(1.0));
		} else {
			write_area(WIFI_DATA_ADDRESS, double(0));
		}
		return 0;
	}
	if (cmd_var == 3) {
		n_server *s = _get_server(pid);
		if (s == NULL) {
			return -1;
		}
		new_number((char *)"WIFI_CLIENT_CONNECTED", double(s->client.connected()), pid);
		return 0;
	}
	if (cmd_var == 4) {
		n_server *s = _get_server(pid);
		if (s == NULL) {
			return -1;
		}
		if (!s->client.connected()) {
			error_msg("Client is not connected", pid);
			return -1;
		}
		new_number((char *)"WIFI_CLIENT_AVAILABLE", double(s->client.available()), pid);
		return 0;
	}
	if (cmd_var == 5) {
		n_server *s = _get_server(pid);
		if (s == NULL) {
			return -1;
		}
		if (!s->client.connected()) {
			error_msg("Client is not connected", pid);
			return -1;
		}
		char *cm = (char *)malloc(1);
		cm[0] = s->client.read();
		new_string((char *)"WIFI_CLIENT_READ", cm, pid);
		free(cm);
		return 0;
	}
	if (cmd_var == 6) {
		n_server *s = _get_server(pid);
		if (s == NULL) {
			return -1;
		}
		if (!s->client.connected()) {
			error_msg("Client is not connected", pid);
			return -1;
		}
		String st = s->client.readStringUntil('\n');
		char *cm = (char *)malloc(st.length() + 1);
		memset(cm, 0, st.length() + 1);
		strcat(cm, st.c_str());
		new_string((char *)"WIFI_CLIENT_READLINE", cm, pid);
		free(cm);
		return 0;
	}
	if (cmd_var == 7) {
		n_server *s = _get_server(pid);
		if (s == NULL) {
			return -1;
		}
		if (!s->client.connected()) {
			error_msg("Client is not connected", pid);
			return -1;
		}
		char *data = find_string("WIFI_CLIENT_PRINT", pid);
		if (data == NULL) {
			error_msg("WIFI_CLIENT_PRINT not defined", pid);
			return -1;
		}
		s->client.print(data);
		return 0;
	}
	if (cmd_var == 8) {
		n_server *s = _get_server(pid);
		if (s == NULL) {
			return -1;
		}
		if (!s->client.connected()) {
			error_msg("Client is not connected", pid);
			return -1;
		}
		char *data = find_string("WIFI_CLIENT_PRINTLN", pid);
		if (data == NULL) {
			error_msg("WIFI_CLIENT_PRINT not defined", pid);
			return -1;
		}
		s->client.println(data);
		return 0;
	}
	if (cmd_var == 9) {
		n_server *s = _get_server(pid);
		if (s == NULL) {
			return -1;
		}
		if (!s->client.connected()) {
			error_msg("Client is not connected", pid);
			return -1;
		}
		s->client.stop();
		return 0;
	}
	if (cmd_var == 10) {
		n_server *s = _get_server(pid);
		if (s == NULL) {
			return -1;
		}
		if (s->client) {
			if (s->client.connected()) {
				s->client.stop();
			}
		}
		free(s->server);
		s->id = 0;
		s->pid = 0;
		s->port = 0;
		return 0;
	}
	return 0;
}

#endif
#endif
