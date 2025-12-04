#pragma once
#include <JuceHeader.h>

class LicenseManager
{
public:
    LicenseManager();
    ~LicenseManager();
    
    // License status
    enum class Status
    {
        Unlicensed,
        Trial,
        Licensed,
        Expired
    };
    
    // Check current license status
    Status getStatus();
    
    // Activate with license key
    bool activateLicense(const juce::String& licenseKey);
    
    // Get remaining trial days
    int getRemainingTrialDays();
    
    // Check if plugin is usable
    bool isPluginUsable();
    
    // Get license info for display
    juce::String getLicenseInfo();
    
    // Deactivate license (for testing)
    void deactivateLicense();
    
private:
    // Generate hardware ID (unique per computer)
    juce::String getHardwareID();
    
    // Validate license key format and checksum
    bool validateLicenseKey(const juce::String& key);
    
    // Check if license key matches this computer
    bool isLicenseValidForThisComputer(const juce::String& key);
    
    // Trial management
    void initializeTrial();
    bool isTrialExpired();
    juce::Time getTrialStartDate();
    
    // Storage
    void saveLicenseData();
    void loadLicenseData();
    
    // Encryption helpers
    juce::String encrypt(const juce::String& data);
    juce::String decrypt(const juce::String& data);
    
    // Constants
    static constexpr int TRIAL_DAYS = 14;
    static constexpr const char* PRODUCT_ID = "AURICHALO";
    
    // Cached data
    Status cachedStatus;
    juce::String cachedLicenseKey;
    juce::Time trialStartDate;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LicenseManager)
};
