cmake_minimum_required(VERSION 3.11)

project(game_server CXX)
set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake)
conan_basic_setup(TARGETS)

if (MSVC)
  set(CMAKE_CXX_FLAGS_RELEASE "/MT")
  set(CMAKE_CXX_FLAGS_DEBUG "/MD")
endif ()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(ModelGame STATIC 
src/model.h
src/model.cpp
src/loot_generator.h
src/loot_generator.cpp
src/tagged.h
src/collision_detector.h
src/collision_detector.cpp
src/geom.h)

target_include_directories(ModelGame PUBLIC CONAN_PKG::boost)
target_link_libraries(ModelGame PUBLIC CONAN_PKG::boost Threads::Threads)

add_executable(game_server
	src/main.cpp
	src/http_server.cpp
	src/http_server.h
	src/sdk.h
	src/boost_json.cpp
	src/json_loader.h
	src/json_loader.cpp
	src/request_handler.cpp
	src/request_handler.h
	src/json_utils.h
    src/json_utils.cpp
	src/json_logger.h
	src/json_logger.cpp
	src/application.h
	src/application.cpp
	src/ticker.h
	src/ticker.cpp
	src/extra_data.h
	src/serialization.h
	src/serializing_listener.h
	src/db_connection_pool.h
	src/db_handler.h
	src/db_handler.cpp
	src/tagged_uuid.h
	src/tagged_uuid.cpp
)

target_link_libraries(game_server ModelGame CONAN_PKG::libpqxx CONAN_PKG::libpq) 

add_executable(game_server_tests
    tests/model-tests.cpp
    tests/loot_generator_tests.cpp
	tests/collision-detector-tests.cpp
)

target_link_libraries(game_server_tests CONAN_PKG::catch2 ModelGame)