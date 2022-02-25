#ifndef _helpers_h
#define _helpers_h

// strtok should do fine but I need to keep "..." intact
unsigned int extract_size(const char *text, char delimiter, unsigned int index);
int extract(const char *text, char delimiter, unsigned int index, char *back);
unsigned int argc(const char *text, char delimiter);
unsigned int arg_loc(const char *arg);
unsigned int arg_type(const char *arg);
double ctod(char *data);

char hex2c(char *hex);

void ltrim(char *src);
bool is_data_type(int type);
int pfree();
#endif
