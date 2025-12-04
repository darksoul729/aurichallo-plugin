#include "HaloSlider.h"

HaloSlider::HaloSlider()
{
    addAndMakeVisible(slider);
    slider.setSliderStyle(juce::Slider::LinearVertical);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    slider.setRange(0.0, 150.0, 1.0);
    slider.setValue(100.0);
    
    addAndMakeVisible(label);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::textColourId, juce::Colour(0xffd4af37));
    label.setFont(juce::FontOptions(12.0f, juce::Font::bold));
    
    addAndMakeVisible(valueLabel);
    valueLabel.setJustificationType(juce::Justification::centred);
    valueLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    valueLabel.setFont(juce::FontOptions(11.0f));
    
    slider.onValueChange = [this]() {
        valueLabel.setText(juce::String((int)slider.getValue()) + "%", juce::dontSendNotification);
    };
    
    valueLabel.setText("100%", juce::dontSendNotification);
}

void HaloSlider::setLabel(const juce::String& text)
{
    labelText = text;
    label.setText(text, juce::dontSendNotification);
}

void HaloSlider::paint(juce::Graphics& g)
{
    juce::ignoreUnused(g);
}

void HaloSlider::resized()
{
    auto bounds = getLocalBounds();
    
    // Label at top
    label.setBounds(bounds.removeFromTop(20));
    
    // Value label at bottom
    valueLabel.setBounds(bounds.removeFromBottom(20));
    
    // Slider takes remaining space
    slider.setBounds(bounds.reduced(10, 5));
}
