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
    auto bounds = slider.getBounds().toFloat();
    auto centre = bounds.getCentre();
    auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f - 10.0f;
    
    // Get current value (0-1)
    float value = (float)slider.getValue();
    float normalizedValue = (value - (float)slider.getMinimum()) / ((float)slider.getMaximum() - (float)slider.getMinimum());
    
    // Rotation angle (-135° to +135° = 270° total)
    float rotaryStartAngle = juce::MathConstants<float>::pi * 1.2f;
    float rotaryEndAngle = juce::MathConstants<float>::pi * 2.8f;
    float angle = rotaryStartAngle + normalizedValue * (rotaryEndAngle - rotaryStartAngle);
    
    // === 3D KNOB RENDERING ===
    
    // 1. Outer shadow (depth)
    g.setColour(juce::Colours::black.withAlpha(0.5f));
    g.fillEllipse(centre.x - radius + 3, centre.y - radius + 3, radius * 2, radius * 2);
    
    // 2. Base knob body (dark metallic)
    juce::ColourGradient baseGradient(
        juce::Colour(0xff3a3a3a), centre.x - radius * 0.5f, centre.y - radius * 0.5f,
        juce::Colour(0xff1a1a1a), centre.x + radius * 0.5f, centre.y + radius * 0.5f,
        true
    );
    g.setGradientFill(baseGradient);
    g.fillEllipse(centre.x - radius, centre.y - radius, radius * 2, radius * 2);
    
    // 3. Top highlight (3D lighting effect)
    juce::ColourGradient topLight(
        juce::Colours::white.withAlpha(0.3f), centre.x, centre.y - radius * 0.7f,
        juce::Colours::transparentBlack, centre.x, centre.y,
        false
    );
    g.setGradientFill(topLight);
    g.fillEllipse(centre.x - radius * 0.8f, centre.y - radius * 0.8f, radius * 1.6f, radius * 1.6f);
    
    // 4. Inner ring (detail)
    g.setColour(juce::Colour(0xff2a2a2a));
    g.fillEllipse(centre.x - radius * 0.7f, centre.y - radius * 0.7f, radius * 1.4f, radius * 1.4f);
    
    // 5. Center cap (gold for main knob, silver for others)
    float capRadius = radius * 0.5f;
    juce::ColourGradient capGradient(
        isMainKnob ? juce::Colour(0xfff4d03f) : juce::Colour(0xffcccccc),
        centre.x - capRadius * 0.3f, centre.y - capRadius * 0.3f,
        isMainKnob ? juce::Colour(0xffc19a2e) : juce::Colour(0xff888888),
        centre.x + capRadius * 0.3f, centre.y + capRadius * 0.3f,
        true
    );
    g.setGradientFill(capGradient);
    g.fillEllipse(centre.x - capRadius, centre.y - capRadius, capRadius * 2, capRadius * 2);
    
    // 6. Pointer/indicator line
    juce::Path pointer;
    float pointerLength = radius * 0.6f;
    float pointerThickness = isMainKnob ? 4.0f : 3.0f;
    
    pointer.addRectangle(-pointerThickness * 0.5f, -pointerLength, pointerThickness, pointerLength * 0.7f);
    pointer.applyTransform(juce::AffineTransform::rotation(angle).translated(centre.x, centre.y));
    
    g.setColour(isMainKnob ? juce::Colour(0xffa855f7) : juce::Colours::white);
    g.fillPath(pointer);
    
    // 7. Value arc (progress indicator)
    juce::Path valueArc;
    valueArc.addCentredArc(centre.x, centre.y, radius * 0.85f, radius * 0.85f, 
                           0.0f, rotaryStartAngle, angle, true);
    
    g.setColour(isMainKnob ? juce::Colour(0xffa855f7) : juce::Colour(0xffd4af37));
    g.strokePath(valueArc, juce::PathStrokeType(3.0f));
    
    // 8. Track arc (background)
    juce::Path trackArc;
    trackArc.addCentredArc(centre.x, centre.y, radius * 0.85f, radius * 0.85f,
                           0.0f, rotaryStartAngle, rotaryEndAngle, true);
    
    g.setColour(juce::Colour(0xff444444));
    g.strokePath(trackArc, juce::PathStrokeType(2.0f));
}

void HaloKnob::resized()
{
    auto bounds = getLocalBounds();
    
    // Label at bottom
    label.setBounds(bounds.removeFromBottom(25));
    
    // Slider takes remaining space
    slider.setBounds(bounds);
}
