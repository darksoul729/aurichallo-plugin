# Feature Added: Input Gain Knob

## ✅ New Control: INPUT

Menambahkan **Input Gain** knob untuk kontrol pre-gain sebelum processing.

---

## 🎛️ Layout Baru

### Sebelum (4 knobs):
```
        [TONE]    [DRIVE]    [MIX]
                             [OUTPUT]
```

### Sesudah (5 knobs):
```
       [INPUT]    [DRIVE]    [MIX]
        [TONE]               [OUTPUT]
```

**Posisi:**
- **Kiri Atas:** INPUT (baru!)
- **Kiri Bawah:** TONE
- **Tengah:** DRIVE (main knob)
- **Kanan Atas:** MIX
- **Kanan Bawah:** OUTPUT

---

## 🎯 Fungsi INPUT Knob

### Purpose:
- **Pre-gain** sebelum saturation
- Kontrol "how hard you hit the saturation"
- Seperti gain staging di analog gear

### Range:
- **0%:** Silent
- **80%:** Default (balanced)
- **100%:** Unity gain

### Use Cases:

#### 1. Subtle Processing (Default)
```
Input: 80%
Drive: 20%
Result: Gentle saturation
```

#### 2. Hit Harder
```
Input: 100%
Drive: 30-40%
Result: More saturation character
```

#### 3. Tame Hot Signals
```
Input: 60-70%
Drive: 20-30%
Result: Control overly hot input
```

#### 4. Parallel Saturation
```
Input: 100%
Drive: 80%
Mix: 30%
Result: Heavy saturation blended subtly
```

---

## 🔊 Signal Flow

```
Audio Input
    ↓
[INPUT GAIN] ← New!
    ↓
[Store Dry for Mix]
    ↓
[DRIVE Saturation]
    ↓
[TONE EQ]
    ↓
[Compression]
    ↓
[MIX Blend with Dry]
    ↓
[OUTPUT GAIN]
    ↓
Audio Output
```

---

## 📊 Comparison

### Without Input Control (Before):
```
Hot Signal → Saturation → Too much distortion
Quiet Signal → Saturation → Not enough character
```
**Problem:** No way to control input level

### With Input Control (After):
```
Hot Signal → Input 60% → Saturation → Controlled
Quiet Signal → Input 100% → Saturation → More character
```
**Solution:** ✅ Full control over gain staging

---

## 🎚️ Gain Staging Tips

### Mastering:
```
Input: 70-80%
Drive: 15-20%
Mix: 25-35%
Output: 80-85%
```
**Goal:** Subtle glue without coloring too much

### Mixing - Drums:
```
Input: 90-100%
Drive: 30-50%
Mix: 40-60%
Output: 70-80%
```
**Goal:** Punch and character

### Mixing - Vocals:
```
Input: 75-85%
Drive: 20-30%
Mix: 30-45%
Output: 75-85%
```
**Goal:** Warmth without harshness

### Creative FX:
```
Input: 100%
Drive: 80-100%
Mix: 70-100%
Output: 60-70%
```
**Goal:** Heavy distortion

---

## 🔧 Technical Implementation

### Parameter:
```cpp
layout.add(std::make_unique<juce::AudioParameterFloat>(
    "input", "Input", 0.0f, 100.0f, 80.0f)); // Default: 80%
```

### Processing:
```cpp
// Apply input gain BEFORE saturation
float inputValue = *parameters.getRawParameterValue("input") / 100.0f;

for (int channel = 0; channel < totalNumOutputChannels; ++channel)
{
    auto* channelData = buffer.getWritePointer(channel);
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        channelData[sample] *= inputValue;
    }
}

// Then store dry signal for mix
dryBuffer.makeCopyOf(buffer);

// Then process saturation...
```

---

## 🎯 Why Input Gain Matters

### 1. Gain Staging
- Control how hard you hit the saturation
- Like turning up preamp gain on analog gear

### 2. Flexibility
- Tame hot signals
- Boost quiet signals
- Match different source levels

### 3. Creative Control
- Drive saturation harder without increasing Drive knob
- More natural saturation response

### 4. Professional Workflow
- Proper gain staging = better sound
- Industry standard practice

---

## 📝 Updated Default Settings

```
Input:       80%  ← New!
Drive:       20%
Tone:        50%
Mix:         30%
Output:      80%
Oversampling: 2x
```

**Philosophy:** Balanced, subtle, professional

---

## 🎨 UI Layout

```
┌─────────────────────────────────────────────┐
│  [License]                                  │
│                                             │
│  ║        AURIC HALO                    ║  │
│  ║                                      ║  │
│  ║ [INPUT]  [DRIVE]  [MIX]             ║  │
│  I                                      O  │
│  N  [TONE]           [OUTPUT]           U  │
│  P                                      T  │
│  U                                      P  │
│  T  [Halo]                              U  │
│  ║                                      T  │
│  ║      Oversampling                    ║  │
│  ║      [  2x  ▼  ]                     ║  │
└─────────────────────────────────────────────┘
```

---

## 🧪 Testing Scenarios

### Test 1: Normal Level
```
Input: 80% (default)
Expected: Balanced saturation
```

### Test 2: Hot Signal
```
Input: 60%
Expected: Tamed, controlled
```

### Test 3: Quiet Signal
```
Input: 100%
Expected: More character
```

### Test 4: Extreme
```
Input: 100%
Drive: 100%
Expected: Heavy distortion
```

---

## ✨ Benefits

✅ **Better Control** - Full gain staging  
✅ **More Flexible** - Handle any input level  
✅ **Professional** - Industry standard workflow  
✅ **Creative** - More saturation options  
✅ **Practical** - Solve real-world problems  

---

## 📦 Files Modified

```
✅ source/Parameters.cpp         - Added input parameter
✅ source/AuricHaloProcessor.cpp - Added input gain processing
✅ source/AuricHaloEditor.h      - Added inputKnob
✅ source/AuricHaloEditor.cpp    - Added UI and layout
✅ Release/AuricHalo-1.1.0-macOS.pkg - Rebuilt
```

---

## 🎉 Complete!

Plugin sekarang memiliki **5 knobs** untuk kontrol penuh:

1. **INPUT** - Pre-gain (how hard to hit)
2. **TONE** - Tonal character
3. **DRIVE** - Saturation amount (main)
4. **MIX** - Dry/wet blend
5. **OUTPUT** - Final level

**Total Control = Professional Results!** 🚀
