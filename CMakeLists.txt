cmake_minimum_required(VERSION 3.15)
project(calcmod)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find Max SDK
if(NOT DEFINED MAX_SDK_DIR)
    if(WIN32)
        set(MAX_SDK_DIR "C:/Program Files/Cycling '74/max-sdk-8.0.3")
    else()
        set(MAX_SDK_DIR "/Applications/Max.app/Contents/Resources/max-sdk-8.0.3")
    endif()
endif()

# Include directories
include_directories(
    "${MAX_SDK_DIR}/source/c74support/max-includes"
    "${MAX_SDK_DIR}/source/c74support/msp-includes"
)

# Add library
add_library(calcmod MODULE
    src/calcmod.c
)

# Set output name and directory
set_target_properties(calcmod PROPERTIES
    PREFIX ""
    SUFFIX ".mxe64"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/externals"
)

# Link libraries
if(WIN32)
    target_link_libraries(calcmod
        "${MAX_SDK_DIR}/source/c74support/max-includes/x64/MaxAPI.lib"
    )
endif()