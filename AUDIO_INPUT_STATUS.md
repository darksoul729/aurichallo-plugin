# Audio Input Status - Standalone App

## Current Status

### What We've Done:
1. ✅ **Patched JUCE Source Code**
   - File: `~/Downloads/JUCE/modules/juce_audio_plugin_client/Standalone/juce_StandaloneFilterWindow.h`
   - Line 109: Changed `auto audioInputRequired = (inChannels > 0);` to `auto audioInputRequired = true;`
   - This forces JUCE to ALWAYS enable audio input in standalone mode

2. ✅ **Configured Plugin Correctly**
   - Bus configuration: Stereo input + Stereo output
   - `acceptsMidi()` returns `true` for flexibility
   - Input/Output parameters: 0-150% range

3. ✅ **Added Debug Logging**
   - `prepareToPlay()` logs channel configuration
   - `processBlock()` logs buffer data every 100 blocks
   - Should see "=== PREPARE TO PLAY ===" in console

4. ✅ **Clean Rebuild**
   - Cleaned all build artifacts
   - Rebuilt with patched JUCE
   - Installed to `/Applications/AuricHalo.app`

### The Problem:
**Debug output is NOT appearing in console**, which means either:
- JUCE's default standalone is being used (not our custom one)
- Debug logging is being stripped in Release build
- Console filtering is hiding our output

## Testing Steps

### Test 1: Check if Plugin Works in DAW
```bash
# Test in Pro Tools or Logic Pro
# Load AuricHalo as AAX or AU plugin
# Verify audio input works there
```

**Expected**: Should work fine (you confirmed it does in Pro Tools)

### Test 2: Test Standalone with Built-in Mic
```bash
# 1. Open standalone app
open /Applications/AuricHalo.app

# 2. In app, look for audio settings
#    - Might be "Options" button
#    - Or menu bar → Preferences

# 3. Select "Built-in Microphone" as input
# 4. Enable Input channels 1+2
# 5. SPEAK LOUDLY or CLAP
# 6. Watch input meter (left side)
```

**Expected**: Input meter should move when you make noise

### Test 3: Check Console Output (Debug Build)
```bash
# Build DEBUG version to see logs
xcodebuild -project Builds/MacOSX/AuricHalo.xcodeproj \
  -scheme "AuricHalo - Standalone Plugin" \
  -configuration Debug \
  build

# Install debug version
sudo cp -R "Builds/MacOSX/build/Debug/AuricHalo.app" /Applications/

# Monitor console
log stream --process AuricHalo --level debug
```

**Expected**: Should see our debug messages

## Why Audio Input Might Not Work

### Possible Causes:

1. **JUCE Standalone Wrapper Issue**
   - JUCE's `StandalonePluginHolder` might be detecting plugin as "effect only"
   - Even with our patch, it might not be calling `initialise()` with input enabled

2. **macOS Permissions**
   - App might not have microphone permission
   - Check: System Settings → Privacy & Security → Microphone
   - Make sure "AuricHalo" is listed and checked

3. **Loopback Audio Routing**
   - Loopback might not be routing audio correctly
   - Test with Built-in Mic first to isolate issue

4. **Audio Device Configuration**
   - Standalone might be selecting wrong device
   - Or input channels might be disabled in device settings

## Solutions to Try

### Solution 1: Use Debug Build
Debug builds have logging enabled. This will help us see what's happening:

```bash
# Build debug version
xcodebuild -project Builds/MacOSX/AuricHalo.xcodeproj \
  -scheme "AuricHalo - Standalone Plugin" \
  -configuration Debug \
  build

# Install
sudo cp -R "Builds/MacOSX/build/Debug/AuricHalo.app" /Applications/

# Run and monitor
open /Applications/AuricHalo.app
log stream --process AuricHalo --level debug | grep -E "(PREPARE|AUDIO|Input|channels)"
```

### Solution 2: Check Permissions
```bash
# Open System Settings
open "x-apple.systempreferences:com.apple.preference.security?Privacy_Microphone"

# Make sure AuricHalo is listed and enabled
```

### Solution 3: Test with Simple Audio Source
Instead of Loopback:
1. Use Built-in Microphone
2. Speak loudly or clap
3. If input meter moves → Loopback is the issue
4. If still no input → Standalone wrapper is the issue

### Solution 4: Compare with Working Plugin
Find another JUCE standalone plugin that works with audio input:
1. See how they configure their processor
2. Check if they use custom standalone wrapper
3. Compare with our implementation

## Next Steps

1. **Build Debug version** and check console output
2. **Test with Built-in Mic** to isolate Loopback issue
3. **Check macOS permissions** for microphone access
4. **If still not working**: We may need to create a completely custom standalone app (not using JUCE's wrapper)

## Alternative: Use Plugin in DAW

Since the plugin works perfectly in Pro Tools (AAX), you can:
- Use it as AAX/AU plugin in your DAW
- Route Loopback audio into DAW track
- Process with AuricHalo plugin

This is actually the recommended workflow for most users anyway!

---

**Current Status**: Audio input in standalone is NOT working yet
**Workaround**: Use plugin in DAW (Pro Tools, Logic, etc.) - works perfectly
**Next Action**: Build debug version and check console logs
