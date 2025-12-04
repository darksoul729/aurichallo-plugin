#pragma once

// Standalone Plugin Configuration
// This file configures the JUCE standalone wrapper to enable audio input

// Force audio input to be enabled in standalone mode
#define JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP 1

// Custom standalone initialization
namespace juce
{
    // This will be called by JUCE's StandalonePluginHolder
    inline void customStandaloneDeviceManagerInit(AudioDeviceManager& deviceManager)
    {
        DBG("=== CUSTOM STANDALONE DEVICE INIT ===");
        
        // Initialize with input explicitly enabled
        String error = deviceManager.initialise(
            2,      // numInputChannelsNeeded - ENABLE INPUT
            2,      // numOutputChannelsNeeded
            nullptr,
            true
        );
        
        if (error.isNotEmpty())
        {
            DBG("Audio init error: " << error);
        }
        
        // Force enable input channels
        auto setup = deviceManager.getAudioDeviceSetup();
        setup.inputChannels.setRange(0, 2, true);
        setup.useDefaultInputChannels = false;
        
        DBG("Input channels enabled: " << setup.inputChannels.toString(2));
        
        deviceManager.setAudioDeviceSetup(setup, true);
        
        // Verify
        auto verify = deviceManager.getAudioDeviceSetup();
        DBG("Active inputs: " << verify.inputChannels.countNumberOfSetBits());
        DBG("=====================================");
    }
}
