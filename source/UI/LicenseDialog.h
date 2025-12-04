#pragma once
#include <JuceHeader.h>
#include "../LicenseManager.h"

class LicenseDialog : public juce::Component,
                      public juce::Button::Listener,
                      public juce::TextEditor::Listener
{
public:
    LicenseDialog(LicenseManager& licenseManager);
    ~LicenseDialog() override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    void buttonClicked(juce::Button* button) override;
    void textEditorReturnKeyPressed(juce::TextEditor&) override;
    
    // Callback when license is activated
    std::function<void()> onLicenseActivated;
    
    // Update UI based on license status
    void updateUIForLicenseStatus();
    
private:
    void activateLicense();
    void showPurchaseInfo();
    
    LicenseManager& licenseManager;
    bool isLicensed = false;
    
    juce::Label titleLabel;
    juce::Label statusLabel;
    juce::Label instructionLabel;
    juce::Label licenseInfoLabel;
    juce::Label creditsLabel;
    
    juce::TextEditor licenseKeyEditor;
    
    juce::TextButton activateButton;
    juce::TextButton purchaseButton;
    juce::TextButton closeButton;
    juce::TextButton continueTrialButton;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LicenseDialog)
};
