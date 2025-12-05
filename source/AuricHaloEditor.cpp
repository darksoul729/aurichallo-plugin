#include "AuricHaloEditor.h"

AuricHaloEditor::AuricHaloEditor(AuricHaloProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    // Terapkan LookAndFeel custom
    setLookAndFeel(&lookAndFeel);
    
    // --- Setup Components ---
    
    // 1. Input Slider (Kiri - Vertical)
    addAndMakeVisible(inputSlider);
    inputSlider.setLabel("INPUT");
    inputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processor.getParameters().getAPVTS(), "input", inputSlider.getSlider());
    
    // 2. Output Slider (Kanan - Vertical)
    addAndMakeVisible(outputSlider);
    outputSlider.setLabel("OUTPUT");
    outputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processor.getParameters().getAPVTS(), "output", outputSlider.getSlider());
    
    // 3. Tone Knob (Kiri Bawah)
    addAndMakeVisible(toneKnob);
    toneKnob.setLabel("TONE");
    toneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processor.getParameters().getAPVTS(), "tone", toneKnob.getSlider());

    // 4. Drive Knob (Tengah - Main)
    addAndMakeVisible(driveKnob);
    driveKnob.setLabel("DRIVE");
    driveKnob.setIsMainKnob(true); 
    driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processor.getParameters().getAPVTS(), "drive", driveKnob.getSlider());

    // 5. Mix Knob (Kanan Bawah)
    addAndMakeVisible(mixKnob);
    mixKnob.setLabel("MIX");
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processor.getParameters().getAPVTS(), "mix", mixKnob.getSlider());

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
    
    // 6. License Button
    addAndMakeVisible(licenseButton);
    licenseButton.setButtonText("License");
    licenseButton.onClick = [this]() {
        showLicenseDialog();
    };
    
    // 7. Level Meters
    addAndMakeVisible(inputMeter);
    inputMeter.setIsInput(true);
    inputMeter.setAlwaysOnTop(true);
    
    addAndMakeVisible(outputMeter);
    outputMeter.setIsInput(false);
    outputMeter.setAlwaysOnTop(true);
    
    // 8. Oversampling Selector
    addAndMakeVisible(oversamplingLabel);
    oversamplingLabel.setText("Oversampling", juce::dontSendNotification);
    oversamplingLabel.setJustificationType(juce::Justification::centred);
    oversamplingLabel.setColour(juce::Label::textColourId, juce::Colour(0xffd4af37));
    oversamplingLabel.setFont(juce::FontOptions(12.0f, juce::Font::bold));
    oversamplingLabel.setAlwaysOnTop(true);
    
    addAndMakeVisible(oversamplingBox);
    oversamplingBox.addItem("Off", 1);
    oversamplingBox.addItem("2x", 2);
    oversamplingBox.addItem("4x", 3);
    oversamplingBox.setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xff2a2a2a));
    oversamplingBox.setColour(juce::ComboBox::textColourId, juce::Colours::white);
    oversamplingBox.setColour(juce::ComboBox::outlineColourId, juce::Colour(0xffd4af37));
    oversamplingBox.setColour(juce::ComboBox::arrowColourId, juce::Colour(0xffd4af37));
    oversamplingBox.setAlwaysOnTop(true);
    oversamplingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        processor.getParameters().getAPVTS(), "oversampling", oversamplingBox);
    
    // 9. Size Selector
    addAndMakeVisible(sizeLabel);
    sizeLabel.setText("UI Size", juce::dontSendNotification);
    sizeLabel.setJustificationType(juce::Justification::centred);
    sizeLabel.setColour(juce::Label::textColourId, juce::Colour(0xffd4af37));
    sizeLabel.setFont(juce::FontOptions(12.0f, juce::Font::bold));
    sizeLabel.setAlwaysOnTop(true);
    
    addAndMakeVisible(sizeSelector);
    sizeSelector.addItem("Small", 1);
    sizeSelector.addItem("Medium", 2);
    sizeSelector.addItem("Large", 3);
    sizeSelector.setSelectedId(2); // Medium by default
    sizeSelector.setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xff2a2a2a));
    sizeSelector.setColour(juce::ComboBox::textColourId, juce::Colours::white);
    sizeSelector.setColour(juce::ComboBox::outlineColourId, juce::Colour(0xffd4af37));
    sizeSelector.setColour(juce::ComboBox::arrowColourId, juce::Colour(0xffd4af37));
    sizeSelector.setAlwaysOnTop(true);
    sizeSelector.onChange = [this]() {
        int selectedId = sizeSelector.getSelectedId();
        if (selectedId == 1) setUISize(UISize::Small);
        else if (selectedId == 2) setUISize(UISize::Medium);
        else if (selectedId == 3) setUISize(UISize::Large);
    };

    // --- Window Setup ---
    setSize(700, 550); 
    startTimerHz(30);
    
    // Check license status on startup
    DBG("AuricHaloEditor: Constructor - checking license status");
    checkLicenseStatus();  
}

AuricHaloEditor::~AuricHaloEditor()
{
    stopTimer();
    setLookAndFeel(nullptr);
}

void AuricHaloEditor::timerCallback()
{
    // Update level meters
    inputMeter.setLevel(processor.getInputLevel());
    outputMeter.setLevel(processor.getOutputLevel());
    
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
    // Area panel utama (FULLSCREEN - no modal)
    auto cardBounds = getLocalBounds().toFloat().reduced(5.0f); 
    
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
    // 5. TEXT & TYPOGRAPHY (Responsive)
    // -------------------------------------------------------------------------
    auto center = cardBounds.getCentreX();
    
    // Calculate text scale
    float textScale = w / 700.0f;
    
    // "AURIC" Title
    float auricSize = 42.0f * textScale;
    g.setFont(juce::FontOptions("Arial", auricSize, juce::Font::bold));
    // Text Shadow (emboss effect)
    g.setColour(juce::Colours::black.withAlpha(0.7f));
    g.drawText("AURIC", 0, (int)(53 * textScale), getWidth(), (int)(50 * textScale), juce::Justification::centred); 
    // Main Gold Gradient Text
    juce::ColourGradient goldTextGrad(juce::Colour(0xfff4d03f), center, 40.0f * textScale, juce::Colour(0xffc19a2e), center, 90.0f * textScale, false);
    g.setGradientFill(goldTextGrad);
    g.drawText("AURIC", 0, (int)(50 * textScale), getWidth(), (int)(50 * textScale), juce::Justification::centred);

    // "HALO" Title
    float haloSize = 58.0f * textScale;
    g.setFont(juce::FontOptions("Arial", haloSize, juce::Font::bold));
    g.setColour(juce::Colours::black.withAlpha(0.7f));
    g.drawText("HALO", 0, (int)(93 * textScale), getWidth(), (int)(60 * textScale), juce::Justification::centred);
    
    juce::ColourGradient purpleTextGrad(juce::Colour(0xffe9d5ff), center, 90.0f * textScale, juce::Colour(0xffa855f7), center, 150.0f * textScale, false);
    g.setGradientFill(purpleTextGrad);
    g.drawText("HALO", 0, (int)(90 * textScale), getWidth(), (int)(60 * textScale), juce::Justification::centred);

    // Subtitle & Decoration Line
    g.setColour(juce::Colour(0xffa855f7).withAlpha(0.8f));
    float subtitleSize = 14.0f * textScale;
    g.setFont(juce::FontOptions("Arial", subtitleSize, juce::Font::bold));
    g.drawText("HARMONIC SATURATOR", 0, (int)(145 * textScale), getWidth(), (int)(20 * textScale), juce::Justification::centred);

    g.setColour(juce::Colour(0xff444444));
    g.fillRect(center - 80 * textScale, 170.0f * textScale, 160.0f * textScale, 2.0f); // Garis dekorasi

    // Label Halo Engine (Above button, not overlapping)
    auto btnBounds = haloEngineButton.getBounds();
    float labelSize = 10.0f * textScale;
    float labelY = (float)btnBounds.getY() - 18 * textScale;
    
    g.setColour(juce::Colour(0xffd4af37));
    g.setFont(juce::FontOptions("Arial", labelSize, juce::Font::bold));
    g.drawText("HALO ENGINE", 
               static_cast<int>((float)btnBounds.getX() - 20), 
               static_cast<int>(labelY), 
               static_cast<int>((float)btnBounds.getWidth() + 40), 
               static_cast<int>(15.0f * textScale), 
               juce::Justification::centred);
}

void AuricHaloEditor::resized()
{
    auto bounds = getLocalBounds();
    float w = (float)bounds.getWidth();
    float h = (float)bounds.getHeight();
    auto center = bounds.getCentre();
    
    // === RESPONSIVE SIZING ===
    // Base sizes (for Medium = 700x550)
    const float baseWidth = 700.0f;
    const float baseHeight = 550.0f;
    
    // Calculate actual scale from window size
    float widthScale = w / baseWidth;
    float heightScale = h / baseHeight;
    float actualScale = juce::jmin(widthScale, heightScale); // Use smaller to maintain aspect ratio
    
    // Component sizes (scaled)
    const int mainKnobSize = (int)(180 * actualScale);
    const int smallKnobSize = (int)(90 * actualScale);
    const int sliderWidth = (int)(40 * actualScale);
    const int sliderHeight = (int)(200 * actualScale);
    
    // Level meters (paling kiri dan kanan)
    const int meterWidth = (int)(22 * actualScale);
    const int meterHeight = (int)(260 * actualScale);
    const int meterMargin = (int)(12 * actualScale);
    
    inputMeter.setBounds(meterMargin, center.y - meterHeight/2, meterWidth, meterHeight);
    outputMeter.setBounds((int)w - meterMargin - meterWidth, center.y - meterHeight/2, meterWidth, meterHeight);
    
    // === LAYOUT AREAS ===
    int titleHeight = (int)(170 * actualScale);
    int mainAreaY = titleHeight;
    int mainAreaHeight = (int)(300 * actualScale);
    
    // === MAIN CONTROLS (Centered Grid) ===
    
    // 1. DRIVE KNOB (Center)
    int driveY = mainAreaY + (mainAreaHeight - mainKnobSize) / 2;
    driveKnob.setBounds(center.x - mainKnobSize/2, driveY, mainKnobSize, mainKnobSize);
    
    // 2. INPUT SLIDER (Left of Drive, aligned with Drive center)
    int sliderOffsetX = (int)(145 * actualScale);
    int sliderY = driveY + (mainKnobSize - sliderHeight) / 2; // Align with Drive center
    inputSlider.setBounds(center.x - sliderOffsetX - sliderWidth/2, sliderY, sliderWidth, sliderHeight);
    
    // 3. OUTPUT SLIDER (Right of Drive, aligned with Drive center)
    outputSlider.setBounds(center.x + sliderOffsetX - sliderWidth/2, sliderY, sliderWidth, sliderHeight);
    
    // 4. TONE KNOB (Left side, vertically centered with Drive)
    int sideKnobOffsetX = (int)(250 * actualScale);
    int sideKnobY = driveY + (mainKnobSize - smallKnobSize) / 2; // Vertically centered with Drive
    toneKnob.setBounds(center.x - sideKnobOffsetX - smallKnobSize/2, sideKnobY, smallKnobSize, smallKnobSize);
    
    // 5. MIX KNOB (Right side, vertically centered with Drive)
    mixKnob.setBounds(center.x + sideKnobOffsetX - smallKnobSize/2, sideKnobY, smallKnobSize, smallKnobSize);
    
    // 6. HALO ENGINE BUTTON (Below Tone, with space)
    int buttonWidth = (int)(90 * actualScale);
    int buttonHeight = (int)(30 * actualScale);
    int buttonY = toneKnob.getBottom() + (int)(25 * actualScale); // Space below Tone
    haloEngineButton.setBounds(
        toneKnob.getX() + (toneKnob.getWidth() - buttonWidth) / 2,
        buttonY,
        buttonWidth,
        buttonHeight
    );
    
    // === TOP BAR ===
    // 7. LICENSE BUTTON (Top right)
    int topMargin = (int)(10 * actualScale);
    int licenseWidth = (int)(90 * actualScale);
    int licenseHeight = (int)(28 * actualScale);
    licenseButton.setBounds((int)w - licenseWidth - topMargin, topMargin, licenseWidth, licenseHeight);
    
    // === BOTTOM BAR ===
    int bottomMargin = (int)(12 * actualScale);
    int comboWidth = (int)(110 * actualScale);
    int comboHeight = (int)(24 * actualScale);
    int labelHeight = (int)(18 * actualScale);
    int bottomY = (int)h - bottomMargin - comboHeight - labelHeight - 5;
    
    // 8. OVERSAMPLING (Bottom left)
    int leftX = (int)(50 * actualScale);
    oversamplingLabel.setBounds(leftX, bottomY, comboWidth, labelHeight);
    oversamplingBox.setBounds(leftX, bottomY + labelHeight + 3, comboWidth, comboHeight);
    
    // 9. SIZE SELECTOR (Bottom right)
    int rightX = (int)w - (int)(50 * actualScale) - comboWidth;
    sizeLabel.setBounds(rightX, bottomY, comboWidth, labelHeight);
    sizeSelector.setBounds(rightX, bottomY + labelHeight + 3, comboWidth, comboHeight);
}

void AuricHaloEditor::checkLicenseStatus()
{
    auto& licenseManager = processor.getLicenseManager();
    
    // Update license button text
    auto status = licenseManager.getStatus();
    DBG("checkLicenseStatus: status = " + juce::String((int)status));
    
    if (status == LicenseManager::Status::Licensed)
    {
        DBG("checkLicenseStatus: Plugin is licensed");
        licenseButton.setButtonText("Licensed");
        licenseButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green.darker());
    }
    else
    {
        DBG("checkLicenseStatus: Plugin is NOT licensed - will show dialog");
        licenseButton.setButtonText("Activate");
        licenseButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red.darker());
        
        // Show license dialog immediately if not licensed
        juce::Timer::callAfterDelay(500, [this]() {
            DBG("Timer callback: About to show license dialog");
            showLicenseDialog();
        });
    }
}

void AuricHaloEditor::showLicenseDialog()
{
    DBG("showLicenseDialog: Called");
    
    if (licenseDialog != nullptr)
    {
        DBG("showLicenseDialog: Dialog already showing, returning");
        return; // Already showing
    }
    
    DBG("showLicenseDialog: Creating new dialog");
    auto& licenseManager = processor.getLicenseManager();
    
    licenseDialog.reset(new LicenseDialog(licenseManager));
    DBG("showLicenseDialog: Dialog created");
    
    licenseDialog->onLicenseActivated = [this]()
    {
        DBG("License activated callback received");
        // Refresh license status
        juce::MessageManager::callAsync([this]()
        {
            checkLicenseStatus();
            licenseDialog.reset(); // Clean up dialog
        });
    };
    
    // Add as child component (overlay on top of editor)
    DBG("showLicenseDialog: Adding as child component");
    addAndMakeVisible(licenseDialog.get());
    licenseDialog->setBounds(getLocalBounds());
    licenseDialog->toFront(true);
    DBG("showLicenseDialog: Dialog should now be visible");
}

void AuricHaloEditor::setUISize(UISize newSize)
{
    currentSize = newSize;
    
    // Set scale factor based on size
    switch (currentSize)
    {
        case UISize::Small:
            scaleFactor = 0.75f;
            setSize(525, 412);  // 75% of 700x550
            break;
        case UISize::Medium:
            scaleFactor = 1.0f;
            setSize(700, 550);  // Default
            break;
        case UISize::Large:
            scaleFactor = 1.25f;
            setSize(875, 687);  // 125% of 700x550
            break;
    }
    
    updateSizeForCurrentScale();
}

void AuricHaloEditor::updateSizeForCurrentScale()
{
    resized();
    repaint();
}
