# Fix: GarageBand AU Plugin - Audio Track Support

## ❌ Masalah Sebelumnya
Plugin AuricHalo hanya muncul di **MIDI/Instrument tracks** di GarageBand, tidak bisa digunakan di **Audio tracks**.

## ✅ Solusi
Menambahkan `pluginAUMainType="'aufx'"` di file `.jucer` untuk menandai plugin sebagai **Audio Unit Effect**.

## 🔧 Perubahan di AuricHalo.jucer

### Sebelum:
```xml
<JUCERPROJECT ... 
  pluginAUExportPrefix="AuricHaloAU"
  pluginRTASCategory="2" ...>
```

### Sesudah:
```xml
<JUCERPROJECT ... 
  pluginAUExportPrefix="AuricHaloAU"
  pluginAUMainType="'aufx'"
  pluginRTASCategory="2" ...>
```

## 📋 AU Main Types

| Type | Code | Deskripsi | Muncul di Track |
|------|------|-----------|-----------------|
| **'aufx'** | Audio Effect | Effect plugin untuk audio | ✅ Audio tracks |
| **'aumu'** | Music Effect | Effect untuk MIDI/instrument | ❌ MIDI tracks only |
| **'aumf'** | Format Converter | Format conversion | Audio tracks |
| **'auol'** | Offline Effect | Offline processing | Audio tracks |
| **'aumi'** | Music Instrument | Synth/instrument | Instrument tracks |

## 🚀 Steps yang Dilakukan

### 1. Edit .jucer file
```bash
# Tambahkan pluginAUMainType="'aufx'" di AuricHalo.jucer
```

### 2. Regenerate project
```bash
~/Downloads/JUCE/Projucer.app/Contents/MacOS/Projucer --resave AuricHalo.jucer
```

### 3. Rebuild AU
```bash
cd Builds/MacOSX
xcodebuild -scheme "AuricHalo - AU" -configuration Release clean build
```

### 4. Clear AU cache
```bash
# Kill AU cache process
killall -9 AudioComponentRegistrar

# Remove cache folder
rm -rf ~/Library/Caches/AudioUnitCache
```

### 5. Verify plugin
```bash
# Check if registered correctly
auval -a | grep -i auric

# Output harus:
# aufx AuHa Aurc  -  Auric Audio: AuricHalo
#  ^^^^
#  Ini harus 'aufx' bukan 'aumu'
```

### 6. Validate plugin
```bash
auval -v aufx AuHa Aurc

# Harus PASS semua tests
```

## ✅ Hasil

Plugin sekarang muncul di:
- ✅ **Audio tracks** di GarageBand
- ✅ **Audio tracks** di Logic Pro
- ✅ **Audio tracks** di semua DAW yang support AU
- ✅ Tetap bisa digunakan di instrument tracks (jika ada audio output)

## 🎯 Testing di GarageBand

### Test 1: Audio Track
```
1. Buka GarageBand
2. Create Audio Track (bukan Software Instrument)
3. Klik Smart Controls atau Plug-ins & EQ
4. Pilih Audio Units
5. Cari "Auric Audio" → "AuricHalo"
6. ✅ Plugin harus muncul dan bisa di-load
```

### Test 2: Instrument Track
```
1. Create Software Instrument track
2. Klik Plug-ins & EQ
3. Pilih Audio Units
4. ✅ Plugin juga bisa muncul di sini (optional)
```

## 🔍 Troubleshooting

### Plugin masih tidak muncul?

#### 1. Restart GarageBand
```bash
killall GarageBand
```

#### 2. Rescan plugins
```
GarageBand → Preferences → Audio/MIDI → Reset & Rescan Selection
```

#### 3. Check plugin location
```bash
ls -la ~/Library/Audio/Plug-Ins/Components/AuricHalo.component
```

#### 4. Verify AU type
```bash
auval -a | grep -i auric
# Harus menunjukkan 'aufx' bukan 'aumu'
```

#### 5. Full validation
```bash
auval -v aufx AuHa Aurc
# Semua tests harus PASS
```

#### 6. Check console for errors
```bash
log stream --predicate 'process == "GarageBand"' --level debug
```

### Plugin crash atau error?

#### Re-validate
```bash
auval -v aufx AuHa Aurc
```

#### Check for conflicts
```bash
# List all AuricHalo instances
find ~/Library/Audio/Plug-Ins -name "*Auric*"
```

#### Reinstall
```bash
# Remove old plugin
rm -rf ~/Library/Audio/Plug-Ins/Components/AuricHalo.component

# Copy new one
cp -R Builds/MacOSX/build/Release/AuricHalo.component ~/Library/Audio/Plug-Ins/Components/

# Clear cache
killall -9 AudioComponentRegistrar
rm -rf ~/Library/Caches/AudioUnitCache
```

## 📝 Notes

### Compatibility
- ✅ macOS 10.13+
- ✅ GarageBand 10.x
- ✅ Logic Pro X/11
- ✅ All AU-compatible DAWs

### Plugin Settings
```
Type: Audio Unit Effect (aufx)
Channels: Stereo (2 in, 2 out)
MIDI: Not required
Category: Effect
```

### Build Configurations
- Debug: `build/Debug/AuricHalo.component`
- Release: `build/Release/AuricHalo.component`
- Installed: `~/Library/Audio/Plug-Ins/Components/AuricHalo.component`

## ✨ Success Indicators

Plugin bekerja dengan benar jika:
- ✅ `auval -a` menunjukkan 'aufx' (bukan 'aumu')
- ✅ `auval -v aufx AuHa Aurc` PASS semua tests
- ✅ Muncul di Audio tracks di GarageBand
- ✅ Bisa di-load tanpa crash
- ✅ Audio processing berfungsi normal
- ✅ UI muncul dengan benar

## 🎉 Selesai!

Plugin AuricHalo sekarang bisa digunakan di **semua jenis audio tracks** di GarageBand dan Logic Pro!
