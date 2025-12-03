#include "HaloKnob.h"

HaloKnob::HaloKnob()
{
    addAndMakeVisible(slider);
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    slider.setRange(0.0, 100.0, 0.1);
    slider.setValue(50.0);
    
    slider.onValueChange = [this]() {
        targetValue = static_cast<float>(slider.getValue());
    };
    
    addAndMakeVisible(label);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::textColourId, juce::Colour(0xffd4af37));
    label.setFont(juce::FontOptions(13.0f, juce::Font::bold));
    
    // Disable animation for performance
    // startTimerHz(60);
}

void HaloKnob::setLabel(const juce::String& text)
{
    labelText = text;
    label.setText(text, juce::dontSendNotification);
}

void HaloKnob::setIsMainKnob(bool isMain)
{
    isMainKnob = isMain;
}

void HaloKnob::timerCallback()
{
    // Animation disabled for performance
}

void HaloKnob::paint(juce::Graphics& g)
{
    juce::ignoreUnused(g);
}

void HaloKnob::resized()
{
    auto bounds = getLocalBounds();
    
    // Label at bottom
    label.setBounds(bounds.removeFromBottom(25));
    
    // Slider takes remaining space
    slider.setBounds(bounds);
}
