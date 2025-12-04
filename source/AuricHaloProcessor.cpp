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
    
    // DEBUG: Log audio setup
    DBG("=== PREPARE TO PLAY ===");
    DBG("Sample rate: " << sampleRate);
    DBG("Samples per block: " << samplesPerBlock);
    DBG("Total input channels: " << getTotalNumInputChannels());
    DBG("Total output channels: " << getTotalNumOutputChannels());
    DBG("Main input channels: " << getMainBusNumInputChannels());
    DBG("Main output channels: " << getMainBusNumOutputChannels());
    DBG("=======================");
    
    // Get oversampling setting
    int oversamplingChoice = static_cast<int>(*parameters.getAPVTS().getRawParameterValue("oversampling"));
    int oversamplingFactor = (oversamplingChoice == 0) ? 1 : (oversamplingChoice == 1) ? 2 : 4;
    
    // Initialize oversampling
    if (oversamplingFactor > 1)
    {
        int order = (oversamplingFactor == 2) ? 1 : 2; // 2x = order 1, 4x = order 2
        oversampling = std::make_unique<juce::dsp::Oversampling<float>>(
            getTotalNumOutputChannels(),
            order,
            juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR,
            true
        );
        
        oversampling->initProcessing(static_cast<size_t>(samplesPerBlock));
    }
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate * oversamplingFactor;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock * oversamplingFactor);
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
    
    // DEBUG: Log channel and buffer info every 100 blocks
    static int debugCounter = 0;
    if (++debugCounter % 100 == 0)
    {
        DBG("=== AUDIO DEBUG ===");
        DBG("Input channels: " << totalNumInputChannels);
        DBG("Output channels: " << totalNumOutputChannels);
        DBG("Buffer channels: " << buffer.getNumChannels());
        DBG("Buffer samples: " << buffer.getNumSamples());
        
        // Check if buffer has any signal
        float maxSample = 0.0f;
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        {
            auto* data = buffer.getReadPointer(ch);
            for (int i = 0; i < buffer.getNumSamples(); ++i)
            {
                maxSample = std::max(maxSample, std::abs(data[i]));
            }
        }
        DBG("Max sample value in buffer: " << maxSample);
        DBG("==================");
    }
    
    // Safety check
    if (buffer.getNumSamples() == 0 || totalNumOutputChannels == 0)
        return;
    
    // Clear any unused output channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
    
    // Measure input level (before any processing)
    // Use buffer.getNumChannels() instead of totalNumInputChannels for standalone
    float inLevel = 0.0f;
    int channelsToCheck = std::min(buffer.getNumChannels(), totalNumOutputChannels);
    for (int ch = 0; ch < channelsToCheck; ++ch)
    {
        if (buffer.getNumSamples() > 0)
        {
            auto magnitude = buffer.getMagnitude(ch, 0, buffer.getNumSamples());
            inLevel = std::max(inLevel, magnitude);
        }
    }
    inputLevel.store(inLevel);
    
    // Get parameters
    float inputValue = *parameters.getAPVTS().getRawParameterValue("input") / 100.0f;
    float driveValue = *parameters.getAPVTS().getRawParameterValue("drive");
    float toneValue = *parameters.getAPVTS().getRawParameterValue("tone");
    float mixValue = *parameters.getAPVTS().getRawParameterValue("mix") / 100.0f;
    float outputValue = *parameters.getAPVTS().getRawParameterValue("output") / 100.0f;
    
    // Apply input gain
    for (int channel = 0; channel < totalNumOutputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] *= inputValue;
        }
    }
    
    // Store dry signal for mix (after input gain)
    juce::AudioBuffer<float> dryBuffer;
    dryBuffer.makeCopyOf(buffer);
    
    try
    {
        // Update DSP parameters
        haloSaturator.setDrive(driveValue);
        haloEQ.setTone(toneValue);
        
        if (oversampling != nullptr)
        {
            // Upsample
            juce::dsp::AudioBlock<float> block(buffer);
            auto oversampledBlock = oversampling->processSamplesUp(block);
            
            // Create temporary buffer for oversampled processing
            juce::AudioBuffer<float> oversampledBuffer(
                oversampledBlock.getNumChannels(),
                oversampledBlock.getNumSamples()
            );
            
            // Copy oversampled data to buffer
            for (size_t ch = 0; ch < oversampledBlock.getNumChannels(); ++ch)
            {
                auto* dest = oversampledBuffer.getWritePointer(ch);
                auto* src = oversampledBlock.getChannelPointer(ch);
                std::copy(src, src + oversampledBlock.getNumSamples(), dest);
            }
            
            // Process chain: Saturation -> EQ -> Compression
            haloSaturator.process(oversampledBuffer);
            haloEQ.process(oversampledBuffer);
            haloCompressor.process(oversampledBuffer);
            
            // Copy back to oversampled block
            for (size_t ch = 0; ch < oversampledBlock.getNumChannels(); ++ch)
            {
                auto* dest = oversampledBlock.getChannelPointer(ch);
                auto* src = oversampledBuffer.getReadPointer(ch);
                std::copy(src, src + oversampledBlock.getNumSamples(), dest);
            }
            
            // Downsample
            oversampling->processSamplesDown(block);
        }
        else
        {
            // No oversampling - process directly
            haloSaturator.process(buffer);
            haloEQ.process(buffer);
            haloCompressor.process(buffer);
        }
        
        // Smart Mix: Intelligent blend that respects processing intensity
        // Calculate processing intensity (0-1)
        float driveIntensity = driveValue / 100.0f; // 0-1
        float toneDeviation = std::abs(toneValue - 50.0f) / 50.0f; // 0-1 (deviation from neutral)
        float processingIntensity = std::max(driveIntensity, toneDeviation); // Overall processing amount
        
        // Smart behavior:
        // - If no processing (intensity=0): Always output dry regardless of Mix
        // - If full processing (intensity=1): Mix works normally (0=dry, 100=wet)
        // - In between: Mix range scales with processing intensity
        
        float effectiveMix;
        if (processingIntensity < 0.01f)
        {
            // No processing: Force dry signal
            effectiveMix = 0.0f;
        }
        else
        {
            // Scale mix range by processing intensity
            // Mix 0% = always dry
            // Mix 100% = wet (scaled by processing intensity)
            effectiveMix = mixValue * processingIntensity;
        }
        
        // Equal-power crossfade
        float dryGain = std::cos(effectiveMix * juce::MathConstants<float>::halfPi);
        float wetGain = std::sin(effectiveMix * juce::MathConstants<float>::halfPi);
        
        for (int channel = 0; channel < totalNumOutputChannels; ++channel)
        {
            auto* wetData = buffer.getWritePointer(channel);
            auto* dryData = dryBuffer.getReadPointer(channel);
            
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                wetData[sample] = dryData[sample] * dryGain + wetData[sample] * wetGain;
            }
        }
        
        // Apply output gain
        for (int channel = 0; channel < totalNumOutputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer(channel);
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                channelData[sample] *= outputValue;
            }
        }
    }
    catch (...)
    {
        // If processing fails, pass through dry signal
        buffer.makeCopyOf(dryBuffer);
    }
    
    // Measure output level
    float outLevel = 0.0f;
    for (int ch = 0; ch < totalNumOutputChannels; ++ch)
    {
        auto magnitude = buffer.getMagnitude(ch, 0, buffer.getNumSamples());
        outLevel = std::max(outLevel, magnitude);
    }
    outputLevel.store(outLevel);
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
