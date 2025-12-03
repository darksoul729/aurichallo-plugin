#include "AuricHaloProcessor.h"
#include "AuricHaloEditor.h"

AuricHaloProcessor::AuricHaloProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this)
{
}

AuricHaloProcessor::~AuricHaloProcessor()
{
}

void AuricHaloProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    if (sampleRate <= 0 || samplesPerBlock <= 0)
        return;
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32>(getTotalNumOutputChannels());

    try
    {
        haloEQ.prepare(spec);
        haloEQ.reset();
        
        haloCompressor.prepare(spec);
        haloCompressor.reset();
        
        haloSaturator.prepare(spec);
        haloSaturator.reset();
    }
    catch (...)
    {
        // Initialization failed, but don't crash
    }
    
    if (audioPlayer != nullptr)
        audioPlayer->prepareToPlay(sampleRate, samplesPerBlock);
}

void AuricHaloProcessor::releaseResources()
{
    if (audioPlayer != nullptr)
        audioPlayer->releaseResources();
}

void AuricHaloProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    // Clear any unused output channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
    
    // SIMPLE PASSTHROUGH - Audio input langsung ke output
    // DSP processing disabled untuk testing
    
    // Get parameters
    float mixValue = *parameters.getAPVTS().getRawParameterValue("mix") / 100.0f;
    float outputValue = *parameters.getAPVTS().getRawParameterValue("output") / 100.0f;
    
    // Apply output gain
    for (int channel = 0; channel < totalNumOutputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] *= outputValue;
        }
    }
    
    // TODO: Enable DSP processing setelah passthrough confirmed working
    /*
    try
    {
        haloEQ.process(buffer);
        haloCompressor.process(buffer);
        haloSaturator.process(buffer);
    }
    catch (...) {}
    */
}

juce::AudioProcessorEditor* AuricHaloProcessor::createEditor()
{
    return new AuricHaloEditor(*this);
}

void AuricHaloProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    parameters.saveState(destData);
}

void AuricHaloProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    parameters.loadState(data, sizeInBytes);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AuricHaloProcessor();
}
