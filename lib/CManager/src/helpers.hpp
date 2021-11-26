#ifndef _helpers_h
#define _helpers_h

#include <Arduino.h>

// strtok should do fine but I need to keep "..." intact
uint extract_size(const char *text, char delimiter, uint index);
char *extract(const char *text, char delimiter, uint index, uint size);
uint argc(const char *text, char delimiter);
uint arg_type(const char *arg);
char *dtoc(double d);
double ctod(char *data);
void ltrim(char *src);
bool is_data_type(int type);

#endif
