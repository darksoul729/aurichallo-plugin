#include "Parameters.h"
#include "AuricHaloProcessor.h"

AuricHaloParameters::AuricHaloParameters(AuricHaloProcessor& processor)
    : apvts(processor, nullptr, "Parameters", createParameterLayout())
{
}

juce::AudioProcessorValueTreeState::ParameterLayout AuricHaloParameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "input", "Input", 0.0f, 150.0f, 100.0f)); // Default: 100% (unity), range 0-150%
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "tone", "Tone", 0.0f, 100.0f, 50.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "drive", "Drive", 0.0f, 100.0f, 20.0f)); // Default: 20% (subtle)
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "mix", "Mix", 0.0f, 100.0f, 30.0f)); // Default: 30% (more subtle)
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "output", "Output", 0.0f, 150.0f, 100.0f)); // Default: 100% (unity), range 0-150%
    
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        "oversampling", "Oversampling", 
        juce::StringArray{"Off", "2x", "4x"}, 1)); // Default: 2x
    
    return layout;
}

void AuricHaloParameters::saveState(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    juce::AudioProcessor::copyXmlToBinary(*xml, destData);
}

void AuricHaloParameters::loadState(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(juce::AudioProcessor::getXmlFromBinary(data, sizeInBytes));
    if (xml && xml->hasTagName(apvts.state.getType()))
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
}
