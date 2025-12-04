# AuricHalo Standalone - Testing Instructions

## Build Status
✅ **REBUILT** with audio input + MIDI support enabled
✅ **INSTALLED** to `/Applications/AuricHalo.app`

## Changes Made

### 1. Enabled MIDI Input
**File**: `AuricHalo.jucer`
```xml
pluginWantsMidiIn="1"  <!-- Changed from 0 to 1 -->
```

**File**: `source/AuricHaloProcessor.h`
```cpp
bool acceptsMidi() const override { return true; }  // Changed from false
```

**Why**: Enables standalone app to work with both audio AND MIDI inputs. Makes it more flexible.

### 2. Custom Standalone Wrapper
**File**: `source/StandaloneApp.cpp`

Forces audio input to be enabled:
```cpp
deviceManager.initialise(
    2,      // Enable 2 input channels
    2,      // Enable 2 output channels
    nullptr,
    true
);

// Force input channels active
auto setup = deviceManager.getAudioDeviceSetup();
setup.inputChannels.setRange(0, 2, true);
setup.useDefaultInputChannels = false;
deviceManager.setAudioDeviceSetup(setup, true);
```

### 3. Debug Logging
Added comprehensive logging to track:
- Audio device initialization
- Channel configuration
- Buffer data flow
- Input/output levels

## Testing Steps

### Method 1: Quick Test
```bash
# Launch app
open /Applications/AuricHalo.app

# In the app:
# 1. Look for audio settings (might be in menu or button)
# 2. Select "Loopback Audio" as input
# 3. Enable Input 1+2
# 4. Play guitar
# 5. Check if input meter (left side) shows signal
```

### Method 2: Debug Monitor
```bash
# Run debug monitor script
./test_standalone_debug.sh

# This will:
# 1. Launch AuricHalo
# 2. Show debug output in terminal
# 3. Look for these messages:
```

**Expected Output**:
```
=== STANDALONE APP INIT ===
Initializing audio with INPUT channels enabled
Forcing input channels: 11
Active input count: 2
Input device: Loopback Audio
===========================

=== PREPARE TO PLAY ===
Sample rate: 48000
Total input channels: 2
Total output channels: 2
=======================

=== AUDIO DEBUG ===
Input channels: 2
Buffer channels: 2
Buffer samples: 512
Max sample value in buffer: 0.0234  <-- Should be > 0 when playing
==================
```

### Method 3: Manual Console Check
```bash
# In one terminal, monitor logs:
log stream --process AuricHalo --level debug

# In another terminal, launch app:
open /Applications/AuricHalo.app
```

## What to Check

### 1. Audio Device Settings
When you open the app, check if there's a way to access audio settings:
- Menu bar → Preferences/Settings
- Button in the UI (might say "Options" or "Settings")
- Right-click on the window

### 2. Input Meter
- **Left side meter** = Input level
- Should show green/yellow/red LEDs when guitar is playing
- If it stays empty (blue), input is not working

### 3. Output Meter
- **Right side meter** = Output level
- Should show processed signal
- Should respond to Drive/Tone/Mix parameters

### 4. Console Output
Look for these key indicators:

**✅ GOOD**:
```
Active input count: 2
Max sample value in buffer: 0.0234  (any value > 0)
```

**❌ BAD**:
```
Active input count: 0
Max sample value in buffer: 0.0  (always zero)
```

## Troubleshooting

### If Input Still Not Working:

#### Option 1: Check Loopback Configuration
```bash
# Verify Loopback is routing audio:
# 1. Open Loopback app
# 2. Check Output 1+2 is active
# 3. Test with another app (Osun) to confirm routing works
```

#### Option 2: Test with Built-in Mic
```bash
# In AuricHalo audio settings:
# 1. Select "Built-in Microphone" instead of Loopback
# 2. Speak/clap near Mac
# 3. If input meter responds, issue is with Loopback routing
# 4. If still no response, issue is with standalone app
```

#### Option 3: Check Console for Errors
```bash
# Look for error messages:
log stream --process AuricHalo --level error

# Common errors:
# - "Audio init error: ..."
# - "Setup result: ..." (should be "SUCCESS")
# - "Active input count: 0" (should be 2)
```

#### Option 4: Verify App Permissions
```bash
# macOS might block microphone access
# Go to: System Settings → Privacy & Security → Microphone
# Make sure "AuricHalo" is checked
```

#### Option 5: Compare with DAW
```bash
# Test in Pro Tools or Logic:
# 1. Load AuricHalo as AAX/AU plugin
# 2. Same Loopback routing
# 3. If it works in DAW but not standalone, we need to debug further
```

## Debug Information to Provide

If input still doesn't work, please provide:

1. **Console Output**:
   ```bash
   ./test_standalone_debug.sh > debug_output.txt
   # Play guitar for 10 seconds
   # Press Ctrl+C
   # Send debug_output.txt
   ```

2. **Audio Settings Screenshot**:
   - Take screenshot of audio settings in AuricHalo
   - Show which device is selected
   - Show which channels are enabled

3. **Loopback Configuration**:
   - Screenshot of Loopback routing
   - Confirm Output 1+2 is active

4. **System Audio Settings**:
   - System Settings → Sound → Input
   - What input device is selected?
   - Does level meter move when playing guitar?

## Next Steps

### If It Works:
🎉 Great! The standalone app is now functional.

### If It Still Doesn't Work:
We'll try these advanced fixes:
1. Patch JUCE's StandalonePluginHolder directly
2. Add explicit audio callback routing
3. Check if macOS security is blocking audio input
4. Test with different audio interfaces
5. Compare with working JUCE standalone examples

## Quick Reference

**Launch App**:
```bash
open /Applications/AuricHalo.app
```

**Monitor Debug**:
```bash
./test_standalone_debug.sh
```

**Rebuild**:
```bash
xcodebuild -project Builds/MacOSX/AuricHalo.xcodeproj \
  -scheme "AuricHalo - Standalone Plugin" \
  -configuration Release clean build

sudo rm -rf /Applications/AuricHalo.app
sudo cp -R "Builds/MacOSX/build/Release/AuricHalo.app" /Applications/
```

**Check Permissions**:
```bash
# System Settings → Privacy & Security → Microphone
# Make sure AuricHalo is listed and checked
```

---

**Status**: READY FOR TESTING
**Version**: 1.0.0 (with audio input + MIDI support)
**Date**: December 4, 2025
