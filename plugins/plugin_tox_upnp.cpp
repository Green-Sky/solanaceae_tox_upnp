#include <solanaceae/plugin/solana_plugin_v1.h>

#include <solanaceae/tox_upnp.hpp>

#include <memory>
#include <limits>
#include <iostream>

static std::unique_ptr<ToxUPnP> g_tox_upnp = nullptr;

constexpr const char* plugin_name = "ToxUPnP";

extern "C" {

SOLANA_PLUGIN_EXPORT const char* solana_plugin_get_name(void) {
	return plugin_name;
}

SOLANA_PLUGIN_EXPORT uint32_t solana_plugin_get_version(void) {
	return SOLANA_PLUGIN_VERSION;
}

SOLANA_PLUGIN_EXPORT uint32_t solana_plugin_start(struct SolanaAPI* solana_api) {
	std::cout << "PLUGIN " << plugin_name << " START()\n";

	if (solana_api == nullptr) {
		return 1;
	}

	try {
		// TODO: toxI
		auto* tox_i = PLUG_RESOLVE_INSTANCE(ToxI);

		// static store, could be anywhere tho
		// construct with fetched dependencies
		g_tox_upnp = std::make_unique<ToxUPnP>(*tox_i);

		// register types
		PLUG_PROVIDE_INSTANCE(ToxUPnP, plugin_name, g_tox_upnp.get());
	} catch (const ResolveException& e) {
		std::cerr << "PLUGIN " << plugin_name << " " << e.what << "\n";
		return 2;
	}

	return 0;
}

SOLANA_PLUGIN_EXPORT void solana_plugin_stop(void) {
	std::cout << "PLUGIN " << plugin_name << " STOP()\n";

	g_tox_upnp.reset();
}

SOLANA_PLUGIN_EXPORT float solana_plugin_tick(float) {
	return std::numeric_limits<float>::max();
}

} // extern C

