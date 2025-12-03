#pragma once
#include <JuceHeader.h>

class HaloSaturator
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::AudioBuffer<float>& buffer);
    void setDrive(float driveAmount);
    void reset() { drive = 1.0f; }

private:
    float drive = 1.0f;
    double sampleRate = 44100.0;
};
