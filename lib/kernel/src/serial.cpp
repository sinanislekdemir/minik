#include "serial.hpp"

bool breaker(char c) { return c == '\n'; }
bool ignore(char c) { return (c < 32); }

int serial_getline(unsigned int buffer_size, char *back) {
	char c = 0;
	unsigned int cursor = 0;
	while (strlen(back) < buffer_size) {
		if (!Serial.available()) {
			continue;
		}
		c = Serial.read();
		if (breaker(c)) {
			break;
		}
		if (ignore(c)) {
			continue;
		}
		Serial.print(c);
		back[cursor++] = c;
	}

	Serial.println("");
	return cursor;
}

char *serial_get_multiline(unsigned int buffer_size) {
	if (!Serial) {
		return NULL;
	}

	char *result = NULL;
	int total_size = 0;

	while (true) {
		char *buffer = (char *)malloc(buffer_size);
		memset(buffer, 0, buffer_size);

		int bytes_received = serial_getline(buffer_size, buffer);

		if (strcmp(buffer, ".") == 0) {
			free(buffer);
			break;
		}

		if (strlen(buffer) < 2) {
			free(buffer);
			continue;
		}

		total_size += bytes_received + 1; // bytes_received + \n

		char *temp = (char *)malloc(total_size + 1); // total_string + \0
		memset(temp, 0, total_size + 1);

		if (result == NULL) {
			result = (char *)malloc(1);
			sprintf(temp, "%s\n", buffer);
		} else {
			sprintf(temp, "%s%s\n", result, buffer);
		}

		char *swap = result;
		result = temp;
		free(swap);
		free(buffer);
	}
	return result;
}
