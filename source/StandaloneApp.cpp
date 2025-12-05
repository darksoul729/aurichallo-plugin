/*
  ==============================================================================
   Custom Standalone Application for AuricHalo
   
   This file FORCES audio input to be enabled on macOS.
   JUCE's default standalone wrapper disables input for effect plugins.
  ==============================================================================
*/

#define JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP 1

#include <JuceHeader.h>
#include "AuricHaloProcessor.h"

//==============================================================================
class StandaloneFilterWindow : public juce::DocumentWindow
{
public:
    StandaloneFilterWindow(const juce::String& title,
                          juce::Colour backgroundColour,
                          int requiredButtons)
        : DocumentWindow(title, backgroundColour, requiredButtons)
    {
        setUsingNativeTitleBar(true);
        
        // Create the plugin processor
        processor = std::make_unique<AuricHaloProcessor>();
        
        // CRITICAL FIX: FORCE audio input to be enabled on macOS
        // Initialize with INPUT FIRST (this is the key!)
        auto error = deviceManager.initialise(
            2,      // numInputChannelsNeeded - FORCE 2 input channels!
            2,      // numOutputChannelsNeeded
            nullptr,
            true    // selectDefaultDeviceOnFailure
        );
        
        if (error.isNotEmpty())
        {
            DBG("Audio init error: " << error);
        }
        
        // FORCE input channels to be active (macOS needs this!)
        juce::AudioDeviceManager::AudioDeviceSetup setup;
        deviceManager.getAudioDeviceSetup(setup);
        
        // Enable ALL possible input channels (not just 2!)
        setup.inputChannels.clear();
        setup.inputChannels.setRange(0, 32, true);  // Enable up to 32 input channels!
        setup.useDefaultInputChannels = false;      // Don't use default (which is 0)
        
        // Apply the setup
        auto setupError = deviceManager.setAudioDeviceSetup(setup, true);
        if (setupError.isNotEmpty())
        {
            DBG("Setup error: " << setupError);
        }
        
        // Add audio callback
        deviceManager.addAudioCallback(&player);
        player.setProcessor(processor.get());
        
        // Create and show editor
        if (auto* editor = processor->createEditorIfNeeded())
        {
            setContentOwned(editor, true);
            setResizable(false, false);
        }
        
        // Center on screen
        centreWithSize(getWidth(), getHeight());
        setVisible(true);
        
        // Debug output
        deviceManager.getAudioDeviceSetup(setup);
        DBG("=== STANDALONE APP INITIALIZED ===");
        DBG("Input channels enabled: " << setup.inputChannels.toInteger());
        DBG("Active input channels: " << setup.inputChannels.countNumberOfSetBits());
        if (auto* device = deviceManager.getCurrentAudioDevice())
        {
            DBG("Audio device: " << device->getName());
            DBG("Device input channels: " << device->getActiveInputChannels().countNumberOfSetBits());
            DBG("Device output channels: " << device->getActiveOutputChannels().countNumberOfSetBits());
        }
        DBG("==================================");
    }
    
    ~StandaloneFilterWindow() override
    {
        deviceManager.removeAudioCallback(&player);
        player.setProcessor(nullptr);
        processor = nullptr;
    }
    
    void closeButtonPressed() override
    {
        juce::JUCEApplication::getInstance()->systemRequestedQuit();
    }
    
    juce::AudioDeviceManager& getDeviceManager() { return deviceManager; }
    
private:
    //==============================================================================
    class AudioCallback : public juce::AudioIODeviceCallback
    {
    public:
        void setProcessor(juce::AudioProcessor* proc) { processor = proc; }
        
        void audioDeviceIOCallbackWithContext(const float* const* inputChannelData,
                                             int numInputChannels,
                                             float* const* outputChannelData,
                                             int numOutputChannels,
                                             int numSamples,
                                             const juce::AudioIODeviceCallbackContext& context) override
        {
            // DEBUG: Log callback info every 100 calls
            static int callbackCounter = 0;
            if (++callbackCounter % 100 == 0)
            {
                DBG("=== AUDIO CALLBACK ===");
                DBG("Input channels: " << numInputChannels);
                DBG("Output channels: " << numOutputChannels);
                DBG("Samples: " << numSamples);
                
                // Check if input has signal
                float maxInput = 0.0f;
                for (int i = 0; i < numInputChannels; ++i)
                {
                    if (inputChannelData[i] != nullptr)
                    {
                        for (int s = 0; s < numSamples; ++s)
                        {
                            maxInput = std::max(maxInput, std::abs(inputChannelData[i][s]));
                        }
                    }
                }
                DBG("Max input level: " << maxInput);
                DBG("=====================");
            }
            
            if (processor == nullptr)
            {
                // No processor, just clear output
                for (int i = 0; i < numOutputChannels; ++i)
                    if (outputChannelData[i] != nullptr)
                        juce::FloatVectorOperations::clear(outputChannelData[i], numSamples);
                return;
            }
            
            // CRITICAL: Copy input to output FIRST, then process
            // This ensures audio flows through even if processor doesn't handle it
            const int numChannels = juce::jmin(numInputChannels, numOutputChannels);
            
            for (int i = 0; i < numChannels; ++i)
            {
                if (inputChannelData[i] != nullptr && outputChannelData[i] != nullptr)
                {
                    // Copy input to output
                    juce::FloatVectorOperations::copy(outputChannelData[i],
                                                     inputChannelData[i],
                                                     numSamples);
                }
            }
            
            // Clear any extra output channels
            for (int i = numChannels; i < numOutputChannels; ++i)
            {
                if (outputChannelData[i] != nullptr)
                    juce::FloatVectorOperations::clear(outputChannelData[i], numSamples);
            }
            
            // Create buffer wrapper for processing
            juce::AudioBuffer<float> buffer(outputChannelData,
                                           numOutputChannels,
                                           numSamples);
            
            // Process audio (will modify the buffer in-place)
            juce::MidiBuffer midiBuffer;
            processor->processBlock(buffer, midiBuffer);
        }
        
        void audioDeviceAboutToStart(juce::AudioIODevice* device) override
        {
            if (processor != nullptr && device != nullptr)
            {
                processor->setRateAndBufferSizeDetails(device->getCurrentSampleRate(),
                                                      device->getCurrentBufferSizeSamples());
                processor->prepareToPlay(device->getCurrentSampleRate(),
                                       device->getCurrentBufferSizeSamples());
            }
        }
        
        void audioDeviceStopped() override
        {
            if (processor != nullptr)
                processor->releaseResources();
        }
        
    private:
        juce::AudioProcessor* processor = nullptr;
    };
    
    //==============================================================================
    juce::AudioDeviceManager deviceManager;
    std::unique_ptr<AuricHaloProcessor> processor;
    AudioCallback player;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StandaloneFilterWindow)
};

//==============================================================================
class StandaloneFilterApp : public juce::JUCEApplication
{
public:
    StandaloneFilterApp() = default;
    
    const juce::String getApplicationName() override { return "Auric Halo"; }
    const juce::String getApplicationVersion() override { return "1.0.0"; }
    bool moreThanOneInstanceAllowed() override { return true; }
    
    void initialise(const juce::String&) override
    {
        mainWindow = std::make_unique<StandaloneFilterWindow>(
            getApplicationName(),
            juce::Colours::black,
            DocumentWindow::allButtons
        );
    }
    
    void shutdown() override
    {
        mainWindow = nullptr;
    }
    
    void systemRequestedQuit() override
    {
        quit();
    }
    
private:
    std::unique_ptr<StandaloneFilterWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION(StandaloneFilterApp)
