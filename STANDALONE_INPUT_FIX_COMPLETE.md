# Standalone Audio Input Fix - COMPLETE ✅

## Problem
The AuricHalo standalone app was not receiving audio input from guitar/microphone through Loopback Audio. The level meters remained empty even though:
- Loopback Audio was selected as input device
- Input channels 1+2 were active
- The same audio worked in Pro Tools (AAX plugin)

## Root Cause
JUCE's default standalone wrapper (`StandalonePluginHolder`) initializes the audio device manager with input channels **disabled by default** for audio effect plugins. This is a known JUCE behavior where standalone apps assume effects don't need input unless explicitly configured.

## Solution Applied - ARKAN_AI_PATCH_PROTOCOL v1.1

### 1. Created Custom Standalone Application
**File**: `source/StandaloneApp.cpp`

Replaced JUCE's default standalone wrapper with a custom implementation that:

```cpp
// CRITICAL FIX: Initialize with input channels ENABLED
deviceManager.initialise(
    2,      // numInputChannelsNeeded - Enable 2 input channels
    2,      // numOutputChannelsNeeded
    nullptr,
    true
);

// FORCE input channels to be active (fixes macOS disabled state)
auto setup = deviceManager.getAudioDeviceSetup();
setup.inputChannels.setRange(0, 2, true);  // Enable channels 0 and 1
setup.useDefaultInputChannels = false;
deviceManager.setAudioDeviceSetup(setup, true);
```

### 2. Disabled Default JUCE Standalone
**File**: `JuceLibraryCode/include_juce_audio_plugin_client_Standalone.cpp`

Commented out the default JUCE standalone include to prevent conflicts:
```cpp
// #include <juce_audio_plugin_client/juce_audio_plugin_client_Standalone.cpp>
```

### 3. Added Custom Standalone to Project
**File**: `AuricHalo.jucer`

Registered the custom standalone app file:
```xml
<FILE id="src009" name="StandaloneApp.cpp" compile="1" resource="0"
      file="source/StandaloneApp.cpp" compilerFlagSchemes="Standalone"/>
```

### 4. Added Debug Logging
**File**: `source/AuricHaloProcessor.cpp`

Added comprehensive debug logging to track:
- Channel configuration in `prepareToPlay()`
- Buffer data in `processBlock()` (every 100 blocks)
- Input/output level measurements

## Testing Instructions

### 1. Launch Standalone App
```bash
open /Applications/AuricHalo.app
```

### 2. Check Console for Debug Output
```bash
# In Terminal, monitor the app's debug output:
log stream --predicate 'process == "AuricHalo"' --level debug
```

You should see:
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
Max sample value in buffer: 0.0234  <-- Should be > 0 when playing guitar
==================
```

### 3. Test Audio Input

1. **Setup Audio Routing**:
   - Guitar → Audio Interface → Loopback (Output 1+2)
   - AuricHalo Standalone: Select "Loopback Audio" as input device
   - Enable Input channels 1+2

2. **Verify Signal Flow**:
   - Play guitar
   - **Input meter** (left side) should show green/yellow/red LEDs
   - **Output meter** (right side) should show processed signal
   - Adjust Input knob (0-150%) to control input gain
   - Adjust Drive/Tone to hear saturation effect
   - Adjust Mix to blend dry/wet signal

3. **Compare with DAW**:
   - Open Pro Tools or Logic Pro
   - Load AuricHalo as AAX/AU plugin
   - Same audio routing should work identically

## Build Commands

### Rebuild Standalone Only
```bash
cd /Users/ariewardhana/Project_Plugin/aurichallo-plugin
xcodebuild -project Builds/MacOSX/AuricHalo.xcodeproj \
  -scheme "AuricHalo - Standalone Plugin" \
  -configuration Release \
  clean build
```

### Install to Applications
```bash
sudo rm -rf /Applications/AuricHalo.app
sudo cp -R "Builds/MacOSX/build/Release/AuricHalo.app" /Applications/
```

### Rebuild All Formats (VST3, AU, AAX, Standalone)
```bash
xcodebuild -project Builds/MacOSX/AuricHalo.xcodeproj \
  -scheme "AuricHalo - All" \
  -configuration Release \
  clean build
```

## Files Modified

1. **source/StandaloneApp.cpp** (NEW)
   - Custom standalone application with input enabled
   - Explicit audio device manager configuration
   - Debug logging for troubleshooting

2. **source/AuricHaloProcessor.cpp**
   - Added debug logging in `prepareToPlay()`
   - Added debug logging in `processBlock()`
   - Fixed input level measurement for standalone

3. **AuricHalo.jucer**
   - Added StandaloneApp.cpp to project
   - Configured for Standalone build scheme

4. **JuceLibraryCode/include_juce_audio_plugin_client_Standalone.cpp**
   - Disabled default JUCE standalone wrapper

## Technical Details

### Why This Fix Works

1. **Explicit Input Initialization**: The default JUCE standalone wrapper calls `initialise(0, 2, ...)` for effect plugins, assuming they don't need input. Our custom wrapper explicitly requests 2 input channels.

2. **Force Channel Activation**: Even after initialization, macOS can leave input channels disabled. We explicitly set `inputChannels.setRange(0, 2, true)` to force activation.

3. **Disable Default Behavior**: Setting `useDefaultInputChannels = false` prevents JUCE from reverting to its default (input-disabled) configuration.

4. **Direct Audio Callback**: Our custom wrapper directly connects the processor to the device manager's audio callback, ensuring audio flows through `processBlock()`.

### Channel Configuration

The plugin supports:
- **Stereo**: 2 in / 2 out (default)
- **Mono**: 1 in / 1 out

Configured in `AuricHaloProcessor` constructor:
```cpp
AudioProcessor(BusesProperties()
    .withInput("Input", juce::AudioChannelSet::stereo(), true)
    .withOutput("Output", juce::AudioChannelSet::stereo(), true))
```

## Verification Checklist

- [x] Standalone app builds successfully
- [x] Custom standalone wrapper replaces JUCE default
- [x] Audio device manager initializes with input enabled
- [x] Input channels forced to active state
- [x] Debug logging added for troubleshooting
- [x] App installed to /Applications/
- [ ] **USER TEST**: Input meter shows signal from guitar
- [ ] **USER TEST**: Output meter shows processed signal
- [ ] **USER TEST**: Audio processing matches DAW plugin behavior

## Next Steps

1. **Launch the app**: `open /Applications/AuricHalo.app`
2. **Select Loopback Audio** as input device in Audio Settings
3. **Enable Input 1+2** channels
4. **Play guitar** and verify input meter responds
5. **Check Console** for debug output if issues persist

## Troubleshooting

### If Input Still Not Working:

1. **Check Console Output**:
   ```bash
   log stream --predicate 'process == "AuricHalo"' --level debug
   ```
   Look for "Max sample value in buffer" - should be > 0 when playing

2. **Verify Audio Device**:
   - Open Audio Settings in app
   - Confirm "Loopback Audio" is selected
   - Confirm Input 1+2 are checked (blue)

3. **Test with Built-in Mic**:
   - Select "Built-in Microphone" as input
   - Speak/clap near Mac
   - If this works, issue is with Loopback routing

4. **Check Loopback Configuration**:
   - Open Loopback app
   - Verify Output 1+2 is routing your guitar signal
   - Test with another app (Osun) to confirm Loopback works

5. **Rebuild with Clean**:
   ```bash
   xcodebuild -project Builds/MacOSX/AuricHalo.xcodeproj \
     -scheme "AuricHalo - Standalone Plugin" \
     -configuration Release \
     clean build
   ```

## Success Criteria

✅ **FIXED** when:
- Input meter (left) shows green/yellow/red LEDs when playing guitar
- Output meter (right) shows processed signal
- Audio sounds identical to AAX/AU plugin in DAW
- No latency or dropouts
- All parameters (Input, Drive, Tone, Mix, Output) work correctly

---

**Status**: READY FOR USER TESTING
**Build**: Release (optimized)
**Location**: `/Applications/AuricHalo.app`
**Date**: December 4, 2025
