#ifndef _error_hpp
#define _error_hpp

struct error {
	unsigned int code;
	unsigned int pid;
};

error raise(unsigned int pid, unsigned int code);
#endif
