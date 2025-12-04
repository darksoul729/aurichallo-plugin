#include "LookAndFeel.h" 

AuricHaloLookAndFeel::AuricHaloLookAndFeel()
{
}

void AuricHaloLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                            float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                                            juce::Slider& slider)
{
    juce::ignoreUnused(slider);

    auto bounds = juce::Rectangle<float>((float)x, (float)y, (float)width, (float)height).reduced(10.0f);
    auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto centre = bounds.getCentre();

    // ================================================================
    // 1. SOCKET (Lubang Cekung di Panel)
    // ================================================================
    // Ini membuat ilusi bahwa knob "tertanam" di dalam panel
    juce::Path socketParams;
    float socketRadius = radius * 1.1f;
    socketParams.addEllipse(centre.x - socketRadius, centre.y - socketRadius, socketRadius * 2.0f, socketRadius * 2.0f);
    
    // Bayangan dalam (Inner Shadow untuk lubang) - Digambar manual dengan gradien
    juce::ColourGradient socketGrad(
        juce::Colour(0xff000000), centre.x, centre.y - socketRadius,
        juce::Colour(0xff252525), centre.x, centre.y + socketRadius, false);
    g.setGradientFill(socketGrad);
    g.fillPath(socketParams);

    // ================================================================
    // 2. THE HALO (GLOW)
    // ================================================================
    if (sliderPos > 0.01f) // Hanya gambar glow jika ada nilai
    {
        juce::ColourGradient haloGradient(
            colorPurpleGlow.withAlpha(0.5f + (sliderPos * 0.3f)), centre.x, centre.y,
            juce::Colours::transparentBlack, centre.x, centre.y + (radius * 1.4f),
            true);

        g.setGradientFill(haloGradient);
        g.fillEllipse(centre.x - radius * 1.4f, centre.y - radius * 1.4f, 
                      radius * 2.8f, radius * 2.8f);
    }

    // ================================================================
    // 3. KNOB SHADOW (Bayangan Knob ke Panel)
    // ================================================================
    // Bayangan jauh (Soft)
    juce::Path knobPath;
    knobPath.addEllipse(centre.x - radius, centre.y - radius, radius * 2.0f, radius * 2.0f);
    juce::DropShadow(juce::Colours::black.withAlpha(0.9f), 15, {0, 8}).drawForPath(g, knobPath);
    
    // Bayangan dekat (Hard - Contact Shadow)
    juce::DropShadow(juce::Colours::black, 2, {0, 2}).drawForPath(g, knobPath);

    // ================================================================
    // 4. OUTER RING (METALIC GOLD - 3D BEVEL)
    // ================================================================
    // Base Gold Gradient
    juce::ColourGradient goldGradient(
        colorGoldMain, centre.x - radius, centre.y - radius,
        colorGoldShadow, centre.x + radius, centre.y + radius, false);
    
    // Tambahkan titik terang untuk simulasi logam
    goldGradient.addColour(0.3f, colorGoldHighlight); 
    goldGradient.addColour(0.7f, colorGoldShadow);

    g.setGradientFill(goldGradient);
    g.fillEllipse(centre.x - radius, centre.y - radius, radius * 2.0f, radius * 2.0f);

    // RING BEVEL (Sisi tajam ring)
    // Sisi atas terang (pantulan cahaya)
    g.setColour(juce::Colours::white.withAlpha(0.6f));
    g.drawEllipse(centre.x - radius + 1, centre.y - radius + 1, (radius - 1) * 2.0f, (radius - 1) * 2.0f, 1.0f);
    // Sisi bawah gelap
    g.setColour(juce::Colours::black.withAlpha(0.6f));
    g.drawEllipse(centre.x - radius, centre.y - radius, radius * 2.0f, radius * 2.0f, 1.5f);

    // ================================================================
    // 5. MAIN KNOB BODY (DOME SHAPE)
    // ================================================================
    float innerRadius = radius * 0.88f;
    
    // Gradien radial yang digeser ke atas untuk efek cembung (dome)
    juce::ColourGradient knobBodyGradient(
        juce::Colour(0xff3a3a3a), centre.x, centre.y - innerRadius * 0.7f, // Titik terang di atas
        colorDeepAbyss, centre.x, centre.y + innerRadius, 
        true); // Radial

    g.setGradientFill(knobBodyGradient);
    g.fillEllipse(centre.x - innerRadius, centre.y - innerRadius, innerRadius * 2.0f, innerRadius * 2.0f);

    // Tekstur halus pada body (opsional, membuat tidak terlalu plastik)
    g.setColour(juce::Colours::black.withAlpha(0.2f));
    g.drawEllipse(centre.x - innerRadius * 0.7f, centre.y - innerRadius * 0.7f, 
                 innerRadius * 1.4f, innerRadius * 1.4f, innerRadius * 0.4f);

    // ================================================================
    // 6. INDICATORS (VALUE ARC & POINTER)
    // ================================================================
    // Track (Parit gelap)
    juce::Path trackArc;
    float arcRadius = innerRadius * 0.82f;
    trackArc.addCentredArc(centre.x, centre.y, arcRadius, arcRadius, 0.0f,
                           rotaryStartAngle, rotaryEndAngle, true);
    
    // Efek parit terukir (Emboss ke dalam)
    g.setColour(juce::Colours::white.withAlpha(0.1f)); // Highlight bawah parit
    g.strokePath(trackArc, juce::PathStrokeType(4.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    g.setColour(juce::Colours::black.withAlpha(0.8f)); // Shadow atas parit
    g.strokePath(trackArc, juce::PathStrokeType(3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // Value Active
    if (sliderPos > 0.0f)
    {
        juce::Path valueArc;
        valueArc.addCentredArc(centre.x, centre.y, arcRadius, arcRadius, 0.0f,
                               rotaryStartAngle, toAngle, true);
        
        juce::ColourGradient valueGradient(
            colorPurpleGlow, centre.x - radius, centre.y,
            colorOrangeHot, centre.x + radius, centre.y, false);
        g.setGradientFill(valueGradient);
        g.strokePath(valueArc, juce::PathStrokeType(3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // Neon Glow
        g.setColour(colorPurpleGlow.withAlpha(0.5f));
        g.strokePath(valueArc, juce::PathStrokeType(8.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    }

    // Pointer 3D
    juce::Path pointerPath;
    float pointerLength = innerRadius * 0.45f;
    float pointerThickness = 5.0f;
    
    pointerPath.addRoundedRectangle(-pointerThickness * 0.5f, -arcRadius + 8, pointerThickness, pointerLength, 2.0f);
    pointerPath.applyTransform(juce::AffineTransform::rotation(toAngle).translated(centre.x, centre.y));

    juce::DropShadow(juce::Colours::black, 3, {1, 1}).drawForPath(g, pointerPath); // Shadow jarum
    
    // Jarum Emas
    juce::ColourGradient pointerGrad(colorGoldHighlight, 0, 0, colorGoldMain, 0, 10, false);
    g.setGradientFill(pointerGrad);
    g.fillPath(pointerPath);
}

void AuricHaloLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button,
                                                const juce::Colour& bgColour,
                                                bool shouldDrawButtonAsHighlighted,
                                                bool shouldDrawButtonAsDown)
{
    juce::ignoreUnused(bgColour, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

    auto bounds = button.getLocalBounds().toFloat().reduced(2.0f);
    auto isOn = button.getToggleState();
    
    // Glow jika ON
    if (isOn)
    {
        g.setColour(colorPurpleGlow.withAlpha(0.4f));
        g.fillRoundedRectangle(bounds.expanded(4), 6.0f);
    }

    // 1. Tombol 3D Shape (Bevel Effect)
    // Background dasar
    g.setColour(isOn ? colorDarkSurface : juce::Colour(0xff1a1a1a));
    g.fillRoundedRectangle(bounds, 6.0f);

    // 2. Efek Timbul (Bevel)
    // Highlight di Kiri & Atas (Cahaya datang dari kiri atas)
    g.setColour(juce::Colours::white.withAlpha(0.2f));
    g.drawLine(bounds.getX(), bounds.getY(), bounds.getRight(), bounds.getY(), 2.0f); // Top
    g.drawLine(bounds.getX(), bounds.getY(), bounds.getX(), bounds.getBottom(), 2.0f); // Left

    // Shadow di Kanan & Bawah
    g.setColour(juce::Colours::black.withAlpha(0.8f));
    g.drawLine(bounds.getRight(), bounds.getY(), bounds.getRight(), bounds.getBottom(), 2.0f); // Right
    g.drawLine(bounds.getX(), bounds.getBottom(), bounds.getRight(), bounds.getBottom(), 2.0f); // Bottom

    // 3. Gold Frame border
    g.setColour(isOn ? colorGoldHighlight : colorGoldShadow);
    g.drawRoundedRectangle(bounds.reduced(2), 6.0f, 1.0f);
}

void AuricHaloLookAndFeel::drawButtonText(juce::Graphics& g, juce::TextButton& button,
                                          bool shouldDrawButtonAsHighlighted,
                                          bool shouldDrawButtonAsDown)
{
    juce::ignoreUnused(shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
    
    // Text Shadow (Emboss effect)
    g.setColour(juce::Colours::black.withAlpha(0.8f));
    g.setFont(juce::FontOptions("Arial", 14.0f, juce::Font::bold));
    auto textBounds = button.getLocalBounds().toFloat();
    g.drawText(button.getButtonText(), textBounds.translated(0, 1), juce::Justification::centred, true);

    // Main Text
    g.setColour(button.getToggleState() ? juce::Colour(0xffffffff) : colorGoldMain);
    g.drawText(button.getButtonText(), textBounds, juce::Justification::centred, true);
}

void AuricHaloLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                                           float sliderPos, float minSliderPos, float maxSliderPos,
                                           const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    juce::ignoreUnused(minSliderPos, maxSliderPos, style);
    
    auto bounds = juce::Rectangle<float>((float)x, (float)y, (float)width, (float)height);
    auto trackWidth = juce::jmin(20.0f, bounds.getWidth() * 0.5f);
    auto trackBounds = bounds.withSizeKeepingCentre(trackWidth, bounds.getHeight());
    
    // === TRACK (Groove) ===
    // Shadow untuk depth
    g.setColour(juce::Colours::black.withAlpha(0.8f));
    g.fillRoundedRectangle(trackBounds.translated(1, 1), trackWidth * 0.5f);
    
    // Track body (dark groove)
    juce::ColourGradient trackGrad(
        juce::Colour(0xff1a1a1a), trackBounds.getX(), trackBounds.getY(),
        juce::Colour(0xff2a2a2a), trackBounds.getRight(), trackBounds.getY(),
        false
    );
    g.setGradientFill(trackGrad);
    g.fillRoundedRectangle(trackBounds, trackWidth * 0.5f);
    
    // Inner shadow (embossed groove)
    g.setColour(juce::Colours::black.withAlpha(0.6f));
    g.drawRoundedRectangle(trackBounds.reduced(1), trackWidth * 0.5f - 1, 1.0f);
    
    // === VALUE FILL (Active portion) ===
    float normalizedValue = (float)slider.getValue() / (float)slider.getMaximum();
    float fillHeight = trackBounds.getHeight() * (1.0f - normalizedValue);
    auto fillBounds = trackBounds.removeFromBottom(trackBounds.getHeight() - fillHeight);
    
    // Gradient from purple to gold
    juce::ColourGradient fillGrad(
        colorPurpleGlow, fillBounds.getCentreX(), fillBounds.getBottom(),
        colorGoldMain, fillBounds.getCentreX(), fillBounds.getY(),
        false
    );
    g.setGradientFill(fillGrad);
    g.fillRoundedRectangle(fillBounds.reduced(2), (trackWidth - 4) * 0.5f);
    
    // Glow effect
    g.setColour(colorPurpleGlow.withAlpha(0.3f));
    g.fillRoundedRectangle(fillBounds.reduced(1), (trackWidth - 2) * 0.5f);
    
    // === THUMB (Slider handle) ===
    auto thumbWidth = trackWidth * 1.5f;
    auto thumbHeight = 12.0f;
    auto thumbY = sliderPos - thumbHeight * 0.5f;
    auto thumbBounds = juce::Rectangle<float>(
        trackBounds.getCentreX() - thumbWidth * 0.5f,
        thumbY,
        thumbWidth,
        thumbHeight
    );
    
    // Thumb shadow
    g.setColour(juce::Colours::black.withAlpha(0.7f));
    g.fillRoundedRectangle(thumbBounds.translated(0, 2), 3.0f);
    
    // Thumb body (gold gradient)
    juce::ColourGradient thumbGrad(
        colorGoldHighlight, thumbBounds.getCentreX(), thumbBounds.getY(),
        colorGoldShadow, thumbBounds.getCentreX(), thumbBounds.getBottom(),
        false
    );
    g.setGradientFill(thumbGrad);
    g.fillRoundedRectangle(thumbBounds, 3.0f);
    
    // Thumb highlight
    g.setColour(juce::Colours::white.withAlpha(0.4f));
    g.fillRoundedRectangle(thumbBounds.reduced(2, 1).removeFromTop(3), 2.0f);
    
    // Thumb border
    g.setColour(juce::Colours::black.withAlpha(0.5f));
    g.drawRoundedRectangle(thumbBounds, 3.0f, 1.0f);
}