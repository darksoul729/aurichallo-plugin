#pragma once
#include <JuceHeader.h>

class LevelMeter : public juce::Component, private juce::Timer
{
public:
    LevelMeter();
    ~LevelMeter() override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    void setLevel(float newLevel);
    void setIsInput(bool isInput) { isInputMeter = isInput; }
    
private:
    void timerCallback() override;
    
    float currentLevel = 0.0f;
    float displayLevel = 0.0f;
    bool isInputMeter = true;
    
    static constexpr float decayRate = 0.95f;
    static constexpr int numSegments = 20;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelMeter)
};
