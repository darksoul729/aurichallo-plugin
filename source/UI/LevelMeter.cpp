#include "LevelMeter.h"

LevelMeter::LevelMeter()
{
    setOpaque(false);
    startTimerHz(30);
}

LevelMeter::~LevelMeter()
{
    stopTimer();
}

void LevelMeter::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    
    // Background (dark metallic)
    g.setColour(juce::Colour(0xff1a1a1a));
    g.fillRoundedRectangle(bounds, 4.0f);
    
    // Border (gold untuk visibility)
    g.setColour(juce::Colour(0xffd4af37));
    g.drawRoundedRectangle(bounds, 4.0f, 2.0f);
    
    // Calculate meter fill
    float meterHeight = bounds.getHeight() - 4.0f;
    // fillHeight calculated but used in segment drawing below
    juce::ignoreUnused(displayLevel, meterHeight);
    
    // Draw segments
    float segmentHeight = meterHeight / numSegments;
    float segmentSpacing = 2.0f;
    
    for (int i = 0; i < numSegments; ++i)
    {
        float segmentY = bounds.getBottom() - 2.0f - (i + 1) * segmentHeight;
        float segmentFillHeight = segmentHeight - segmentSpacing;
        
        juce::Rectangle<float> segmentRect(
            bounds.getX() + 2.0f,
            segmentY + segmentSpacing / 2.0f,
            bounds.getWidth() - 4.0f,
            segmentFillHeight
        );
        
        // Check if this segment should be lit
        float segmentThreshold = (float)i / numSegments;
        
        if (displayLevel > segmentThreshold)
        {
            // Color gradient: green -> yellow -> red
            juce::Colour segmentColour;
            
            if (i < numSegments * 0.6f)
                segmentColour = juce::Colour(0xff00ff00); // Green
            else if (i < numSegments * 0.85f)
                segmentColour = juce::Colour(0xffffff00); // Yellow
            else
                segmentColour = juce::Colour(0xffff0000); // Red
            
            g.setColour(segmentColour);
            g.fillRoundedRectangle(segmentRect, 2.0f);
            
            // Glow effect
            g.setColour(segmentColour.withAlpha(0.3f));
            g.fillRoundedRectangle(segmentRect.expanded(1.0f), 2.0f);
        }
        else
        {
            // Unlit segment (dark)
            g.setColour(juce::Colour(0xff2a2a2a));
            g.fillRoundedRectangle(segmentRect, 2.0f);
        }
    }
    
    // Label
    g.setColour(juce::Colour(0xffd4af37));
    g.setFont(juce::FontOptions(10.0f, juce::Font::bold));
    g.drawText(isInputMeter ? "IN" : "OUT", 
               bounds.reduced(2.0f), 
               juce::Justification::centredBottom);
}

void LevelMeter::resized()
{
}

void LevelMeter::setLevel(float newLevel)
{
    currentLevel = juce::jlimit(0.0f, 1.0f, newLevel);
}

void LevelMeter::timerCallback()
{
    // Smooth decay
    if (currentLevel > displayLevel)
        displayLevel = currentLevel;
    else
        displayLevel *= decayRate;
    
    repaint();
}
