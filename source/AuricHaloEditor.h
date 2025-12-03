#pragma once
#include <JuceHeader.h>
#include "AuricHaloProcessor.h"
#include "UI/LookAndFeel.h"
#include "UI/Components/HaloKnob.h"
#include "UI/Components/AudioFilePlayer.h"

class AuricHaloEditor : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    AuricHaloEditor(AuricHaloProcessor&);
    ~AuricHaloEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;
    void drawHaloGlow(juce::Graphics& g, juce::Rectangle<float> bounds, float intensity);
    
    AuricHaloProcessor& processor;
    AuricHaloLookAndFeel lookAndFeel;
    
    // Knobs
    HaloKnob toneKnob;
    HaloKnob driveKnob;
    HaloKnob mixKnob;
    HaloKnob outputKnob;
    
    // Toggle button
    juce::TextButton haloEngineButton;
    bool haloEngineOn = false;
    
    // Audio file player
    AudioFilePlayer audioPlayer;
    
    // Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> toneAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AuricHaloEditor)
};
