# Fix: Standalone Audio Input dari Loopback

## ❌ Masalah
Audio dari gitar → Loopback (Output 1+2) → Osun (masuk ✓) → AuricHalo Standalone (tidak masuk ❌)

## ✅ Solusi: Konfigurasi Audio Device

### Step 1: Buka AuricHalo Standalone
```
/Applications/AuricHalo.app
```

### Step 2: Buka Audio Settings
Di menu bar AuricHalo:
```
Options → Audio/MIDI Settings
atau
Preferences → Audio Settings
```

### Step 3: Pilih Audio Device
```
Input Device:  Loopback Audio
Output Device: Loopback Audio (atau speaker Anda)

Sample Rate: 44100 Hz (atau 48000 Hz)
Buffer Size: 512 samples (atau sesuai kebutuhan)
```

### Step 4: Pilih Input Channels
```
Active Input Channels:
☑ Channel 1 (Left)
☑ Channel 2 (Right)

Atau pilih channel yang sesuai dengan routing Loopback Anda
```

### Step 5: Test Audio
```
1. Play gitar
2. Check input meter di AuricHalo (bar kiri)
3. Harus ada signal masuk
```

---

## 🔧 Troubleshooting Detail

### Problem 1: Device Tidak Muncul
**Symptom:** Loopback tidak ada di list
**Solution:**
```
1. Quit AuricHalo
2. Buka Audio MIDI Setup (/Applications/Utilities/)
3. Pastikan Loopback device aktif
4. Restart AuricHalo
```

### Problem 2: No Input Signal
**Symptom:** Device terpilih tapi tidak ada signal
**Solution:**
```
1. Check Loopback routing:
   - Gitar → Loopback Input
   - Loopback Output 1+2 → Virtual channel
   
2. Check AuricHalo input channels:
   - Pastikan channel 1+2 active
   - Atau channel yang sesuai routing Loopback

3. Check Input Gain di AuricHalo:
   - Input knob harus > 0%
   - Default: 100%
```

### Problem 3: Audio Routing Loopback
**Setup yang Benar:**
```
Gitar/Audio Source
    ↓
Loopback Input (Physical)
    ↓
Loopback Virtual Output 1+2
    ↓
AuricHalo Standalone Input
    ↓
AuricHalo Processing
    ↓
AuricHalo Output
    ↓
Speaker/Headphones
```

---

## 🎛️ Konfigurasi Loopback untuk AuricHalo

### Di Loopback App:

#### 1. Create Virtual Device
```
Name: "AuricHalo Input"
Channels: 2 (Stereo)
```

#### 2. Add Source
```
Source: Your Audio Interface (gitar input)
atau
Source: System Audio
atau  
Source: Application Audio
```

#### 3. Route to Output
```
Output 1+2 → AuricHalo Input device
```

#### 4. Monitor
```
Enable monitoring jika ingin dengar langsung
```

---

## 🎚️ Di AuricHalo Standalone:

### Audio Settings:
```
Input Device:  [Loopback Virtual Device]
Output Device: [Your Speakers/Interface]

Input Channels:
☑ 1-2 (Stereo)

Sample Rate: Match dengan Loopback (44.1k atau 48k)
Buffer Size: 512 (balance latency/stability)
```

### Plugin Settings:
```
Input Gain: 100% (default)
Drive: 20-40% (sesuai taste)
Mix: 30-50%
Output: 100%
```

---

## 🧪 Testing Steps

### Test 1: Check Input Meter
```
1. Play gitar
2. Lihat Input Meter (bar kiri) di AuricHalo
3. Harus ada movement (hijau/kuning)
4. Jika tidak ada → Audio tidak masuk
```

### Test 2: Check Loopback Monitoring
```
1. Di Loopback app, check level meter
2. Harus ada signal dari gitar
3. Jika ada di Loopback tapi tidak di AuricHalo
   → Problem di device selection
```

### Test 3: Bypass Test
```
1. Set Mix = 0% (pure dry)
2. Harus dengar gitar langsung
3. Jika tidak dengar → Routing problem
```

### Test 4: Processing Test
```
1. Set Drive = 50%
2. Set Mix = 50%
3. Harus dengar saturation effect
4. Output meter (bar kanan) harus bergerak
```

---

## 🔊 Alternative Setup (Jika Loopback Bermasalah)

### Option 1: Direct Audio Interface
```
AuricHalo Input Device: [Your Audio Interface]
Input Channels: Pilih channel gitar (biasanya 1 atau 2)
Output Device: [Your Audio Interface]
```

### Option 2: Aggregate Device
```
1. Buka Audio MIDI Setup
2. Create Aggregate Device
3. Combine: Audio Interface + Loopback
4. Pilih Aggregate Device di AuricHalo
```

### Option 3: BlackHole (Free Alternative)
```
1. Install BlackHole (free virtual audio)
2. Setup routing seperti Loopback
3. Pilih BlackHole di AuricHalo
```

---

## 📝 Common Issues

### Issue 1: "No Input Detected"
```
Checklist:
☐ Loopback device selected?
☐ Correct input channels active?
☐ Loopback routing correct?
☐ Input Gain > 0%?
☐ Gitar/source playing?
```

### Issue 2: "Audio Crackling"
```
Solution:
- Increase buffer size (512 → 1024)
- Check sample rate match
- Close other audio apps
```

### Issue 3: "High Latency"
```
Solution:
- Decrease buffer size (512 → 256)
- Use direct audio interface (skip Loopback)
- Check Loopback pass-through settings
```

### Issue 4: "No Output Sound"
```
Checklist:
☐ Output device selected?
☐ Output Gain > 0%?
☐ Output meter moving?
☐ System volume up?
☐ Correct output channels?
```

---

## 🎯 Recommended Setup

### For Low Latency (Live Playing):
```
Input: Direct Audio Interface
Buffer: 128-256 samples
Sample Rate: 48000 Hz
Monitoring: Direct (hardware)
```

### For Recording/Processing:
```
Input: Loopback Virtual Device
Buffer: 512 samples
Sample Rate: 44100 Hz
Monitoring: Software (through AuricHalo)
```

### For Testing:
```
Input: Built-in Microphone (untuk test cepat)
Buffer: 512 samples
Sample Rate: 44100 Hz
```

---

## 🔍 Debug Commands

### Check Audio Devices:
```bash
# List all audio devices
system_profiler SPAudioDataType

# Check Loopback
ls -la ~/Library/Audio/Plug-Ins/HAL/

# Test audio input
afplay /System/Library/Sounds/Ping.aiff
```

### Check AuricHalo:
```bash
# Check if app is running
ps aux | grep AuricHalo

# Check audio permissions
tccutil reset Microphone com.auricaudio.aurichalo
```

---

## ✅ Success Checklist

Standalone app bekerja dengan baik jika:
- ☑ Input meter (kiri) bergerak saat ada audio
- ☑ Output meter (kanan) bergerak saat processing
- ☑ Dengar audio output dengan effect
- ☑ No crackling atau dropouts
- ☑ Latency acceptable

---

## 🎉 Quick Fix Summary

**Paling Sering:**
```
1. Options → Audio Settings
2. Input Device: Pilih Loopback
3. Active Channels: Enable 1-2
4. Test dengan play gitar
5. Check input meter
```

**Jika Masih Tidak Masuk:**
```
1. Restart AuricHalo
2. Restart Loopback
3. Check Loopback routing
4. Try direct audio interface
5. Check system audio permissions
```

---

## 📞 Still Not Working?

### Last Resort:
1. Quit AuricHalo
2. Delete preferences:
   ```bash
   rm -rf ~/Library/Preferences/com.auricaudio.aurichalo.plist
   ```
3. Restart AuricHalo
4. Reconfigure audio settings

### Alternative:
- Use VST3/AU in DAW instead of standalone
- DAW handles audio routing automatically
- More reliable for complex setups

---

**Note:** Standalone app memerlukan manual audio device configuration. Untuk workflow yang lebih smooth, gunakan plugin di DAW!
