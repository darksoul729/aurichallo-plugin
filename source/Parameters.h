#pragma once
#include <JuceHeader.h>

class AuricHaloProcessor;

class AuricHaloParameters
{
public:
    AuricHaloParameters(AuricHaloProcessor& processor);
    
    void saveState(juce::MemoryBlock& destData);
    void loadState(const void* data, int sizeInBytes);
    
    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; }

private:
    juce::AudioProcessorValueTreeState apvts;
    
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
};
