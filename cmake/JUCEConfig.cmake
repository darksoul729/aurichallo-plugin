# Helper to find JUCE installation

if(DEFINED ENV{JUCE_DIR})
    set(JUCE_DIR $ENV{JUCE_DIR})
endif()

if(NOT JUCE_DIR)
    message(STATUS "JUCE_DIR not set, assuming submodule at external/JUCE")
    set(JUCE_DIR "${CMAKE_SOURCE_DIR}/external/JUCE")
endif()

if(NOT EXISTS "${JUCE_DIR}/CMakeLists.txt")
    message(FATAL_ERROR "JUCE not found at ${JUCE_DIR}")
endif()

message(STATUS "Using JUCE from: ${JUCE_DIR}")
