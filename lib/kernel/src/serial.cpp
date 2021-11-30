#include "serial.hpp"

bool breaker(char c) { return c == '\n'; }
bool ignore(char c) { return (c < 32); }

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
    if (breaker(c)) {
      break;
    }
    if (ignore(c)) {
      continue;
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
  int result_size = 0;
  while (true) {
    char *buffer = serial_getline(buffer_size);
    if (strcmp(buffer, ".") == 0) {
      free(buffer);
      break;
    }
    if (strlen(buffer) < 2) {
      free(buffer);
      continue;
    }

    int buffer_len = strlen(buffer) + 1;
    total_size += buffer_len;

    if (result == NULL) {
      result = (char *)malloc(0);
    }
    char *temp = (char *)malloc(total_size + 1);
    memset(temp, 0, total_size + 1);
    sprintf(temp, "%s\n%s", result, buffer);
    char *a = result;
    result = temp;
    free(a);
    free(buffer);
    breath();
  }
  Serial.println(result);
  return result;
}
