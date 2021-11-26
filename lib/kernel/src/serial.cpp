#include "serial.hpp"

bool ignore(char c) { return (c < 14); }

char *serial_getline(int buffer_size) {
  char *result = (char *)malloc(buffer_size);
  memset(result, 0, buffer_size);

  char c = 0;
  unsigned int cursor = 0;
  while (strlen(result) < buffer_size) {
    if (!Serial.available()) {
      breath();
      continue;
    }
    c = Serial.read();
    if (ignore(c)) {
      break;
    }
    Serial.print(c);
    result[cursor++] = c;
  }
  Serial.println("");
  return result;
}

char *serial_get_multiline(int buffer_size) {
  if (!Serial) {
    return NULL;
  }

  char *result = NULL;
  int total_size = 0;
  while (true) {
    char *buffer = serial_getline(buffer_size + 1);
    buffer[strlen(buffer)] = '\n';

    int read_size = strlen(buffer);
    total_size += read_size;

    if (strcmp(buffer, ".\n") == 0) {
      free(buffer);
      break;
    }

    if (read_size < 2) {
      free(buffer);
      continue;
    }

    if (result == NULL) {
      result = (char *)malloc(read_size);
      memset(result, '\0', read_size);
      strcpy(result, buffer);
      free(buffer);
      continue;
    }

    char *temp = (char *)malloc(total_size + 1);
    memset(temp, '\0', total_size + 1);
    strcpy(temp, result);
    strcpy(temp + strlen(result), buffer);

    result = temp;
    free(buffer);
    breath();
  }
  return result;
}
