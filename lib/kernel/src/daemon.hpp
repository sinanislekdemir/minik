#ifndef _kernel_daemon_hpp
#define _kernel_daemon_hpp

class daemon {
	// NOTE: Avoid using virtual functions.
	//       They are mapped in runtime. Has an overhead;
      public:
	virtual int process();
};

#endif
