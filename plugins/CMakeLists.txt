cmake_minimum_required(VERSION 3.9...3.24 FATAL_ERROR)

########################################

add_library(plugin_tox_upnp SHARED
	./plugin_tox_upnp.cpp
)
target_compile_features(plugin_tox_upnp PUBLIC cxx_std_17)
target_link_libraries(plugin_tox_upnp PUBLIC
	solanaceae_tox_upnp
	solanaceae_plugin
)

