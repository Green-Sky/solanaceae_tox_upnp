#include "./tox_upnp.hpp"

#include <miniupnpc.h>
#include <upnpcommands.h>
#include <upnperrors.h>

#include <chrono>

#include <iostream>

ToxUPnP::ToxUPnP(ToxI& tox) {
	// get tox port
	auto [port_opt, _] = tox.toxSelfGetUDPPort();
	if (!port_opt.has_value()) {
		return; // oof
	}
	_local_port = port_opt.value();

	// start upnp thread
	_thread = std::thread([this](void) {
		int seconds_since_last {60*60};
		bool last_mapping_succ {false};
		while (!_quit) {
			if (seconds_since_last < 60*60) {
				std::this_thread::sleep_for(std::chrono::seconds(1));
				seconds_since_last++;
				continue;
			}
			seconds_since_last = 0;
			last_mapping_succ = false;

			// first get available devices
			int error {0};
			std::cerr << "TUPNP: starting search\n";
			auto* devices = upnpDiscover(2000, nullptr, nullptr, UPNP_LOCAL_PORT_ANY, 0, 2, &error);
			if (error != 0 || devices == nullptr) {
				std::cerr << "TUPNP error: no device found\n";
				continue;
			}

			std::cerr << "TUPNP: discovered devices:\n";
			for (auto* d = devices; d != nullptr; d = d->pNext) {
				std::cerr << "  " << d->descURL << " " << d->st << " " << d->usn << "\n";
			}

			UPNPUrls urls;
			IGDdatas data;
			char lanaddr[64] = "unset";
			auto res = UPNP_GetValidIGD(devices, &urls, &data, lanaddr, sizeof(lanaddr));

			if (res < 1) {
				std::cerr << "TUPNP error: no valid connected IGD has been found\n";
				if (res != 0) {
					FreeUPNPUrls(&urls);
					freeUPNPDevlist(devices);
				}
				continue;
			}

			std::cerr << "TUPNP: valid IGD found (" << res << "), local ip: " << lanaddr << "\n";

			const auto port_string = std::to_string(_local_port);
			auto map_ret = UPNP_AddPortMapping(
				urls.controlURL,
				data.first.servicetype,
				port_string.c_str(),
				port_string.c_str(),
				lanaddr,
				"Tomato UPnP Tox UDP port forwarding",
				"UDP",
				nullptr,
				//"900" // lease duration in seconds
				"0"
			);

			if (map_ret != UPNPCOMMAND_SUCCESS) {
				std::cerr << "TUPNP error: adding port mapping failed " << strupnperror(map_ret) << "\n";
				FreeUPNPUrls(&urls);
				freeUPNPDevlist(devices);
				continue;
			}

			char intClient[40] {};
			char intPort[6] {};
			char duration[16] {};
			auto getmap_ret = UPNP_GetSpecificPortMappingEntry(
				urls.controlURL,
				data.first.servicetype,
				port_string.c_str(),
				"UDP",
				nullptr,
				intClient,
				intPort,
				nullptr,
				nullptr,
				duration
			);

			if (getmap_ret != UPNPCOMMAND_SUCCESS) {
				std::cerr << "TUPNP error: getting port mapping failed " << strupnperror(getmap_ret) << "\n";

				// potentially succ ???
				last_mapping_succ = true;

				FreeUPNPUrls(&urls);
				freeUPNPDevlist(devices);
				continue;
			}

			std::cerr << "TUPNP: mapping active external :" << port_string << " is redirected to internal " << intClient << ":" << intPort << " (for " << duration << "s)\n";
			// potentially succ
			last_mapping_succ = true;

			FreeUPNPUrls(&urls);
			freeUPNPDevlist(devices);
		}

		// remove mapping here?
	});
}

ToxUPnP::~ToxUPnP(void) {
	_quit = true;

	if (_thread.joinable()) {
		_thread.join();
	}
}

