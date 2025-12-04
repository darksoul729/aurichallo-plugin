#include "LicenseManager.h"

LicenseManager::LicenseManager()
    : cachedStatus(Status::Unlicensed)
{
    loadLicenseData();
}

LicenseManager::~LicenseManager()
{
}

LicenseManager::Status LicenseManager::getStatus()
{
    // Check if we have a valid license
    if (!cachedLicenseKey.isEmpty())
    {
        if (validateLicenseKey(cachedLicenseKey) && 
            isLicenseValidForThisComputer(cachedLicenseKey))
        {
            cachedStatus = Status::Licensed;
            return cachedStatus;
        }
    }
    
    // No trial - must have license
    cachedStatus = Status::Unlicensed;
    return cachedStatus;
}

bool LicenseManager::activateLicense(const juce::String& licenseKey)
{
    // Validate format
    if (!validateLicenseKey(licenseKey))
    {
        return false;
    }
    
    // Check if valid for this computer
    if (!isLicenseValidForThisComputer(licenseKey))
    {
        return false;
    }
    
    // Save license
    cachedLicenseKey = licenseKey;
    cachedStatus = Status::Licensed;
    saveLicenseData();
    
    return true;
}

int LicenseManager::getRemainingTrialDays()
{
    if (cachedStatus == Status::Licensed)
        return -1; // Licensed, no trial
    
    if (trialStartDate == juce::Time())
        return TRIAL_DAYS;
    
    auto now = juce::Time::getCurrentTime();
    auto daysPassed = (now.toMilliseconds() - trialStartDate.toMilliseconds()) / (1000 * 60 * 60 * 24);
    
    return juce::jmax(0, TRIAL_DAYS - (int)daysPassed);
}

bool LicenseManager::isPluginUsable()
{
    auto status = getStatus();
    return (status == Status::Licensed);
}

juce::String LicenseManager::getLicenseInfo()
{
    auto status = getStatus();
    
    switch (status)
    {
        case Status::Licensed:
            return "Licensed";
            
        case Status::Unlicensed:
        default:
            return "Not Activated - Please Enter License Key";
    }
}

void LicenseManager::deactivateLicense()
{
    cachedLicenseKey = "";
    cachedStatus = Status::Unlicensed;
    saveLicenseData();
}

// Private methods

juce::String LicenseManager::getHardwareID()
{
    // Generate unique ID based on computer hardware
    juce::String hwid;
    
    // Use MAC address as base
    auto addresses = juce::MACAddress::getAllAddresses();
    if (!addresses.isEmpty())
    {
        hwid = addresses[0].toString().removeCharacters(":-");
    }
    
    // Add system ID
    hwid += juce::SystemStats::getComputerName().removeCharacters(" ");
    
    // Simple hash using string hash
    auto hashValue = hwid.hashCode64();
    return juce::String::toHexString((juce::int64)hashValue).substring(0, 16).toUpperCase();
}

bool LicenseManager::validateLicenseKey(const juce::String& key)
{
    // Format: XXXX-XXXX-XXXX-XXXX (16 chars + 3 dashes = 19 total)
    if (key.length() != 19)
        return false;
    
    // Check format
    if (key[4] != '-' || key[9] != '-' || key[14] != '-')
        return false;
    
    // Remove dashes for validation
    auto cleanKey = key.removeCharacters("-");
    
    // Check if all alphanumeric
    for (int i = 0; i < cleanKey.length(); ++i)
    {
        if (!juce::CharacterFunctions::isLetterOrDigit(cleanKey[i]))
            return false;
    }
    
    // Validate checksum (last 4 chars)
    auto keyData = cleanKey.substring(0, 12);
    auto checksum = cleanKey.substring(12);
    
    // Simple checksum validation using string hash
    auto hashValue = (PRODUCT_ID + keyData).hashCode();
    auto expectedChecksum = juce::String::toHexString(hashValue).substring(0, 4).toUpperCase();
    
    return checksum.equalsIgnoreCase(expectedChecksum);
}

bool LicenseManager::isLicenseValidForThisComputer(const juce::String& key)
{
    // TEMPORARY: Disable hardware lock for testing
    // Any valid checksum will work
    return true;
    
    /* TODO: Re-enable for production
    // Extract hardware ID from license key
    auto cleanKey = key.removeCharacters("-");
    auto keyHWID = cleanKey.substring(4, 12); // 8 chars for HWID
    
    // Get this computer's HWID
    auto thisHWID = getHardwareID().substring(0, 8);
    
    return keyHWID.equalsIgnoreCase(thisHWID);
    */
}

void LicenseManager::initializeTrial()
{
    trialStartDate = juce::Time::getCurrentTime();
    saveLicenseData();
}

bool LicenseManager::isTrialExpired()
{
    if (trialStartDate == juce::Time())
        return false;
    
    auto now = juce::Time::getCurrentTime();
    auto daysPassed = (now.toMilliseconds() - trialStartDate.toMilliseconds()) / (1000 * 60 * 60 * 24);
    
    return daysPassed >= TRIAL_DAYS;
}

juce::Time LicenseManager::getTrialStartDate()
{
    return trialStartDate;
}

void LicenseManager::saveLicenseData()
{
    auto appData = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory);
    auto licenseFile = appData.getChildFile("AuricAudio").getChildFile(".aurichalo_lic");
    
    licenseFile.getParentDirectory().createDirectory();
    
    juce::XmlElement xml("License");
    xml.setAttribute("key", encrypt(cachedLicenseKey));
    xml.setAttribute("trial", (int)trialStartDate.toMilliseconds());
    
    xml.writeTo(licenseFile);
}

void LicenseManager::loadLicenseData()
{
    auto appData = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory);
    auto licenseFile = appData.getChildFile("AuricAudio").getChildFile(".aurichalo_lic");
    
    if (!licenseFile.existsAsFile())
        return;
    
    auto xml = juce::XmlDocument::parse(licenseFile);
    if (xml != nullptr)
    {
        cachedLicenseKey = decrypt(xml->getStringAttribute("key"));
        trialStartDate = juce::Time(xml->getIntAttribute("trial", 0));
    }
}

juce::String LicenseManager::encrypt(const juce::String& data)
{
    // Simple XOR encryption (for basic obfuscation)
    juce::MemoryBlock block(data.toUTF8(), data.getNumBytesAsUTF8());
    
    const char key[] = "AuricHaloSecret2024";
    for (size_t i = 0; i < block.getSize(); ++i)
    {
        block[i] ^= key[i % (sizeof(key) - 1)];
    }
    
    return block.toBase64Encoding();
}

juce::String LicenseManager::decrypt(const juce::String& data)
{
    juce::MemoryBlock block;
    if (!block.fromBase64Encoding(data))
        return {};
    
    const char key[] = "AuricHaloSecret2024";
    for (size_t i = 0; i < block.getSize(); ++i)
    {
        block[i] ^= key[i % (sizeof(key) - 1)];
    }
    
    return block.toString();
}
