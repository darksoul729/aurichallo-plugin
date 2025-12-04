#include "HaloEQ.h"

void HaloEQ::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    
    lowShelf.prepare(spec);
    highShelf.prepare(spec);
    midPeak.prepare(spec);
    
    reset();
    setTone(50.0f); // Initialize to neutral
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
        midPeak.process(context);
        highShelf.process(context);
    }
    catch (...)
    {
        // If processing fails, pass through
    }
}

void HaloEQ::setTone(float toneValue)
{
    currentTone = juce::jlimit(0.0f, 100.0f, toneValue);
    
    // Map tone: 0 = very dark, 50 = neutral, 100 = very bright
    float toneNormalized = (currentTone - 50.0f) / 50.0f; // -1 to +1
    
    // More aggressive tone shaping
    float lowGain = -toneNormalized * 12.0f;   // Up to ±12dB (was ±6dB)
    float highGain = toneNormalized * 12.0f;   // Up to ±12dB (was ±6dB)
    
    // Add mid scoop/boost for more character
    float midGain = -std::abs(toneNormalized) * 4.0f; // Scoop mids when extreme
    
    // Low shelf at 200Hz for warmth/body
    *lowShelf.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(
        sampleRate, 200.0f, 0.7f, juce::Decibels::decibelsToGain(lowGain)
    );
    
    // Mid peak at 800Hz for presence control
    *midPeak.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(
        sampleRate, 800.0f, 1.5f, juce::Decibels::decibelsToGain(midGain)
    );
    
    // High shelf at 3kHz for brightness/air
    *highShelf.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(
        sampleRate, 3000.0f, 0.7f, juce::Decibels::decibelsToGain(highGain)
    );
}

void HaloEQ::reset()
{
    lowShelf.reset();
    highShelf.reset();
    midPeak.reset();
}
