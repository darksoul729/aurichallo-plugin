#include "HaloEQ.h"

void HaloEQ::prepare(const juce::dsp::ProcessSpec& spec)
{
    lowShelf.prepare(spec);
    highShelf.prepare(spec);
    reset();
}

void HaloEQ::process(juce::AudioBuffer<float>& buffer)
{
    if (buffer.getNumChannels() <= 0 || buffer.getNumSamples() <= 0)
        return;
    
    try
    {
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);
        
        lowShelf.process(context);
        highShelf.process(context);
    }
    catch (...)
    {
        // If processing fails, pass through
    }
}

void HaloEQ::reset()
{
    lowShelf.reset();
    highShelf.reset();
}
