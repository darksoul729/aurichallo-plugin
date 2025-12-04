#pragma once
#include <JuceHeader.h>

class HaloSlider : public juce::Component
{
public:
    HaloSlider();
    void paint(juce::Graphics&) override;
    void resized() override;
    
    void setLabel(const juce::String& labelText);
    juce::Slider& getSlider() { return slider; }

private:
    juce::Slider slider;
    juce::Label label;
    juce::Label valueLabel;
    juce::String labelText;
};
