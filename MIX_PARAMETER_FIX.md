# Fix: Mix Parameter - Balanced Default Value

## ❌ Masalah Sebelumnya
Parameter **Mix** default-nya 100% (full wet), yang membuat efek terlalu dalam/kuat saat pertama kali load plugin.

## ✅ Solusi

### 1. Default Value Adjustment
```cpp
// Sebelum:
"mix", "Mix", 0.0f, 100.0f, 100.0f  // 100% wet (terlalu kuat)

// Sesudah:
"mix", "Mix", 0.0f, 100.0f, 50.0f   // 50% balanced
```

### 2. Output Compensation
```cpp
// Sebelum:
"output", "Output", 0.0f, 100.0f, 50.0f

// Sesudah:
"output", "Output", 0.0f, 100.0f, 70.0f  // Kompensasi untuk mix 50%
```

### 3. Equal-Power Crossfade
Mengganti linear crossfade dengan equal-power untuk transisi yang lebih smooth:

```cpp
// Sebelum (Linear):
wetData[sample] = dryData[sample] * (1.0f - mixValue) + wetData[sample] * mixValue;

// Sesudah (Equal-Power):
float dryGain = std::cos(mixValue * π/2);
float wetGain = std::sin(mixValue * π/2);
wetData[sample] = dryData[sample] * dryGain + wetData[sample] * wetGain;
```

## 📊 Perbandingan

| Setting | Sebelum | Sesudah | Keterangan |
|---------|---------|---------|------------|
| **Mix Default** | 100% | 50% | Lebih balanced |
| **Output Default** | 50% | 70% | Kompensasi level |
| **Crossfade** | Linear | Equal-Power | Lebih smooth |

## 🎯 Hasil

### Default Settings (Saat Load Plugin):
- **Drive:** 30% (subtle saturation)
- **Tone:** 50% (neutral)
- **Mix:** 50% (balanced dry/wet)
- **Output:** 70% (compensated level)
- **Oversampling:** 2x (quality/CPU balance)

### User Experience:
✅ Plugin tidak terlalu "dalam" saat pertama load  
✅ Efek lebih subtle dan musical  
✅ User bisa increase mix jika ingin efek lebih kuat  
✅ Transisi mix lebih smooth (equal-power)  

## 🔊 Equal-Power Crossfade Explained

### Linear Crossfade (Sebelum):
```
Dry: 100% → 0%
Wet: 0% → 100%
Problem: Volume dip di tengah (50/50)
```

### Equal-Power Crossfade (Sesudah):
```
Dry: cos(mix * π/2)
Wet: sin(mix * π/2)
Benefit: Constant perceived loudness
```

**Grafik:**
```
Volume
  ^
  |     Dry (cos)
  |    /‾‾‾\
  |   /     \___
  |  /          \___
  | /               \___
  |/___________________\___> Mix
  0%   25%   50%   75%  100%
       
       Wet (sin)
      ___/‾‾‾‾‾‾‾‾‾‾‾
  ___/
 /
```

## 🧪 Testing

### Test 1: Default Sound
```
1. Load plugin dengan default settings
2. Play audio
3. ✓ Efek harus subtle, tidak overwhelming
4. ✓ Dry signal masih terdengar jelas
```

### Test 2: Mix Sweep
```
1. Slowly increase Mix dari 0% → 100%
2. ✓ Transisi harus smooth tanpa volume dip
3. ✓ No sudden jumps atau clicks
```

### Test 3: Extreme Settings
```
Mix = 0%:   Pure dry signal
Mix = 50%:  Balanced (default)
Mix = 100%: Pure wet signal
```

## 📝 Recommendations untuk User

### Subtle Enhancement:
```
Drive: 20-40%
Mix: 30-50%
Output: 70-80%
```

### Moderate Saturation:
```
Drive: 40-60%
Mix: 50-70%
Output: 60-70%
```

### Heavy Processing:
```
Drive: 60-80%
Mix: 70-100%
Output: 50-60%
```

### Parallel Processing:
```
Drive: 80-100%
Mix: 20-40%
Output: 80-100%
```

## 🔧 Technical Details

### Mix Processing Flow:
```
1. Store dry buffer (before processing)
2. Process wet signal (saturation → EQ → compression)
3. Calculate crossfade gains:
   - dryGain = cos(mix * π/2)
   - wetGain = sin(mix * π/2)
4. Blend: output = dry * dryGain + wet * wetGain
5. Apply output gain
```

### Why Equal-Power?
- Maintains constant perceived loudness
- No volume dip at 50/50 mix
- More musical and professional
- Industry standard for crossfades

## ✨ Benefits

✅ **Better First Impression** - Plugin sounds good immediately  
✅ **More Musical** - Subtle by default, aggressive when needed  
✅ **Professional** - Equal-power crossfade like pro plugins  
✅ **Flexible** - Easy to dial in desired amount  
✅ **No Surprises** - Predictable behavior  

## 📦 Updated Files

```
✅ source/Parameters.cpp         - Default values changed
✅ source/AuricHaloProcessor.cpp - Equal-power crossfade
✅ Release/AuricHalo-1.1.0-macOS.pkg - Rebuilt installer
```

## 🎉 Result

Plugin sekarang memiliki default settings yang lebih balanced dan musical, dengan mix crossfade yang lebih smooth dan professional!
