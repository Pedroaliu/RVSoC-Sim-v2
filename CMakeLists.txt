cmake_minimum_required(VERSION 3.20)

project(rvsoc_sim_v2 VERSION 0.1.0 LANGUAGES CXX)

include(CTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(archlab-sim
    src/sim/event_queue.cpp
    src/platform/memory_map.cpp
    src/devices/simple_ram.cpp
    src/devices/magic_device.cpp
    src/stats/stats_manager.cpp
    src/cpu/fake_cpu.cpp
        include/archlab/sim/sim_object.h
        src/sim/sim_object.cpp
)

add_library(ArchLab::Sim ALIAS archlab-sim)

target_include_directories(archlab-sim
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if(MSVC)
    target_compile_options(archlab-sim PRIVATE /W4 /permissive-)
else()
    target_compile_options(archlab-sim PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
endif()

add_executable(rvsoc-sim src/main.cpp)
target_link_libraries(rvsoc-sim PRIVATE ArchLab::Sim)

if(BUILD_TESTING)
    add_executable(archlab-tests tests/test_main.cpp)
    target_link_libraries(archlab-tests PRIVATE ArchLab::Sim)

    if(MSVC)
        target_compile_options(archlab-tests PRIVATE /W4 /permissive-)
    else()
        target_compile_options(archlab-tests PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
    endif()

    add_test(NAME archlab-tests COMMAND archlab-tests)
endif()
