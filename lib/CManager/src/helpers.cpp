#include <Arduino.h>

#include "constants.hpp"

extern const char *constants[];

uint extract_size(const char *text, char delimiter, uint index) {
  int index_counter = 0;
  bool quote_block = false;
  uint result = 0;

  for (uint i = 0; i < strlen(text); i++) {
    if (index_counter == index) {
      for (uint j = i; j < strlen(text); j++) {
        if (text[j] == '"') {
          quote_block = !quote_block;
        }
        if (text[j] == delimiter) {
          if (quote_block) {
            result++;
            continue;
          } else {
            return result;
          }
        }
        result++;
      }
      return result;
    }
    if (text[i] == '"') {
      quote_block = !quote_block;
    }
    if (text[i] == delimiter && !quote_block) {
      index_counter++;
    }
  }
  return result;
}

// strtok should do fine but I need to keep "..." intact
char *extract(const char *text, char delimiter, uint index, uint size) {
  // Extract the given parameter from a full string and return *back

  int cursor = 0;
  int index_counter = 0;
  bool quote_block = false;

  for (uint i = 0; i < strlen(text); i++) {
    if (index_counter == index) {
      char *back = (char *)malloc(size);
      memset(back, 0, size);

      for (uint j = i; j < strlen(text); j++) {
        if (text[j] == '"') {
          quote_block = !quote_block;
        }
        if (text[j] == delimiter) {
          if (quote_block) {
            back[cursor++] = text[j];
            continue;
          } else {
            return back;
          }
        }
        back[cursor++] = text[j];
      }
      return back;
    }
    if (text[i] == '"') {
      quote_block = !quote_block;
    }
    if (text[i] == delimiter && !quote_block) {
      index_counter++;
    }
  }
  return NULL;
}

uint argc(const char *text, char delimiter) {
  // return argument count given in a text
  bool string_literal = false;
  uint count = 1;
  for (uint i = 0; i < strlen(text); i++) {
    if (text[i] == '"') {
      string_literal = !string_literal;
    }
    if (!string_literal && text[i] == delimiter) {
      count += 1;
    }
  }
  return count;
}

uint arg_type(const char *arg) {
  // return the argument's type
  if (strlen(arg) == 0) {
    return TYPE_NONE;
  }
  if (arg[0] == '"') {
    return TYPE_STR;
  }
  if (arg[strlen(arg) - 1] == ':') {
    return TYPE_LABEL;
  }
  if (isdigit(arg[0])) {
    return TYPE_NUM;
  }
  for (u_int i = 0; i < CONSTANT_COUNT; i++) {
    if (strcmp(arg, constants[i]) == 0) {
      return TYPE_CONSTANT;
    }
  }
  // skip file check for now.
  // @TODO: Do File type check once we have the
  //        file system drivers in-place.
  return TYPE_VARIABLE;
}

char *dtoc(double d) {
  char *value = (char *)malloc(sizeof(d));
  memcpy(value, &d, sizeof(d));
  return value;
}

double ctod(char *data) {
  double resp = 0;
  memcpy(&resp, data, sizeof(resp));
  return resp;
}

void ltrim(char *src) {
  char *dst;

  /* find position of first non-space character */
  for (dst = src; *src == ' '; src++) {
    ;
  }

  /* nothing to do */
  if (dst == src)
    return;

  /* K&R style strcpy() */
  while ((*dst++ = *src++)) {
    ;
  }

  return;
}

bool is_data_type(int type) {
  return (type == TYPE_NUM || type == TYPE_STR || type == TYPE_FILE ||
          type == TYPE_REGISTER || type == TYPE_CONSTANT ||
          type == TYPE_VARIABLE);
}
