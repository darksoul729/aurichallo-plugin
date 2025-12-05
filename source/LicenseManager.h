#pragma once
#include <JuceHeader.h>

class LicenseManager
{
public:
    enum class Status
    {
        Licensed,
        Trial,
        Expired
    };
    
    LicenseManager();
    ~LicenseManager();
    
    // Check if plugin is licensed
    bool isLicensed() const;
    
    // Activate license with key
    bool activateLicense(const juce::String& licenseKey);
    
    // Get license info
    juce::String getLicenseKey() const;
    juce::String getLicenseEmail() const;
    juce::String getLicenseStatus() const;
    juce::String getLicenseInfo() const;
    
    // Get status
    Status getStatus() const;
    
    // Trial mode
    bool isTrialMode() const;
    int getTrialDaysRemaining() const;
    
    // Deactivate license
    void deactivateLicense();
    
private:
    void loadLicenseFromFile();
    void saveLicenseToFile();
    bool validateLicenseKey(const juce::String& key);
    juce::File getLicenseFile() const;
    
    bool licensed = false;
    juce::String licenseKey;
    juce::String licenseEmail;
    juce::Time activationDate;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LicenseManager)
};
