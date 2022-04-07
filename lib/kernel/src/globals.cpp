#include "globals.hpp"

KernelGlobals::KernelGlobals() {
	this->kernel_next_pid = 1;
	this->main_terminal = Term();
	this->status_engine = StatusEngine();
	this->serial_lock = -1;
	this->out_device = 0;
}

int KernelGlobals::get_next_pid() {
	this->kernel_next_pid++;
	return this->kernel_next_pid;
}

bool KernelGlobals::lock_acquired(int pid) { return this->serial_lock == pid; }
void KernelGlobals::release_serial_lock() { this->serial_lock = -1; }
int KernelGlobals::acquire_serial_lock(int pid) {
	if (this->serial_lock == 0) {
		this->serial_lock = pid;
		return pid;
	}
	while (this->serial_lock != -1) {
#ifdef BOARD_ESP32
		vTaskDelay(10);
#endif
	}
	this->serial_lock = pid;
	return pid;
}

int KernelGlobals::get_out_device() { return this->out_device; }

void KernelGlobals::set_out_device(int device) { this->out_device = device; }
