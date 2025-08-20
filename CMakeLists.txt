cmake_minimum_required(VERSION 3.16)
project(LED-Segments)

set(CMAKE_CXX_STANDARD 17)

# Check if PlatformIO is available
find_program(PLATFORMIO_CMD NAMES platformio pio)
if(NOT PLATFORMIO_CMD)
    message(FATAL_ERROR "PlatformIO CLI not found. Install with: pip install platformio")
endif()

# Add all source files (library only, no main)
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

# Create library target (for CLion code analysis)
add_library(LED-Segments ${SOURCES})

# Set include directories
target_include_directories(LED-Segments PUBLIC src)

# Add FastLED mock headers for CLion intellisense
target_include_directories(LED-Segments PRIVATE
    $ENV{HOME}/.platformio/packages/framework-arduino-samd-seeed/cores/arduino
    $ENV{HOME}/.platformio/lib_deps/seeed_xiao/FastLED/src
)

# PlatformIO integration targets
add_custom_target(pio-build
    COMMAND pio run
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Building with PlatformIO..."
)

add_custom_target(pio-upload
    COMMAND pio run -t upload
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Uploading to board with PlatformIO..."
)

add_custom_target(pio-upload-xiao
    COMMAND pio run -e seeed_xiao -t upload
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Uploading to Seeeduino XIAO..."
)

add_custom_target(pio-monitor
    COMMAND pio device monitor
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Starting serial monitor..."
)

add_custom_target(pio-clean
    COMMAND pio run -t clean
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Cleaning PlatformIO build..."
)

add_custom_target(deploy
    COMMAND pio run -e seeed_xiao -t upload && pio device monitor
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Deploy to XIAO and start serial monitor"
)

# Create example project as subdirectory
add_subdirectory(examples/BasicUsage)