#include "wifi.hpp"
#include "helpers.hpp"

#ifdef BOARD_ESP32
#ifdef WITH_WIFI
#include <Arduino.h>
#include <WiFi.h>

n_server *root_server = NULL;

int wifi(program *_p) {
	unsigned int pid = _p->pid;
	int cmd_var = find_number("WIFI_CMD", pid);
	if (cmd_var == 0) {
		error_msg("WIFI_CMD not defined", pid);
		return -1;
	}

	if (cmd_var == 1) {
		int mode = find_number("WIFI_MODE", pid);
		if (mode == 0) {
			error_msg("WIFI_MODE not defined", pid);
			return -1;
		}
		wifi_mode_t m = static_cast<wifi_mode_t>(mode);
		WiFi.mode(m);
		return 0;
	}
	if (cmd_var == 2) {
		char *ssid = find_string("WIFI_SSID", pid);
		char *passwd = find_string("WIFI_PASSWORD", pid);
		if (ssid == NULL || passwd == NULL) {
			error_msg("WIFI_SSID or WIFI_PASSWORD not defined", pid);
			return -1;
		}
		WiFi.begin(ssid, passwd);
		return 0;
	}
	if (cmd_var == 3) {
		int status = WiFi.status();
		new_number((char *)"WIFI_STATUS", double(status), pid);
		return 0;
	}
	if (cmd_var == 4) {
		char *ssid = find_string("WIFI_SSID", pid);
		char *passwd = find_string("WIFI_PASSWORD", pid);
		int channel = 3;
		int ch = find_number("WIFI_CHANNEL", pid);
		if (ch != 0) {
			channel = ch;
		}
		int ssid_hidden = 0;
		int shd = find_number("WIFI_SSID_HIDDEN", pid);

		if (shd != 0) {
			ssid_hidden = shd;
		}

		int max_conn = 4;
		int mc = find_number("WIFI_MAX_CONNECTION", pid);

		if (mc != 0) {
			max_conn = mc;
		}
		if (ssid == NULL || passwd == NULL) {
			error_msg("WIFI_SSID or WIFI_PASSWORD not defined", pid);
			return -1;
		}
		WiFi.softAP(ssid, passwd, channel, ssid_hidden, max_conn);
		return 0;
	}
	if (cmd_var == 5) {
		int n = WiFi.scanNetworks();
		new_number((char *)"WIFI_NUM_NETWORKS", double(n), pid);
		return 0;
	}
	if (cmd_var == 6) {
		int windex = find_number("WIFI_INDEX", pid);
		uint8_t i = uint8_t(windex);
		int n = WiFi.encryptionType(i);
		new_number((char *)"WIFI_ENCRYPTION_TYPE", double(n), pid);
		return 0;
	}
	if (cmd_var == 7) {
		int windex = find_number("WIFI_INDEX", pid);
		uint8_t i = uint8_t(windex);
		String s = WiFi.SSID(i);
		int l = s.length() + 1;
		char *sid = (char *)malloc(l);
		memset(sid, 0, l);
		s.toCharArray(sid, l - 1);
		new_string((char *)"WIFI_SSID", sid, l, pid);
		free(sid);
		return 0;
	}
	if (cmd_var == 8) {
		int windex = find_number("WIFI_INDEX", pid);
		uint8_t i = uint8_t(windex);
		int strength = WiFi.RSSI(i);
		new_number((char *)"WIFI_RSSI", double(strength), pid);
		return 0;
	}
	if (cmd_var == 9) {
		IPAddress ip = WiFi.localIP();
		String ips = ip.toString();
		char *ip_c = (char *)malloc(16);
		memset(ip_c, 0, 16);
		strcpy(ip_c, ips.c_str());
		new_string((char *)"WIFI_LOCALIP", ip_c, 16, pid);
		free(ip_c);
		return 0;
	}
	if (cmd_var == 10) {
		bool success = WiFi.reconnect();
		if (success) {
			new_number((char *)"WIFI_RECONNECT", 1.0, pid);
		} else {
			new_number((char *)"WIFI_RECONNECT", 0.0, pid);
		}
		return 0;
	}
	if (cmd_var == 11) {
		char *hostname = find_string("WIFI_HOSTNAME", pid);
		if (hostname == NULL) {
			error_msg("WIFI_HOSTNAME not defined", pid);
			return -1;
		}
		WiFi.setHostname(hostname);
		return 0;
	}
	if (cmd_var == 12) {
		IPAddress ip = WiFi.softAPIP();
		String ips = ip.toString();
		char *ip_c = (char *)malloc(16);
		memset(ip_c, 0, 16);
		strcpy(ip_c, ips.c_str());
		new_string((char *)"WIFI_SOFTAPIP", ip_c, 16, pid);
		free(ip_c);
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

n_server *_get_server(unsigned int pid) {
	n_server *head = root_server;
	int server_id = find_number("WIFI_SERVER_ID", pid);
	while (head != NULL) {
		if (head->id == server_id && head->pid == pid) {
			return head;
		}
		head = head->next;
	}
	return NULL;
}

unsigned int _get_next_id(unsigned int pid) {
	n_server *head = root_server;
	unsigned int result = 0;
	while (head != NULL) {
		result = head->id;
		if (head->next == NULL) {
			return result + 1;
		}
		head = head->next;
	}
	return result;
}

int server(program *_p) {
	unsigned int pid = _p->pid;
	int cmd_var = find_number("WIFI_SERVER_CMD", pid);
	if (cmd_var == 0) {
		error_msg("WIFI_SERVER_CMD not defined", pid);
		return -1;
	}

	if (cmd_var == 1) {
		int port = find_number("WIFI_SERVER_PORT", pid);
		if (port == 0) {
			error_msg("WIFI_SERVER_PORT not defined", pid);
			return -1;
		}
		uint16_t iport = uint16_t(port);
		n_server *s = new n_server;
		s->server = new WiFiServer(iport);
		s->server->begin();
		s->next = NULL;
		s->pid = pid;
		s->port = iport;
		s->id = _get_next_id(pid);
		new_number((char *)"WIFI_SERVER_ID", double(s->id), pid);
		if (root_server == NULL) {
			root_server = s;
		} else {
			n_server *head = root_server;
			while (head != NULL) {
				if (head->next == NULL) {
					head->next = s;
					break;
				}
				head = head->next;
			}
		}
		return 0;
	}
	if (cmd_var == 2) {
		n_server *s = _get_server(pid);
		if (s == NULL) {
			return -1;
		}
		if (!s->server->hasClient()) {
			new_number((char *)"WIFI_CLIENT", 0.0, pid);
			return 0;
		}
		s->client = s->server->available();
		if (s->client) {
			new_number((char *)"WIFI_CLIENT", 1.0, pid);
		} else {
			new_number((char *)"WIFI_CLIENT", 0.0, pid);
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
		new_string((char *)"WIFI_CLIENT_READ", cm, 1, pid);
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
		new_string((char *)"WIFI_CLIENT_READLINE", cm, st.length() + 1, pid);
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
