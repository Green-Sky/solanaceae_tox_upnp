#pragma once

#include <cstdint>
#include <atomic>
#include <thread>

class ToxUPnP {
	// TODO: ToxI& _t;

	uint16_t _local_port {33445};

	std::atomic_bool _quit {false};
	std::thread _thread;

	public:
		ToxUPnP(void);
		~ToxUPnP(void);
};

