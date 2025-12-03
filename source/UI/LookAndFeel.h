#pragma once
#include <JuceHeader.h>

class AuricHaloLookAndFeel : public juce::LookAndFeel_V4
{
public:
    AuricHaloLookAndFeel();
    
    // --- Slider Override ---
    void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height,
                          float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                          juce::Slider&) override;
    
    // --- Button Overrides ---
    void drawButtonBackground(juce::Graphics&, juce::Button&, const juce::Colour& bgColour,
                            bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    
    void drawButtonText(juce::Graphics&, juce::TextButton&,
                        bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

private:
    // --- Palet Warna "Auric Halo" ---
    // Disimpan sebagai member variable agar konsisten dan mudah diedit
    
    // Warna Utama (Glow & Aksen)
    const juce::Colour colorPurpleGlow     { 0xffa855f7 };
    const juce::Colour colorOrangeHot      { 0xffff9d5c }; // Untuk ujung value arc
    
    // Warna Emas (Metallic Ring)
    const juce::Colour colorGoldHighlight  { 0xfffff4d0 }; // Specular
    const juce::Colour colorGoldMain       { 0xfff4d03f }; // Base gold
    const juce::Colour colorGoldShadow     { 0xff8a6e2f }; // Darker gold
    
    // Warna Permukaan & Background
    const juce::Colour colorDarkSurface    { 0xff2a2a2a }; // Top lighting of black surface
    const juce::Colour colorDeepAbyss      { 0xff050505 }; // Deepest black
    const juce::Colour colorShadow         { 0xcc000000 }; // Alpha black for shadows
};