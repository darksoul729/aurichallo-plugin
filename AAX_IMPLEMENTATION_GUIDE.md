# 🎚️ AAX Implementation Guide - Auric Halo

## What is AAX?

**AAX (Avid Audio Extension)** adalah format plugin untuk Pro Tools dari Avid.

### AAX Variants
1. **AAX Native** - CPU-based processing (most common)
2. **AAX DSP** - HDX hardware processing (requires HDX cards)
3. **AAX AudioSuite** - Offline processing

**Kita akan implement**: AAX Native

---

## Prerequisites

### 1. AAX SDK
📥 **Download dari**: Avid Developer Portal
- Perlu register sebagai Avid developer
- Free untuk development
- Paid untuk distribution ($299/year)

**Link**: https://www.avid.com/alliance-partner-program/aax-connectivity-toolkit

### 2. iLok License Manager
- Required untuk development
- Required untuk distribution
- Free download dari iLok.com

### 3. Code Signing Certificate
- Required untuk distribution
- Dari Avid/PACE
- ~$299/year

---

## Implementation Steps

### Step 1: Get AAX SDK

```bash
# After downloading AAX SDK
1. Extract to: C:\SDKs\AAX_SDK\
2. Set environment variable:
   AAX_SDK_PATH=C:\SDKs\AAX_SDK
```

### Step 2: Update Projucer Project

Open `AuricHalo.jucer` dan tambahkan:

#### Plugin Formats
```
✅ VST3
✅ Standalone
✅ AAX  ← Enable this
```

#### AAX Settings
```
AAX Identifier: com.auricaudio.aurichalo
AAX Category: Effect
AAX Manufacturer Code: Aurc
AAX Plugin ID: AuHa
```

#### Module Paths
```
AAX SDK Path: C:\SDKs\AAX_SDK
```

### Step 3: Code Modifications

#### No Code Changes Needed!
JUCE handles AAX wrapper automatically jika:
- ✅ Plugin sudah implement AudioProcessor correctly
- ✅ Parameters menggunakan APVTS
- ✅ processBlock() implemented properly

### Step 4: Build AAX

```bash
# Di Projucer
1. Save Project (Ctrl+Shift+S)

# Di Visual Studio
2. Select "AuricHalo_AAX" project
3. Build → Build Solution
4. Output: Builds/VisualStudio2022/x64/Release/AAX/AuricHalo.aaxplugin
```

### Step 5: Sign Plugin (For Distribution)

```bash
# Using PACE wraptool
wraptool sign --verbose --account YOUR_ACCOUNT --password YOUR_PASSWORD --wcguid YOUR_GUID --in AuricHalo.aaxplugin --out AuricHalo_signed.aaxplugin
```

### Step 6: Install & Test

```bash
# Copy to Pro Tools plugin folder
copy AuricHalo.aaxplugin "C:\Program Files\Common Files\Avid\Audio\Plug-Ins\"

# Launch Pro Tools
# Scan plugins
# Test in session
```

---

## AAX-Specific Requirements

### 1. Plugin Characteristics

```cpp
// Already handled by JUCE, but for reference:

// Latency
int getLatencySamples() const override { return 0; }

// Tail length
double getTailLengthSeconds() const override { return 0.0; }

// Channel configs
bool isBusesLayoutSupported(const BusesLayout& layouts) const override
{
    // Stereo in/out
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    if (layouts.getMainInputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    return true;
}
```

### 2. Parameter Requirements

✅ **Already Implemented**:
- Using APVTS (AudioProcessorValueTreeState)
- Parameters have proper ranges
- Parameters are automatable
- State save/load works

### 3. Processing Requirements

✅ **Already Implemented**:
- processBlock() handles audio correctly
- No denormals
- Thread-safe
- No allocations in audio thread

---

## AAX Categories

Choose appropriate category for plugin:

### Effect Categories
- `ePlugInCategory_None`
- `ePlugInCategory_EQ` ← Good for Auric Halo
- `ePlugInCategory_Dynamics` ← Also good
- `ePlugInCategory_PitchShift`
- `ePlugInCategory_Reverb`
- `ePlugInCategory_Delay`
- `ePlugInCategory_Modulation`
- `ePlugInCategory_Harmonic` ← Best for Auric Halo!
- `ePlugInCategory_NoiseReduction`
- `ePlugInCategory_Dither`
- `ePlugInCategory_SoundField`
- `ePlugInCategory_Effect`

**Recommendation**: `ePlugInCategory_Harmonic`

---

## Testing Checklist

### Development Testing
- [ ] Plugin loads in Pro Tools
- [ ] Audio passes through correctly
- [ ] All parameters work
- [ ] Automation works
- [ ] Bypass works
- [ ] No crashes
- [ ] No audio glitches
- [ ] CPU usage acceptable

### Pro Tools Specific
- [ ] Works in AudioSuite mode
- [ ] Works with clip gain
- [ ] Works with track freeze
- [ ] Works with bounce
- [ ] Saves with session
- [ ] Recalls correctly
- [ ] Works in all sample rates
- [ ] Works in all buffer sizes

### Compatibility
- [ ] Pro Tools 2023+
- [ ] Pro Tools 2022
- [ ] Pro Tools 2021
- [ ] Pro Tools | First (free version)

---

## Common Issues & Solutions

### Issue 1: AAX SDK Not Found
**Solution**: 
```
1. Download AAX SDK from Avid
2. Set AAX_SDK_PATH environment variable
3. Restart Visual Studio
4. Re-save Projucer project
```

### Issue 2: Plugin Won't Load in Pro Tools
**Solution**:
```
1. Check plugin is signed (for release)
2. Check installed in correct folder
3. Rescan plugins in Pro Tools
4. Check Pro Tools version compatibility
```

### Issue 3: Parameters Don't Automate
**Solution**:
```
1. Ensure using APVTS
2. Check parameter flags
3. Verify parameter IDs are unique
```

### Issue 4: Audio Glitches
**Solution**:
```
1. Check buffer size handling
2. Verify no allocations in processBlock
3. Check thread safety
4. Test at different buffer sizes
```

---

## Distribution Requirements

### For Public Release

#### 1. Developer Account
- Register at Avid Developer Portal
- Pay annual fee ($299)
- Get developer credentials

#### 2. Code Signing
- Get PACE/iLok account
- Purchase code signing certificate
- Sign plugin with wraptool

#### 3. Testing
- Test on multiple Pro Tools versions
- Test on multiple systems
- Beta testing with users

#### 4. Documentation
- User manual
- Installation guide
- System requirements
- Known issues

#### 5. Support
- Website
- Email support
- Update mechanism
- Bug tracking

---

## Cost Breakdown

### Development (Free)
- ✅ AAX SDK: Free
- ✅ iLok Manager: Free
- ✅ Development testing: Free

### Distribution (Paid)
- 💰 Avid Developer Program: $299/year
- 💰 Code Signing: Included in developer program
- 💰 Optional: Marketing, website, etc.

**Total**: ~$299/year for AAX distribution

---

## Alternative: Start Without AAX

### Phase 1: VST3 Only
1. ✅ Release VST3 version first
2. ✅ Build user base
3. ✅ Get feedback
4. ✅ Prove concept

### Phase 2: Add AAX Later
1. When revenue justifies $299/year
2. When Pro Tools users request it
3. When plugin is stable

**Recommendation**: Start with VST3, add AAX when needed

---

## Quick Reference

### File Locations

**AAX SDK**:
```
C:\SDKs\AAX_SDK\
```

**Built Plugin**:
```
Builds\VisualStudio2022\x64\Release\AAX\AuricHalo.aaxplugin
```

**Installation**:
```
C:\Program Files\Common Files\Avid\Audio\Plug-Ins\AuricHalo.aaxplugin
```

### Environment Variables
```
AAX_SDK_PATH=C:\SDKs\AAX_SDK
```

### Projucer Settings
```
Plugin Formats: VST3, AAX, Standalone
AAX Identifier: com.auricaudio.aurichalo
AAX Category: Harmonic
```

---

## Resources

### Official Documentation
- Avid AAX SDK Documentation
- JUCE AAX Tutorial
- Pro Tools Plugin Development Guide

### Community
- JUCE Forum (AAX section)
- KVR Audio Forum
- Gearspace Plugin Development

### Tools
- iLok License Manager
- PACE wraptool
- Pro Tools (for testing)

---

## Summary

### To Add AAX Support:

1. **Get AAX SDK** (free, requires registration)
2. **Enable AAX in Projucer** (1 checkbox)
3. **Build** (automatic, no code changes)
4. **Test** (in Pro Tools)
5. **Sign** (for distribution, requires paid account)

### Current Status:
- ✅ Code is AAX-ready (using JUCE best practices)
- ⏳ Need AAX SDK to build
- ⏳ Need Avid account to distribute

### Recommendation:
- Start with VST3 (free, works everywhere)
- Add AAX later when needed
- Focus on making plugin great first!

---

**Last Updated**: December 2025
