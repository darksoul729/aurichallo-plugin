#pragma once
#include <JuceHeader.h>

class HaloCompressor
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::AudioBuffer<float>& buffer);
    void reset();

private:
    juce::dsp::Compressor<float> compressor;
};
