/*
  Custom Standalone Application
  This file provides a custom standalone wrapper with audio input properly enabled
*/

#define JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP 1

#include <JuceHeader.h>
#include "AuricHaloProcessor.h"

//==============================================================================
class StandaloneFilterWindow : public juce::DocumentWindow
{
public:
    StandaloneFilterWindow(const juce::String& title)
        : DocumentWindow(title,
                        juce::Desktop::getInstance().getDefaultLookAndFeel()
                            .findColour(juce::ResizableWindow::backgroundColourId),
                        DocumentWindow::minimiseButton | DocumentWindow::closeButton)
    {
        setUsingNativeTitleBar(true);
        
        // Create processor
        processor = std::make_unique<AuricHaloProcessor>();
        
        // Initialize audio device manager with INPUT ENABLED
        DBG("=== STANDALONE APP INIT ===");
        DBG("Initializing audio with INPUT channels enabled");
        
        juce::String audioError = deviceManager.initialise(
            2,      // numInputChannelsNeeded - CRITICAL: Enable 2 input channels
            2,      // numOutputChannelsNeeded
            nullptr,
            true
        );
        
        if (!audioError.isEmpty())
        {
            DBG("Audio initialization error: " << audioError);
            juce::AlertWindow::showMessageBoxAsync(
                juce::AlertWindow::WarningIcon,
                "Audio Device Error",
                audioError
            );
        }
        
        // FORCE input channels to be active (fixes macOS input disabled issue)
        auto setup = deviceManager.getAudioDeviceSetup();
        setup.inputChannels.setRange(0, 2, true);  // Enable input channels 0 and 1
        setup.useDefaultInputChannels = false;
        
        DBG("Forcing input channels: " << setup.inputChannels.toString(2));
        
        juce::String setupResult = deviceManager.setAudioDeviceSetup(setup, true);
        DBG("Setup result: " << (setupResult.isEmpty() ? "SUCCESS" : setupResult));
        
        // Verify the setup
        auto verifySetup = deviceManager.getAudioDeviceSetup();
        DBG("Verified input channels: " << verifySetup.inputChannels.toString(2));
        DBG("Active input count: " << verifySetup.inputChannels.countNumberOfSetBits());
        DBG("Input device: " << verifySetup.inputDeviceName);
        DBG("Output device: " << verifySetup.outputDeviceName);
        DBG("===========================");
        
        // Setup audio callback
        player.setProcessor(processor.get());
        deviceManager.addAudioCallback(&player);
        
        // Prepare processor
        if (auto* device = deviceManager.getCurrentAudioDevice())
        {
            processor->setRateAndBufferSizeDetails(
                device->getCurrentSampleRate(),
                device->getCurrentBufferSizeSamples()
            );
            
            processor->prepareToPlay(
                device->getCurrentSampleRate(),
                device->getCurrentBufferSizeSamples()
            );
        }
        
        // Create and show editor
        if (auto* editor = processor->createEditorIfNeeded())
        {
            setContentNonOwned(editor, true);
            setResizable(false, false);
        }
        
        centreWithSize(getWidth(), getHeight());
        setVisible(true);
    }
    
    ~StandaloneFilterWindow() override
    {
        deviceManager.removeAudioCallback(&player);
        player.setProcessor(nullptr);
        
        if (processor != nullptr)
        {
            if (auto* editor = processor->getActiveEditor())
                processor->editorBeingDeleted(editor);
            
            processor->releaseResources();
        }
    }
    
    void closeButtonPressed() override
    {
        juce::JUCEApplication::getInstance()->systemRequestedQuit();
    }
    
    void showAudioSettings()
    {
        juce::DialogWindow::LaunchOptions options;
        
        auto* selector = new juce::AudioDeviceSelectorComponent(
            deviceManager,
            2, 2,  // min/max input channels
            2, 2,  // min/max output channels
            false, // show MIDI input
            false, // show MIDI output  
            false, // show channels as stereo pairs
            false  // hide advanced options
        );
        
        selector->setSize(500, 450);
        
        options.content.setOwned(selector);
        options.dialogTitle = "Audio Settings";
        options.dialogBackgroundColour = getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId);
        options.escapeKeyTriggersCloseButton = true;
        options.useNativeTitleBar = true;
        options.resizable = false;
        
        options.launchAsync();
    }
    
    juce::AudioDeviceManager& getDeviceManager() { return deviceManager; }
    
private:
    juce::AudioDeviceManager deviceManager;
    juce::AudioProcessorPlayer player;
    std::unique_ptr<AuricHaloProcessor> processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StandaloneFilterWindow)
};

//==============================================================================
class StandaloneApplication : public juce::JUCEApplication
{
public:
    StandaloneApplication() = default;
    
    const juce::String getApplicationName() override { return "Auric Halo"; }
    const juce::String getApplicationVersion() override { return "1.0.0"; }
    bool moreThanOneInstanceAllowed() override { return false; }
    
    void initialise(const juce::String&) override
    {
        mainWindow = std::make_unique<StandaloneFilterWindow>(getApplicationName());
    }
    
    void shutdown() override
    {
        mainWindow = nullptr;
    }
    
    void systemRequestedQuit() override
    {
        quit();
    }
    
    void anotherInstanceStarted(const juce::String&) override {}
    
private:
    std::unique_ptr<StandaloneFilterWindow> mainWindow;
};

//==============================================================================
START_JUCE_APPLICATION(StandaloneApplication)
