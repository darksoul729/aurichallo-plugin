#include "HaloCompressor.h"

void HaloCompressor::prepare(const juce::dsp::ProcessSpec& spec)
{
    compressor.prepare(spec);
    compressor.setThreshold(-10.0f);
    compressor.setRatio(4.0f);
    compressor.setAttack(5.0f);
    compressor.setRelease(100.0f);
}

void HaloCompressor::process(juce::AudioBuffer<float>& buffer)
{
    if (buffer.getNumChannels() <= 0 || buffer.getNumSamples() <= 0)
        return;
    
    try
    {
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);
        compressor.process(context);
    }
    catch (...)
    {
        // If processing fails, pass through
    }
}

void HaloCompressor::reset()
{
    compressor.reset();
}
