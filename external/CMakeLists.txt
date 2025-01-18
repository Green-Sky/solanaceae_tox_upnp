cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

include(FetchContent)

if (NOT TARGET solanaceae_plugin)
	FetchContent_Declare(solanaceae_plugin
		GIT_REPOSITORY https://github.com/Green-Sky/solanaceae_plugin.git
		GIT_TAG master
	)
	FetchContent_MakeAvailable(solanaceae_plugin)
endif()

if (NOT TARGET miniupnpc::miniupnpc)
	set(UPNPC_BUILD_STATIC ON CACHE BOOL "" FORCE)
	set(UPNPC_BUILD_SHARED OFF CACHE BOOL "" FORCE)
	set(UPNPC_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	set(UPNPC_BUILD_SAMPLE ON CACHE BOOL "" FORCE)
	set(UPNPC_NO_INSTALL ON CACHE BOOL "" FORCE)
	FetchContent_Declare(miniupnpc
		GIT_REPOSITORY https://github.com/miniupnp/miniupnp.git
		GIT_TAG miniupnpc_2_3_0
		SOURCE_SUBDIR miniupnpc
	)
	FetchContent_MakeAvailable(miniupnpc)
endif()

