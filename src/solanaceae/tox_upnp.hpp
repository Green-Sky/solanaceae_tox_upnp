#pragma once

#include <cstdint>
#include <atomic>
#include <thread>

#include <solanaceae/toxcore/tox_interface.hpp>

class ToxUPnP {
	uint16_t _local_port {33445};

	std::atomic_bool _quit {false};
	std::thread _thread;

	public:
		ToxUPnP(ToxI& tox);
		~ToxUPnP(void);
};

