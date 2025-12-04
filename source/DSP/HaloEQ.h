#pragma once
#include <JuceHeader.h>

class HaloEQ
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::AudioBuffer<float>& buffer);
    void setTone(float toneValue); // 0-100, 50 = neutral
    void reset();

private:
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, 
                                    juce::dsp::IIR::Coefficients<float>> lowShelf;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, 
                                    juce::dsp::IIR::Coefficients<float>> highShelf;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, 
                                    juce::dsp::IIR::Coefficients<float>> midPeak;
    
    double sampleRate = 44100.0;
    float currentTone = 50.0f;
};
