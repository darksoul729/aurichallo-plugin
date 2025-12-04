#pragma once

// Custom Plugin Defines for Standalone Configuration
// This file is included before JUCE's standalone wrapper

// Enable audio input in standalone mode
#ifndef JucePlugin_PreferredChannelConfigurations
  #define JucePlugin_PreferredChannelConfigurations {1, 1}, {2, 2}
#endif

// Enable MIDI input for flexibility (can be used for MIDI control)
#ifndef JucePlugin_WantsMidiInput
  #define JucePlugin_WantsMidiInput 1
#endif

// Ensure standalone uses audio input
#ifndef JUCE_STANDALONE_APPLICATION
  #define JUCE_STANDALONE_APPLICATION 0
#endif
