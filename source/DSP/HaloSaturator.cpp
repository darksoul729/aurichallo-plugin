#include "HaloSaturator.h"

void HaloSaturator::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    drive = 1.0f; // Initialize to safe value
}

void HaloSaturator::process(juce::AudioBuffer<float>& buffer)
{
    if (buffer.getNumChannels() <= 0 || buffer.getNumSamples() <= 0)
        return;
    
    if (drive <= 0.0f)
    {
        drive = 1.0f; // Reset to safe value
        return;
    }
    
    float invDrive = 1.0f / drive;
    
    try
    {
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            auto* data = buffer.getWritePointer(channel);
            if (data == nullptr)
                continue;
            
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                float x = data[sample] * drive;
                data[sample] = std::tanh(x) * invDrive;
            }
        }
    }
    catch (...)
    {
        // If processing fails, pass through
    }
}

void HaloSaturator::setDrive(float driveAmount)
{
    drive = juce::jlimit(1.0f, 10.0f, driveAmount);
}
