#include "LicenseDialog.h"

LicenseDialog::LicenseDialog(LicenseManager& lm)
    : licenseManager(lm)
{
    isLicensed = (licenseManager.getStatus() == LicenseManager::Status::Licensed);
    
    // Title
    titleLabel.setText(isLicensed ? "Auric Halo - Licensed" : "Auric Halo - License Activation", 
                       juce::dontSendNotification);
    titleLabel.setFont(juce::Font(24.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(titleLabel);
    
    // Status
    statusLabel.setText(licenseManager.getLicenseInfo(), juce::dontSendNotification);
    statusLabel.setFont(juce::Font(16.0f));
    statusLabel.setJustificationType(juce::Justification::centred);
    statusLabel.setColour(juce::Label::textColourId, 
                          isLicensed ? juce::Colours::lightgreen : juce::Colours::orange);
    addAndMakeVisible(statusLabel);
    
    if (!isLicensed)
    {
        // Instruction
        instructionLabel.setText("Enter your license key:", juce::dontSendNotification);
        instructionLabel.setFont(juce::Font(14.0f));
        addAndMakeVisible(instructionLabel);
        
        // License key input
        licenseKeyEditor.setMultiLine(false);
        licenseKeyEditor.setReturnKeyStartsNewLine(false);
        licenseKeyEditor.setTextToShowWhenEmpty("XXXX-XXXX-XXXX-XXXX", juce::Colours::grey);
        licenseKeyEditor.setFont(juce::Font(16.0f, juce::Font::bold));
        licenseKeyEditor.setJustification(juce::Justification::centred);
        licenseKeyEditor.addListener(this);
        addAndMakeVisible(licenseKeyEditor);
        
        // Activate button
        activateButton.setButtonText("Activate License");
        activateButton.addListener(this);
        addAndMakeVisible(activateButton);
        
        // Purchase button
        purchaseButton.setButtonText("Purchase License");
        purchaseButton.addListener(this);
        addAndMakeVisible(purchaseButton);
    }
    else
    {
        // License info for licensed users
        licenseInfoLabel.setText("Thank you for using Auric Halo!\n\n"
                                 "Your license is active and valid.",
                                 juce::dontSendNotification);
        licenseInfoLabel.setFont(juce::Font(14.0f));
        licenseInfoLabel.setJustificationType(juce::Justification::centred);
        licenseInfoLabel.setColour(juce::Label::textColourId, juce::Colours::white);
        addAndMakeVisible(licenseInfoLabel);
        
        // Credits label
        creditsLabel.setText("Credits:\n"
                             "Arie Wardhana\n"
                             "Raditya Rakha Wardhana\n"
                             "Kevin Hermansyah",
                             juce::dontSendNotification);
        creditsLabel.setFont(juce::Font(12.0f));
        creditsLabel.setJustificationType(juce::Justification::centred);
        creditsLabel.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
        addAndMakeVisible(creditsLabel);
        
        // Close button
        closeButton.setButtonText("Close");
        closeButton.addListener(this);
        addAndMakeVisible(closeButton);
    }
    
    setSize(500, isLicensed ? 350 : 350);
}

LicenseDialog::~LicenseDialog()
{
}

void LicenseDialog::paint(juce::Graphics& g)
{
    // Semi-transparent overlay background
    g.fillAll(juce::Colours::black.withAlpha(0.8f));
    
    // Dialog box in center
    auto dialogBounds = getLocalBounds().reduced(getWidth() / 4, getHeight() / 6);
    
    // Dialog background
    g.setColour(juce::Colour(0xff1a1a1a));
    g.fillRoundedRectangle(dialogBounds.toFloat(), 10.0f);
    
    // Draw border
    g.setColour(juce::Colours::white.withAlpha(0.3f));
    g.drawRoundedRectangle(dialogBounds.toFloat(), 10.0f, 2.0f);
}

void LicenseDialog::resized()
{
    // Center dialog box
    auto dialogBounds = getLocalBounds().reduced(getWidth() / 4, getHeight() / 6);
    auto bounds = dialogBounds.reduced(30);
    
    titleLabel.setBounds(bounds.removeFromTop(40));
    bounds.removeFromTop(10);
    
    statusLabel.setBounds(bounds.removeFromTop(30));
    bounds.removeFromTop(30);
    
    if (!isLicensed)
    {
        instructionLabel.setBounds(bounds.removeFromTop(25));
        bounds.removeFromTop(10);
        
        licenseKeyEditor.setBounds(bounds.removeFromTop(40));
        bounds.removeFromTop(20);
        
        activateButton.setBounds(bounds.removeFromTop(40));
        bounds.removeFromTop(10);
        
        purchaseButton.setBounds(bounds.removeFromTop(40));
    }
    else
    {
        licenseInfoLabel.setBounds(bounds.removeFromTop(50));
        bounds.removeFromTop(15);
        
        creditsLabel.setBounds(bounds.removeFromTop(70));
        bounds.removeFromTop(15);
        
        closeButton.setBounds(bounds.removeFromTop(40));
    }
}

void LicenseDialog::buttonClicked(juce::Button* button)
{
    if (button == &activateButton)
    {
        activateLicense();
    }
    else if (button == &purchaseButton)
    {
        showPurchaseInfo();
    }
    else if (button == &closeButton)
    {
        setVisible(false);
        if (onLicenseActivated)
        {
            onLicenseActivated();
        }
    }
}

void LicenseDialog::textEditorReturnKeyPressed(juce::TextEditor&)
{
    activateLicense();
}

void LicenseDialog::activateLicense()
{
    auto key = licenseKeyEditor.getText().trim().toUpperCase();
    
    DBG("Attempting to activate license: " + key);
    
    if (key.isEmpty())
    {
        DBG("License key is empty");
        juce::AlertWindow::showMessageBoxAsync(
            juce::AlertWindow::WarningIcon,
            "Invalid License Key",
            "Please enter a license key.",
            "OK"
        );
        return;
    }
    
    DBG("Calling licenseManager.activateLicense()");
    bool success = licenseManager.activateLicense(key);
    DBG("Activation result: " + juce::String(success ? "SUCCESS" : "FAILED"));
    
    if (success)
    {
        DBG("License activated successfully");
        
        // Update UI to show licensed status
        updateUIForLicenseStatus();
        
        // Show success message
        juce::AlertWindow::showMessageBoxAsync(
            juce::AlertWindow::InfoIcon,
            "License Activated",
            "Your license has been successfully activated!\n\n"
            "Thank you for using Auric Halo.",
            "OK"
        );
    }
    else
    {
        DBG("License activation failed");
        juce::AlertWindow::showMessageBoxAsync(
            juce::AlertWindow::WarningIcon,
            "Activation Failed",
            "Invalid license key or license not valid for this computer.\n\n"
            "Please check your license key and try again.\n"
            "If you continue to have problems, contact support.",
            "OK"
        );
    }
}

void LicenseDialog::showPurchaseInfo()
{
    // Open purchase URL
    juce::URL purchaseURL("https://www.auricaudio.com/purchase"); // Change to your URL
    purchaseURL.launchInDefaultBrowser();
    
    juce::AlertWindow::showMessageBoxAsync(
        juce::AlertWindow::InfoIcon,
        "Purchase Auric Halo",
        "Your web browser will open to our purchase page.\n\n"
        "After purchase, you will receive a license key via email.\n"
        "Enter that key here to activate your license.",
        "OK"
    );
}

void LicenseDialog::updateUIForLicenseStatus()
{
    isLicensed = (licenseManager.getStatus() == LicenseManager::Status::Licensed);
    
    // Update title
    titleLabel.setText(isLicensed ? "Auric Halo - Licensed" : "Auric Halo - License Activation", 
                       juce::dontSendNotification);
    
    // Update status
    statusLabel.setText(licenseManager.getLicenseInfo(), juce::dontSendNotification);
    statusLabel.setColour(juce::Label::textColourId, 
                          isLicensed ? juce::Colours::lightgreen : juce::Colours::orange);
    
    // Show/hide components based on license status
    instructionLabel.setVisible(!isLicensed);
    licenseKeyEditor.setVisible(!isLicensed);
    activateButton.setVisible(!isLicensed);
    purchaseButton.setVisible(!isLicensed);
    
    licenseInfoLabel.setVisible(isLicensed);
    creditsLabel.setVisible(isLicensed);
    closeButton.setVisible(isLicensed);
    
    // Update credits if needed
    if (isLicensed)
    {
        creditsLabel.setText("Credits:\n"
                             "Arie Wardhana\n"
                             "Raditya Rakha Wardhana\n"
                             "Kevin Hermansyah",
                             juce::dontSendNotification);
        creditsLabel.setFont(juce::Font(12.0f));
        creditsLabel.setJustificationType(juce::Justification::centred);
        creditsLabel.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    }
    
    // Resize dialog
    setSize(500, isLicensed ? 350 : 350);
    resized();
}
