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
    
    // Subtle drive scaling for musical saturation
    float driveGain = 1.0f + (drive - 1.0f) * 0.8f; // More subtle drive effect
    float makeup = 1.0f / std::sqrt(driveGain); // Compensate output level
    
    try
    {
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            auto* data = buffer.getWritePointer(channel);
            if (data == nullptr)
                continue;
            
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                float input = data[sample];
                
                // Gentle saturation for musical warmth
                float x = input * driveGain;
                
                // Single-stage soft saturation (more subtle)
                float output;
                if (drive < 3.0f)
                {
                    // Subtle mode: soft tanh only
                    output = std::tanh(x * 0.5f);
                }
                else
                {
                    // Harder mode: tube-like with soft clip
                    float stage1 = tubeDistortion(x * 0.7f);
                    output = std::tanh(stage1 * 0.6f);
                }
                
                // Apply makeup gain
                data[sample] = output * makeup;
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

float HaloSaturator::softClip(float x)
{
    // Soft clipping with asymmetry for even harmonics
    if (x > 1.0f)
        return 1.0f - std::exp(-(x - 1.0f));
    else if (x < -1.0f)
        return -1.0f + std::exp((x + 1.0f)) * 0.8f; // Asymmetric
    else
        return x;
}

float HaloSaturator::hardClip(float x)
{
    return juce::jlimit(-1.0f, 1.0f, x);
}

float HaloSaturator::tubeDistortion(float x)
{
    // Gentle tube-like distortion curve
    float abs_x = std::abs(x);
    
    if (abs_x < 0.5f)
        return x * 1.2f; // Subtle boost
    else if (abs_x < 0.8f)
        return (3.0f - std::pow(2.0f - 3.0f * abs_x, 2.0f)) / 3.0f * (x < 0 ? -1.0f : 1.0f);
    else
        return std::tanh(x * 1.5f); // Soft limiting instead of hard clip
}
