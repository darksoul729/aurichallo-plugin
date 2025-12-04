# Troubleshooting: Meters & Oversampling UI

## Masalah: Level Meters & Oversampling Tidak Terlihat

### ✅ Solusi yang Sudah Diterapkan:

1. **setOpaque(false)** - Meters transparan agar tidak menutupi background
2. **setAlwaysOnTop(true)** - Memastikan meters di layer paling atas
3. **Gold Border** - Border emas (#d4af37) untuk visibility lebih baik
4. **Warna ComboBox** - Background gelap dengan outline emas

### 🔍 Cara Test:

#### Test 1: Standalone App
```bash
./test_plugin_ui.sh
```
atau
```bash
open Builds/MacOSX/build/Debug/AuricHalo.app
```

**Yang Harus Terlihat:**
- ✓ Input meter di kiri (bar vertikal dengan border emas)
- ✓ Output meter di kanan (bar vertikal dengan border emas)
- ✓ Label "Oversampling" di bawah tengah
- ✓ Dropdown box dengan pilihan Off/2x/4x

#### Test 2: VST3 di DAW
```
1. Buka DAW (Logic/Ableton/Reaper)
2. Scan plugins atau restart DAW
3. Load AuricHalo VST3
4. Periksa UI elements
```

### 🐛 Jika Masih Tidak Terlihat:

#### Debug 1: Periksa Bounds
Meters mungkin di luar visible area. Cek di `resized()`:

```cpp
// Input meter - kiri
inputMeter.setBounds(30, center.y - 150, 30, 300);

// Output meter - kanan  
outputMeter.setBounds(getWidth() - 60, center.y - 150, 30, 300);

// Oversampling
oversamplingLabel.setBounds(center.x - 60, getHeight() - 60, 120, 20);
oversamplingBox.setBounds(center.x - 50, getHeight() - 35, 100, 25);
```

#### Debug 2: Tambahkan Debug Paint
Tambahkan di `LevelMeter::paint()`:

```cpp
// Debug: Always draw something visible
g.setColour(juce::Colours::red);
g.fillRect(0, 0, 10, 10); // Red dot di pojok kiri atas
```

#### Debug 3: Periksa isVisible()
Tambahkan di `AuricHaloEditor::resized()`:

```cpp
DBG("Input meter visible: " + juce::String(inputMeter.isVisible()));
DBG("Input meter bounds: " + inputMeter.getBounds().toString());
DBG("Output meter visible: " + juce::String(outputMeter.isVisible()));
DBG("Oversampling box visible: " + juce::String(oversamplingBox.isVisible()));
```

### 📊 Expected Layout:

```
┌─────────────────────────────────────────────┐
│  [License]                                  │
│                                             │
│  ║        AURIC HALO                    ║  │
│  ║                                      ║  │
│  ║  [Tone]  [DRIVE]  [Mix]             ║  │
│  I                                      O  │ <- Meters here
│  N         (Main Knob)                  U  │
│  P                                      T  │
│  U          [Output]                    P  │
│  T                                      U  │
│  ║                                      T  │
│  ║  [Halo Engine]                       ║  │
│  ║                                      ║  │
│  ║      Oversampling                    ║  │ <- Label here
│  ║      [  2x  ▼  ]                     ║  │ <- Dropdown here
└─────────────────────────────────────────────┘
```

### 🔧 Manual Fix:

Jika masih tidak muncul, coba adjust posisi manual:

```cpp
// Di AuricHaloEditor::resized()

// Meters lebih ke tengah
int meterX_left = 50;  // Lebih ke kanan
int meterX_right = getWidth() - 80;  // Lebih ke kiri

inputMeter.setBounds(meterX_left, center.y - 150, 30, 300);
outputMeter.setBounds(meterX_right, center.y - 150, 30, 300);

// Oversampling lebih ke atas
oversamplingLabel.setBounds(center.x - 60, getHeight() - 80, 120, 20);
oversamplingBox.setBounds(center.x - 50, getHeight() - 50, 100, 25);
```

### 🎨 Styling Issues:

Jika terlihat tapi tidak bagus:

```cpp
// Meter colors
g.setColour(juce::Colour(0xff00ff00)); // Green
g.setColour(juce::Colour(0xffffff00)); // Yellow  
g.setColour(juce::Colour(0xffff0000)); // Red

// ComboBox colors
oversamplingBox.setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xff2a2a2a));
oversamplingBox.setColour(juce::ComboBox::textColourId, juce::Colours::white);
oversamplingBox.setColour(juce::ComboBox::outlineColourId, juce::Colour(0xffd4af37));
oversamplingBox.setColour(juce::ComboBox::arrowColourId, juce::Colour(0xffd4af37));
```

### 📝 Rebuild Checklist:

Setelah perubahan kode:

```bash
# 1. Resave project
~/Downloads/JUCE/Projucer.app/Contents/MacOS/Projucer --resave AuricHalo.jucer

# 2. Clean build
cd Builds/MacOSX
xcodebuild -scheme "AuricHalo - VST3" -configuration Debug clean

# 3. Build
xcodebuild -scheme "AuricHalo - VST3" -configuration Debug build

# 4. Test
open build/Debug/AuricHalo.app
```

### 🚀 Quick Test Commands:

```bash
# Test standalone
./test_plugin_ui.sh

# Check if plugin installed
ls -la ~/Library/Audio/Plug-Ins/VST3/AuricHalo.vst3
ls -la ~/Library/Audio/Plug-Ins/Components/AuricHalo.component

# View console logs (untuk DBG messages)
log stream --predicate 'process == "AuricHalo"' --level debug
```

### ✨ Success Indicators:

Plugin UI bekerja dengan baik jika:
- ✅ Meters terlihat dengan border emas
- ✅ Meters bergerak saat ada audio
- ✅ Oversampling dropdown terlihat dan bisa diklik
- ✅ Pilihan Off/2x/4x tersedia
- ✅ Setting tersimpan saat close/reopen

### 📞 Jika Masih Bermasalah:

1. Screenshot UI yang terlihat
2. Cek console untuk error messages
3. Verify bounds dengan debug output
4. Test di DAW berbeda (Logic vs Ableton)
5. Coba resize window plugin
