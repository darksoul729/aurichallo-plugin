#include "LicenseManager.h"

LicenseManager::LicenseManager()
{
    loadLicenseFromFile();
}

LicenseManager::~LicenseManager()
{
}

bool LicenseManager::isLicensed() const
{
    return licensed;
}

bool LicenseManager::activateLicense(const juce::String& key)
{
    if (validateLicenseKey(key))
    {
        licenseKey = key;
        licensed = true;
        activationDate = juce::Time::getCurrentTime();
        saveLicenseToFile();
        return true;
    }
    return false;
}

juce::String LicenseManager::getLicenseKey() const
{
    return licenseKey;
}

juce::String LicenseManager::getLicenseEmail() const
{
    return licenseEmail;
}

juce::String LicenseManager::getLicenseStatus() const
{
    if (licensed)
        return "Licensed";
    else if (isTrialMode())
        return "Trial (" + juce::String(getTrialDaysRemaining()) + " days remaining)";
    else
        return "Unlicensed";
}

bool LicenseManager::isTrialMode() const
{
    // For now, always allow trial
    return !licensed;
}

int LicenseManager::getTrialDaysRemaining() const
{
    // Simple trial: 30 days from first use
    auto firstUseFile = getLicenseFile();
    if (!firstUseFile.existsAsFile())
        return 30;
    
    auto firstUseTime = juce::Time(firstUseFile.getCreationTime());
    auto now = juce::Time::getCurrentTime();
    auto daysPassed = (now.toMilliseconds() - firstUseTime.toMilliseconds()) / (1000 * 60 * 60 * 24);
    
    return juce::jmax(0, 30 - (int)daysPassed);
}

void LicenseManager::deactivateLicense()
{
    licensed = false;
    licenseKey.clear();
    licenseEmail.clear();
    saveLicenseToFile();
}

void LicenseManager::loadLicenseFromFile()
{
    auto file = getLicenseFile();
    if (file.existsAsFile())
    {
        auto xml = juce::parseXML(file);
        if (xml != nullptr)
        {
            licenseKey = xml->getStringAttribute("key");
            licenseEmail = xml->getStringAttribute("email");
            licensed = xml->getBoolAttribute("licensed", false);
            
            if (licensed && !validateLicenseKey(licenseKey))
            {
                // Invalid license, reset
                licensed = false;
                licenseKey.clear();
            }
        }
    }
}

void LicenseManager::saveLicenseToFile()
{
    auto file = getLicenseFile();
    file.getParentDirectory().createDirectory();
    file.create();
    
    juce::XmlElement xml("AuricHaloLicense");
    xml.setAttribute("key", licenseKey);
    xml.setAttribute("email", licenseEmail);
    xml.setAttribute("licensed", licensed);
    xml.setAttribute("activationDate", activationDate.toISO8601(true));
    
    xml.writeTo(file);
}

bool LicenseManager::validateLicenseKey(const juce::String& key)
{
    // Simple validation: key must be at least 16 characters
    // In production, you'd validate against a server or use cryptographic signing
    if (key.length() < 16)
        return false;
    
    // For demo: accept any key starting with "AURIC-"
    if (key.startsWith("AURIC-"))
        return true;
    
    // TODO: Implement proper license validation
    // - Check against server
    // - Verify cryptographic signature
    // - Check hardware ID binding
    
    return false;
}

LicenseManager::Status LicenseManager::getStatus() const
{
    if (licensed)
        return Status::Licensed;
    else if (getTrialDaysRemaining() > 0)
        return Status::Trial;
    else
        return Status::Expired;
}

juce::String LicenseManager::getLicenseInfo() const
{
    if (licensed)
    {
        juce::String info = "Licensed to: ";
        info += licenseEmail;
        info += "\nActivated: ";
        info += activationDate.toString(true, true);
        return info;
    }
    else if (isTrialMode())
    {
        juce::String info = "Trial Mode\n";
        info += juce::String(getTrialDaysRemaining());
        info += " days remaining";
        return info;
    }
    else
    {
        return juce::String("No license found\nPlease purchase a license to continue using this plugin.");
    }
}

juce::File LicenseManager::getLicenseFile() const
{
    auto appData = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory);
    auto folder = appData.getChildFile("Auric Audio").getChildFile("Auric Halo");
    // Don't create directory in const method
    return folder.getChildFile("license.dat");
}
