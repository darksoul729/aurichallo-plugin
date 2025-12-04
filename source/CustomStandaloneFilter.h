#pragma once

#include <JuceHeader.h>

// Custom Standalone Filter Configuration
// This ensures audio input is properly enabled for the standalone app

namespace juce
{

class CustomStandaloneFilterWindow : public DocumentWindow
{
public:
    CustomStandaloneFilterWindow(const String& title, 
                                  std::unique_ptr<AudioProcessor> processor)
        : DocumentWindow(title, 
                        Desktop::getInstance().getDefaultLookAndFeel()
                            .findColour(ResizableWindow::backgroundColourId),
                        DocumentWindow::minimiseButton | DocumentWindow::closeButton),
          optionsButton("Options")
    {
        setUsingNativeTitleBar(true);
        
        // Create the plugin processor wrapper
        pluginHolder = std::make_unique<StandalonePluginHolder>(
            nullptr,
            false,
            PreferredChannelConfigurations(),
            [&processor]() { return processor.release(); }
        );
        
        // CRITICAL FIX: Force enable audio input channels
        auto& deviceManager = pluginHolder->deviceManager;
        
        DBG("=== CUSTOM STANDALONE INIT ===");
        DBG("Initializing audio device with INPUT ENABLED");
        
        // Initialize with input channels explicitly enabled
        String audioError = deviceManager.initialise(
            2,      // numInputChannelsNeeded - ENABLE 2 INPUT CHANNELS
            2,      // numOutputChannelsNeeded
            nullptr,
            true    // selectDefaultDeviceOnFailure
        );
        
        if (audioError.isNotEmpty())
        {
            DBG("Audio device error: " << audioError);
        }
        
        // FORCE input channels to be active
        auto setup = deviceManager.getAudioDeviceSetup();
        setup.inputChannels.setRange(0, 2, true);  // Enable channels 0 and 1
        setup.useDefaultInputChannels = false;      // Don't use default (which might be disabled)
        
        DBG("Setting up input channels: " << setup.inputChannels.toString(2));
        
        deviceManager.setAudioDeviceSetup(setup, true);
        
        // Verify setup
        auto verifySetup = deviceManager.getAudioDeviceSetup();
        DBG("Verified input channels: " << verifySetup.inputChannels.toString(2));
        DBG("Active input channels: " << verifySetup.inputChannels.countNumberOfSetBits());
        DBG("==============================");
        
        // Create and show the editor
        if (auto* editor = pluginHolder->processor->createEditorIfNeeded())
        {
            setContentNonOwned(editor, true);
        }
        
        // Setup options button
        optionsButton.onClick = [this] { showAudioSettings(); };
        optionsButton.setAlwaysOnTop(true);
        
        setResizable(false, false);
        centreWithSize(getWidth(), getHeight());
        setVisible(true);
    }
    
    ~CustomStandaloneFilterWindow() override
    {
        if (auto* editor = pluginHolder->processor->getActiveEditor())
            pluginHolder->processor->editorBeingDeleted(editor);
    }
    
    void closeButtonPressed() override
    {
        JUCEApplication::getInstance()->systemRequestedQuit();
    }
    
    void showAudioSettings()
    {
        DialogWindow::LaunchOptions options;
        
        auto* content = new AudioDeviceSelectorComponent(
            pluginHolder->deviceManager,
            2, 2,  // min/max input channels
            2, 2,  // min/max output channels
            false, // show MIDI input
            false, // show MIDI output
            false, // show channels as stereo pairs
            false  // hide advanced options
        );
        
        content->setSize(500, 450);
        
        options.content.setOwned(content);
        options.dialogTitle = "Audio Settings";
        options.dialogBackgroundColour = getLookAndFeel().findColour(ResizableWindow::backgroundColourId);
        options.escapeKeyTriggersCloseButton = true;
        options.useNativeTitleBar = true;
        options.resizable = false;
        
        options.launchAsync();
    }
    
    BorderSize<int> getBorderThickness() override
    {
        return BorderSize<int>(0);
    }
    
private:
    struct PreferredChannelConfigurations
    {
        Array<AudioProcessor::BusesLayout> layouts;
        
        PreferredChannelConfigurations()
        {
            // Stereo in/out
            AudioProcessor::BusesLayout layout;
            layout.inputBuses.add(AudioChannelSet::stereo());
            layout.outputBuses.add(AudioChannelSet::stereo());
            layouts.add(layout);
            
            // Mono in/out
            layout.inputBuses.clear();
            layout.outputBuses.clear();
            layout.inputBuses.add(AudioChannelSet::mono());
            layout.outputBuses.add(AudioChannelSet::mono());
            layouts.add(layout);
        }
    };
    
    std::unique_ptr<StandalonePluginHolder> pluginHolder;
    TextButton optionsButton;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomStandaloneFilterWindow)
};

} // namespace juce
