#pragma once
#include <JuceHeader.h>
#include "AuricHaloProcessor.h"
#include "UI/LookAndFeel.h"
#include "UI/Components/HaloKnob.h"
#include "UI/Components/HaloSlider.h"
#include "UI/Components/AudioFilePlayer.h"
#include "UI/LicenseDialog.h"
#include "UI/LevelMeter.h"

class AuricHaloEditor : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    enum class UISize
    {
        Small,
        Medium,
        Large
    };
    
    AuricHaloEditor(AuricHaloProcessor&);
    ~AuricHaloEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;
    void drawHaloGlow(juce::Graphics& g, juce::Rectangle<float> bounds, float intensity);
    void checkLicenseStatus();
    void showLicenseDialog();
    void setUISize(UISize newSize);
    void updateSizeForCurrentScale();
    
    AuricHaloProcessor& processor;
    AuricHaloLookAndFeel lookAndFeel;
    
    // UI Size
    UISize currentSize = UISize::Medium;
    float scaleFactor = 1.0f;
    
    // Sliders (Input/Output are now vertical sliders)
    HaloSlider inputSlider;
    HaloSlider outputSlider;
    
    // Knobs (Drive, Tone, Mix remain as knobs)
    HaloKnob toneKnob;
    HaloKnob driveKnob;
    HaloKnob mixKnob;
    
    // Toggle button
    juce::TextButton haloEngineButton;
    bool haloEngineOn = false;
    
    // License button
    juce::TextButton licenseButton;
    
    // Audio file player
    AudioFilePlayer audioPlayer;
    
    // License dialog
    std::unique_ptr<LicenseDialog> licenseDialog;
    
    // Level meters
    LevelMeter inputMeter;
    LevelMeter outputMeter;
    
    // Oversampling selector
    juce::ComboBox oversamplingBox;
    juce::Label oversamplingLabel;
    
    // Size selector
    juce::ComboBox sizeSelector;
    juce::Label sizeLabel;
    
    // Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> toneAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oversamplingAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AuricHaloEditor)
};
