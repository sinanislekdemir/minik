#ifndef _helpers_h
#define _helpers_h

// strtok should do fine but I need to keep "..." intact
unsigned int extract_size(const char *text, char delimiter, unsigned int index);
char *extract(const char *text, char delimiter, unsigned int index, unsigned int size);
unsigned int argc(const char *text, char delimiter);
unsigned int arg_type(const char *arg);
char *dtoc(double d);
double ctod(char *data);
void ltrim(char *src);
bool is_data_type(int type);
int pfree();
#endif
