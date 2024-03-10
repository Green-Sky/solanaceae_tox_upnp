cmake_minimum_required(VERSION 3.9...3.24 FATAL_ERROR)

project(solanaceae)

#########################################

add_library(solanaceae_tox_upnp
	./solanaceae/tox_upnp.hpp
	./solanaceae/tox_upnp.cpp
)

target_include_directories(solanaceae_tox_upnp PUBLIC .)
target_compile_features(solanaceae_tox_upnp PUBLIC cxx_std_17)
target_link_libraries(solanaceae_tox_upnp PUBLIC
	miniupnpc::miniupnpc
	solanaceae_toxcore_interface
)

