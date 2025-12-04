#pragma once
#include <JuceHeader.h>
#include "Parameters.h"
#include "DSP/HaloEQ.h"
#include "DSP/HaloCompressor.h"
#include "DSP/HaloSaturator.h"
#include "LicenseManager.h"

class AuricHaloProcessor : public juce::AudioProcessor
{
public:
    AuricHaloProcessor();
    ~AuricHaloProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "Auric Halo"; }
    bool acceptsMidi() const override { return true; }  // Enable MIDI for standalone flexibility
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    AuricHaloParameters& getParameters() { return parameters; }
    LicenseManager& getLicenseManager() { return licenseManager; }
    
    void setAudioPlayer(class AudioFilePlayer* player) { audioPlayer = player; }
    
    // Level metering
    float getInputLevel() const { return inputLevel.load(); }
    float getOutputLevel() const { return outputLevel.load(); }

private:
    AuricHaloParameters parameters;
    HaloEQ haloEQ;
    HaloCompressor haloCompressor;
    HaloSaturator haloSaturator;
    LicenseManager licenseManager;
    
    class AudioFilePlayer* audioPlayer = nullptr;
    
    // Oversampling (2x)
    std::unique_ptr<juce::dsp::Oversampling<float>> oversampling;
    
    // Level meters
    std::atomic<float> inputLevel { 0.0f };
    std::atomic<float> outputLevel { 0.0f };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AuricHaloProcessor)
};
