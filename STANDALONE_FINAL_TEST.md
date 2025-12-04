# AuricHalo Standalone - Final Testing Guide

## ✅ Audio Input Fix Applied

### Changes Made:
1. **Fixed input level measurement** - Menggunakan input channels yang benar
2. **Added safety checks** - Prevent crashes dengan empty buffers
3. **Improved channel handling** - Support berbagai konfigurasi audio

---

## 🧪 Testing Standalone App

### Step 1: Launch App
```bash
open /Applications/AuricHalo.app
```

### Step 2: Configure Audio
1. **Menu:** Options → Audio/MIDI Settings
2. **Input Device:** Pilih audio source Anda
   - Loopback Audio (untuk routing)
   - Audio Interface (untuk direct)
   - Built-in Microphone (untuk test)
3. **Output Device:** Pilih speaker/headphones
4. **Sample Rate:** 44100 Hz atau 48000 Hz
5. **Buffer Size:** 512 samples (balance)

### Step 3: Enable Input Channels
```
Active Input Channels:
☑ Channel 1 (Left)
☑ Channel 2 (Right)
```

### Step 4: Test Signal Flow
```
1. Play gitar/audio source
2. Check Input Meter (bar kiri) → Harus bergerak!
3. Adjust Drive/Mix
4. Check Output Meter (bar kanan) → Harus bergerak!
5. Dengar audio dengan effect
```

---

## 🔍 Troubleshooting Checklist

### ❌ Input Meter Tidak Bergerak?

**Check 1: Audio Device**
```
☐ Device terpilih dengan benar?
☐ Input channels enabled (1-2)?
☐ Sample rate match?
```

**Check 2: Audio Source**
```
☐ Gitar/source playing?
☐ Cable connected?
☐ Interface/Loopback working?
☐ Test di app lain (Pro Tools) → works?
```

**Check 3: AuricHalo Settings**
```
☐ Input Gain > 0%? (default 100%)
☐ Mix > 0%?
☐ Drive > 0%?
```

**Check 4: System**
```
☐ Microphone permission granted?
☐ Audio device not in use by other app?
☐ System audio working?
```

### ❌ Output Meter Tidak Bergerak?

**Check:**
```
☐ Input meter bergerak? (harus ada input dulu)
☐ Drive > 0%?
☐ Mix > 0%?
☐ Output Gain > 0%?
☐ Processing intensity > 0?
```

### ❌ No Sound Output?

**Check:**
```
☐ Output device selected?
☐ System volume up?
☐ Headphones/speakers connected?
☐ Output meter bergerak?
☐ Output Gain > 0%?
```

---

## 🎛️ Recommended Test Settings

### Test 1: Bypass (Dry Signal)
```
Input: 100%
Drive: 0%
Tone: 50%
Mix: 0%
Output: 100%

Expected: Hear dry signal, no processing
Input meter: Moving ✓
Output meter: Moving ✓
```

### Test 2: Subtle Effect
```
Input: 100%
Drive: 20%
Tone: 50%
Mix: 30%
Output: 100%

Expected: Gentle saturation
Both meters: Moving ✓
Hear: Subtle warmth
```

### Test 3: Full Effect
```
Input: 100%
Drive: 60%
Tone: 60%
Mix: 60%
Output: 100%

Expected: Strong saturation
Both meters: Moving ✓
Hear: Clear effect
```

---

## 🔊 Audio Routing Options

### Option 1: Direct Audio Interface (Recommended)
```
Gitar → Audio Interface → AuricHalo Input
Pros: Low latency, simple, reliable
Cons: None
```

### Option 2: Loopback Virtual Audio
```
Gitar → Interface → Loopback → AuricHalo Input
Pros: Flexible routing
Cons: Higher latency, more complex
```

### Option 3: System Audio
```
Any App → Loopback → AuricHalo Input
Pros: Process any audio
Cons: Highest latency
```

---

## 📊 Expected Behavior

### Input Meter (Left Bar):
- **Green:** Normal level (safe)
- **Yellow:** Hot level (watch it)
- **Red:** Very hot (may clip)
- **Moving:** Audio is coming in ✓
- **Static:** No audio input ❌

### Output Meter (Right Bar):
- Same color coding
- Should move when processing active
- Level depends on Drive/Mix/Output settings

### Smart Mix Behavior:
- **Drive 0%, Mix 100%:** Output = Dry (safe!)
- **Drive 50%, Mix 100%:** Output = 50% wet
- **Drive 100%, Mix 100%:** Output = Full wet

---

## 🎯 Success Criteria

Standalone app bekerja dengan baik jika:
- ✅ Input meter bergerak saat ada audio
- ✅ Output meter bergerak saat processing
- ✅ Dengar audio output
- ✅ Effect bekerja (Drive/Tone/Mix)
- ✅ No crackling/dropouts
- ✅ Latency acceptable

---

## 🔧 Advanced Troubleshooting

### Reset Audio Settings:
```bash
# Quit AuricHalo first
rm -rf ~/Library/Preferences/com.auricaudio.aurichalo.plist
rm -rf ~/Library/Application\ Support/AuricHalo/

# Restart AuricHalo
open /Applications/AuricHalo.app
```

### Check Audio Permissions:
```bash
# Reset microphone permission
tccutil reset Microphone com.auricaudio.aurichalo

# Restart AuricHalo
```

### Test Audio System:
```bash
# List audio devices
system_profiler SPAudioDataType

# Test system audio
afplay /System/Library/Sounds/Ping.aiff
```

---

## 💡 Pro Tips

### Tip 1: Use Direct Input
```
Untuk live playing, skip Loopback:
- Input: Direct Audio Interface
- Lower latency
- More reliable
```

### Tip 2: Buffer Size
```
Live playing: 128-256 samples (low latency)
Recording: 512 samples (stable)
Processing: 1024 samples (safe)
```

### Tip 3: Monitor Through DAW
```
Jika standalone terlalu complex:
- Use VST3/AU in DAW
- DAW handles routing
- More features
- More stable
```

---

## 🎉 Final Notes

### Standalone App:
- ✅ Audio input fixed
- ✅ Safety checks added
- ✅ Channel handling improved
- ✅ Ready for testing

### Alternative:
Jika standalone masih bermasalah, **gunakan plugin di DAW**:
- VST3 di Ableton/Reaper/FL Studio
- AU di Logic Pro/GarageBand
- AAX di Pro Tools
- **Lebih reliable untuk production!**

---

## 📦 Files Updated

```
✅ source/AuricHaloProcessor.cpp - Audio input fix
✅ AuricHalo.app - Rebuilt
✅ VST3/AU Plugins - Rebuilt
✅ Installer PKG - Updated (5.3 MB)
```

**Location:** `Release/AuricHalo-1.1.0-macOS.pkg`

---

**Test sekarang dan report jika masih ada issue!** 🎸🎵
