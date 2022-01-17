#ifndef _error_hpp
#define _error_hpp

struct error {
	char *message;
	unsigned int code;
	unsigned int pid;
};

error *raise(const char *msg, unsigned int pid, unsigned int code);
#endif
