#pragma once
#include <JuceHeader.h>

class HaloKnob : public juce::Component, private juce::Timer
{
public:
    HaloKnob();
    void paint(juce::Graphics&) override;
    void resized() override;
    
    void setLabel(const juce::String& labelText);
    void setIsMainKnob(bool isMain);
    juce::Slider& getSlider() { return slider; }

private:
    void timerCallback() override;
    
    juce::Slider slider;
    juce::Label label;
    juce::String labelText;
    bool isMainKnob = false;
    float animatedValue = 0.0f;
    float targetValue = 0.0f;
};
