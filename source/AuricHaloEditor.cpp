#include "AuricHaloEditor.h"

AuricHaloEditor::AuricHaloEditor(AuricHaloProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    // Terapkan LookAndFeel custom
    setLookAndFeel(&lookAndFeel);
    
    // --- Setup Components ---
    
    // 1. Tone Knob (Kiri Atas)
    addAndMakeVisible(toneKnob);
    toneKnob.setLabel("TONE");
    toneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processor.getParameters().getAPVTS(), "tone", toneKnob.getSlider());

    // 2. Drive Knob (Tengah - Main)
    addAndMakeVisible(driveKnob);
    driveKnob.setLabel("DRIVE");
    driveKnob.setIsMainKnob(true); 
    driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processor.getParameters().getAPVTS(), "drive", driveKnob.getSlider());

    // 3. Mix Knob (Kanan Atas)
    addAndMakeVisible(mixKnob);
    mixKnob.setLabel("MIX");
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processor.getParameters().getAPVTS(), "mix", mixKnob.getSlider());

    // 4. Output Knob (Kanan Bawah)
    addAndMakeVisible(outputKnob);
    outputKnob.setLabel("OUTPUT");
    outputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processor.getParameters().getAPVTS(), "output", outputKnob.getSlider());

    // 5. Halo Engine Button (Switch)
    addAndMakeVisible(haloEngineButton);
    haloEngineButton.setButtonText("OFF");
    haloEngineButton.setClickingTogglesState(true);
    
    // Mengambil state awal
    haloEngineOn = false; 
    
    haloEngineButton.onClick = [this]() {
        haloEngineOn = haloEngineButton.getToggleState();
        haloEngineButton.setButtonText(haloEngineOn ? "ON" : "OFF");
        
        // Trigger repaint area tengah saat switch ditekan
        repaint(driveKnob.getBounds().expanded(60));
    };

    // --- Window Setup ---
    setSize(700, 550); 
    startTimerHz(30);  
}

AuricHaloEditor::~AuricHaloEditor()
{
    stopTimer();
    setLookAndFeel(nullptr);
}

void AuricHaloEditor::timerCallback()
{
    // Optimisasi Repaint: Hanya area glow
    if (haloEngineOn)
    {
        auto glowArea = driveKnob.getBounds().expanded(60);
        repaint(glowArea);
    }
    else
    {
        // Bersihkan sisa glow jika baru saja dimatikan
        static bool lastState = false;
        if (lastState != haloEngineOn) {
             repaint(driveKnob.getBounds().expanded(60));
        }
        lastState = haloEngineOn;
    }
}

void AuricHaloEditor::drawHaloGlow(juce::Graphics& g, juce::Rectangle<float> bounds, float intensity)
{
    auto centre = bounds.getCentre();
    auto radius = bounds.getWidth() / 2.0f; 
    
    // 1. Core Purple Glow (Intensitas Utama)
    if (intensity > 0.01f)
    {
        juce::ColourGradient glowGradient(
            juce::Colour(0xffa855f7).withAlpha(intensity * 0.7f), centre.x, centre.y,
            juce::Colours::transparentBlack, centre.x, centre.y + (radius * 1.5f), 
            true);
            
        g.setGradientFill(glowGradient);
        g.fillEllipse(centre.x - radius * 1.5f, centre.y - radius * 1.5f, 
                      radius * 3.0f, radius * 3.0f);
                      
        // 2. Hot Orange Center (Saat drive tinggi)
        if (intensity > 0.6f)
        {
            float hotIntensity = (intensity - 0.6f) * 2.5f; 
            float coreRadius = radius * 1.2f;
            
            juce::ColourGradient hotGradient(
                juce::Colour(0xffff9d5c).withAlpha(hotIntensity * 0.5f), centre.x, centre.y,
                juce::Colours::transparentBlack, centre.x, centre.y + coreRadius, 
                true);
                
            g.setGradientFill(hotGradient);
            g.fillEllipse(centre.x - coreRadius, centre.y - coreRadius, 
                          coreRadius * 2.0f, coreRadius * 2.0f);
        }
    }
}

void AuricHaloEditor::paint(juce::Graphics& g)
{
    auto w = static_cast<float>(getWidth());
    auto h = static_cast<float>(getHeight());
    
    // -------------------------------------------------------------------------
    // 1. LAYER DASAR: DARK TABLE TEXTURE
    // -------------------------------------------------------------------------
    // Background paling belakang (meja/alas)
    juce::ColourGradient bgGrad(juce::Colour(0xff111111), 0, 0, juce::Colour(0xff050505), 0, h, false);
    g.setGradientFill(bgGrad);
    g.fillAll();

    // -------------------------------------------------------------------------
    // 2. 3D CARD / CHASSIS (Panel Utama)
    // -------------------------------------------------------------------------
    // Area panel utama (sedikit lebih kecil dari window)
    auto cardBounds = getLocalBounds().toFloat().reduced(20.0f); 
    
    // Shadow Drop untuk Card (Agar terlihat melayang/tebal)
    {
        juce::Path p;
        p.addRoundedRectangle(cardBounds, 15.0f);
        juce::DropShadow(juce::Colours::black.withAlpha(0.8f), 20, {0, 10}).drawForPath(g, p);
    }

    // Isi Card dengan warna Dark Metalic Gradient
    juce::ColourGradient cardGrad(
        juce::Colour(0xff2a2a2a), cardBounds.getX(), cardBounds.getY(), // Kiri Atas lebih terang (cahaya)
        juce::Colour(0xff151515), cardBounds.getRight(), cardBounds.getBottom(), // Kanan Bawah gelap
        false);
    g.setGradientFill(cardGrad);
    g.fillRoundedRectangle(cardBounds, 15.0f);

    // Efek Bevel (Pinggiran) pada Card
    // Highlight putih tipis di sisi atas/kiri
    g.setColour(juce::Colours::white.withAlpha(0.1f));
    g.drawRoundedRectangle(cardBounds, 15.0f, 1.0f);
    
    // Shadow tebal di sisi bawah/kanan untuk kesan ketebalan plat
    g.setColour(juce::Colours::black); 
    g.drawLine(cardBounds.getX() + 10, cardBounds.getBottom(), cardBounds.getRight() - 10, cardBounds.getBottom(), 3.0f);

    // -------------------------------------------------------------------------
    // 3. SCREWS (Baut di 4 pojok - Estetika Industrial)
    // -------------------------------------------------------------------------
    auto drawScrew = [&](float x, float y) {
        float size = 12.0f;
        juce::Rectangle<float> screwRect(x - size/2, y - size/2, size, size);
        
        // Lubang baut
        g.setColour(juce::Colours::black);
        g.fillEllipse(screwRect);
        
        // Kepala baut (Silver Gradient)
        juce::ColourGradient silver(juce::Colours::lightgrey, x-5, y-5, juce::Colours::grey, x+5, y+5, false);
        g.setGradientFill(silver);
        g.fillEllipse(screwRect.reduced(2));
        
        // Garis obeng (+)
        g.setColour(juce::Colours::darkgrey);
        g.drawLine(x - 3, y, x + 3, y, 2.0f);
        g.drawLine(x, y - 3, x, y + 3, 2.0f);
    };

    drawScrew(cardBounds.getX() + 20, cardBounds.getY() + 20); // Kiri Atas
    drawScrew(cardBounds.getRight() - 20, cardBounds.getY() + 20); // Kanan Atas
    drawScrew(cardBounds.getX() + 20, cardBounds.getBottom() - 20); // Kiri Bawah
    drawScrew(cardBounds.getRight() - 20, cardBounds.getBottom() - 20); // Kanan Bawah

    // -------------------------------------------------------------------------
    // 4. HALO GLOW (Di belakang Drive Knob)
    // -------------------------------------------------------------------------
    if (haloEngineOn)
    {
        float driveVal = static_cast<float>(driveKnob.getSlider().getValue());
        float maxVal = static_cast<float>(driveKnob.getSlider().getMaximum());
        float minVal = static_cast<float>(driveKnob.getSlider().getMinimum());
        float intensity = (driveVal - minVal) / (maxVal - minVal);
        
        // Gambar glow di atas Card, tapi di bawah Knob (karena urutan render)
        drawHaloGlow(g, driveKnob.getBounds().toFloat(), intensity);
    }

    // -------------------------------------------------------------------------
    // 5. TEXT & TYPOGRAPHY
    // -------------------------------------------------------------------------
    auto center = cardBounds.getCentreX();

    // "AURIC" Title
    g.setFont(juce::FontOptions("Arial", 42.0f, juce::Font::bold));
    // Text Shadow (emboss effect)
    g.setColour(juce::Colours::black.withAlpha(0.7f));
    g.drawText("AURIC", 0, 53, getWidth(), 50, juce::Justification::centred); 
    // Main Gold Gradient Text
    juce::ColourGradient goldTextGrad(juce::Colour(0xfff4d03f), center, 40.0f, juce::Colour(0xffc19a2e), center, 90.0f, false);
    g.setGradientFill(goldTextGrad);
    g.drawText("AURIC", 0, 50, getWidth(), 50, juce::Justification::centred);

    // "HALO" Title
    g.setFont(juce::FontOptions("Arial", 58.0f, juce::Font::bold));
    g.setColour(juce::Colours::black.withAlpha(0.7f));
    g.drawText("HALO", 0, 93, getWidth(), 60, juce::Justification::centred);
    
    juce::ColourGradient purpleTextGrad(juce::Colour(0xffe9d5ff), center, 90.0f, juce::Colour(0xffa855f7), center, 150.0f, false);
    g.setGradientFill(purpleTextGrad);
    g.drawText("HALO", 0, 90, getWidth(), 60, juce::Justification::centred);

    // Subtitle & Decoration Line
    g.setColour(juce::Colour(0xffa855f7).withAlpha(0.8f));
    g.setFont(juce::FontOptions("Arial", 14.0f, juce::Font::bold));
    g.drawText("HARMONIC SATURATOR", 0, 145, getWidth(), 20, juce::Justification::centred);

    g.setColour(juce::Colour(0xff444444));
    g.fillRect(center - 80, 170.0f, 160.0f, 2.0f); // Garis dekorasi

    // Label Halo Engine (Plate Background)
    auto btnBounds = haloEngineButton.getBounds();
    // Background label kecil (plate hitam)
    juce::Rectangle<float> labelPlate((float)btnBounds.getX() - 30, (float)btnBounds.getY() - 25, (float)btnBounds.getWidth() + 60, 20.0f);
    
    g.setColour(juce::Colour(0xff111111));
    g.fillRoundedRectangle(labelPlate, 4.0f);
    g.setColour(juce::Colours::grey.withAlpha(0.5f)); // Border tipis plate
    g.drawRoundedRectangle(labelPlate, 4.0f, 1.0f);
    
    g.setColour(juce::Colour(0xffd4af37));
    g.setFont(juce::FontOptions("Arial", 11.0f, juce::Font::bold));
    g.drawText("HALO ENGINE", labelPlate, juce::Justification::centred);
}

void AuricHaloEditor::resized()
{
    auto bounds = getLocalBounds();
    auto center = bounds.getCentre();
    
    // Ukuran knob
    const int mainKnobSize = 220;
    const int smallKnobSize = 110;
    
    // 1. Main Knob (Tengah Mutlak)
    driveKnob.setBounds(center.x - mainKnobSize/2, center.y - 30, mainKnobSize, mainKnobSize);
    
    // 2. Knob Samping (Relatif terhadap Main Knob)
    // Y position sejajar
    int smallKnobY = driveKnob.getY() + 10;
    int offsetX = 180; // Jarak horizontal dari tengah
    
    toneKnob.setBounds(center.x - offsetX - smallKnobSize, smallKnobY, smallKnobSize, smallKnobSize);
    mixKnob.setBounds(center.x + offsetX, smallKnobY, smallKnobSize, smallKnobSize);
    
    // 3. Output Knob (Di bawah Mix Knob)
    outputKnob.setBounds(mixKnob.getX(), mixKnob.getBottom() + 30, smallKnobSize, smallKnobSize);
    
    // 4. Halo Engine Switch (Di bawah Tone Knob)
    haloEngineButton.setBounds(toneKnob.getX() + 25, toneKnob.getBottom() + 50, 60, 40);
}