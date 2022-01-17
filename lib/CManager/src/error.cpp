#include "error.hpp"
#include "memory.hpp"
#include <Arduino.h>

error *raise(const char *msg, unsigned int pid, unsigned int code) {
	error *result = (error *)malloc(sizeof(error));
	result->message = (char *)malloc(strlen(msg) + 1);
	memset(result->message, 0, strlen(msg) + 1);
	strcpy(result->message, msg);
	result->pid = pid;
	result->code = code;
	error_msg(msg, pid);
	return result;
}
