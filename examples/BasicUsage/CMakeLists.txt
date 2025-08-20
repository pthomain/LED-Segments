cmake_minimum_required(VERSION 3.16)
project(BasicUsage)

set(CMAKE_CXX_STANDARD 17)

# Include parent library
add_subdirectory(../../src LED-Segments-lib)

# Add example sources
file(GLOB_RECURSE EXAMPLE_SOURCES "*.cpp" "*.h" "*.ino")

# Create example executable (for CLion analysis only)
add_executable(BasicUsage ${EXAMPLE_SOURCES})

# Link to library
target_link_libraries(BasicUsage LED-Segments)

# Set include directories
target_include_directories(BasicUsage PRIVATE 
    ${CMAKE_SOURCE_DIR}/../../src
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# PlatformIO targets for this example
add_custom_target(upload-example
    COMMAND pio run -e seeed_xiao -t upload
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Uploading BasicUsage example..."
)