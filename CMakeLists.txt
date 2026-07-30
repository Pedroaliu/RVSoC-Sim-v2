cmake_minimum_required(VERSION 3.20)

project(rvsoc_sim_v2 VERSION 0.1.0 LANGUAGES CXX)

include(CTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(archlab-sim
    src/sim/event_queue.cpp
    src/sim/sim_object.cpp
    src/platform/memory_map.cpp
    src/devices/simple_ram.cpp
    src/devices/magic_device.cpp
    src/stats/stats_manager.cpp
    src/cpu/fake_cpu.cpp
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
    file(GLOB_RECURSE ARCHLAB_TEST_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
    )

    add_executable(archlab-tests ${ARCHLAB_TEST_SOURCES})
    target_link_libraries(archlab-tests PRIVATE ArchLab::Sim)
    target_include_directories(archlab-tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
    )

    if(MSVC)
        target_compile_options(archlab-tests PRIVATE /W4 /permissive-)
    else()
        target_compile_options(archlab-tests PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
    endif()

    add_test(NAME archlab-tests COMMAND archlab-tests)
endif()
