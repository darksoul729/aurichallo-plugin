cmake_minimum_required(VERSION 3.22)

project(AuricHalo VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# JUCE setup
add_subdirectory(external/JUCE)

# Plugin formats
set(FORMATS VST3 AU Standalone)

juce_add_plugin(AuricHalo
    COMPANY_NAME "Auric Audio"
    PLUGIN_MANUFACTURER_CODE Aurc
    PLUGIN_CODE AuHa
    FORMATS ${FORMATS}
    PRODUCT_NAME "Auric Halo"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    VST3_CATEGORIES "Fx" "Distortion" "Dynamics"
    AU_MAIN_TYPE "kAudioUnitType_Effect"
    AAX_CATEGORY "AAX_ePlugInCategory_Harmonic"
)

# Source files
target_sources(AuricHalo PRIVATE
    source/AuricHaloProcessor.cpp
    source/AuricHaloEditor.cpp
    source/Parameters.cpp
    source/DSP/HaloEQ.cpp
    source/DSP/HaloCompressor.cpp
    source/DSP/HaloSaturator.cpp
    source/UI/LookAndFeel.cpp
    source/UI/Components/HaloMeter.cpp
    source/UI/Components/HaloKnob.cpp
)

target_include_directories(AuricHalo PRIVATE source)

target_compile_definitions(AuricHalo PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JucePlugin_Build_VST=0
)

target_link_libraries(AuricHalo PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
)

# CPack installer configuration
include(CPack)
set(CPACK_PACKAGE_NAME "AuricHalo")
set(CPACK_PACKAGE_VENDOR "Auric Audio")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
