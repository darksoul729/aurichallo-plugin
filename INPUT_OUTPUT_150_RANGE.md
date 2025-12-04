# Update: Input/Output Range Extended to 150%

## ✅ Change: Extended Gain Range

Input dan Output knobs sekarang memiliki range **0-150%** (sebelumnya 0-100%)

---

## 🎛️ New Parameter Ranges

### Before:
```
Input:  0-100% (default 80%)
Output: 0-100% (default 80%)
```

### After:
```
Input:  0-150% (default 100%)  ← Extended!
Output: 0-150% (default 100%)  ← Extended!
```

---

## 🎯 Benefits

### 1. **More Headroom**
- Can boost signal up to +50% (150%)
- Useful for quiet sources
- More gain staging flexibility

### 2. **Unity Gain at 100%**
- 100% = 0dB (no change)
- More intuitive than 80%
- Industry standard

### 3. **Boost Capability**
- 100-150% = Boost range
- Great for quiet recordings
- Drive saturation harder

### 4. **Cut Capability**
- 0-100% = Cut range
- Tame hot signals
- Prevent clipping

---

## 📊 Gain Levels

| Setting | Gain | dB | Use Case |
|---------|------|-----|----------|
| **0%** | 0.0x | -∞ dB | Mute |
| **50%** | 0.5x | -6 dB | Significant cut |
| **75%** | 0.75x | -2.5 dB | Moderate cut |
| **100%** | 1.0x | 0 dB | **Unity (default)** |
| **125%** | 1.25x | +2 dB | Moderate boost |
| **150%** | 1.5x | +3.5 dB | Maximum boost |

---

## 🎚️ Practical Examples

### Example 1: Quiet Vocal
```
Input: 120-130%  (boost quiet source)
Drive: 25%
Mix: 35%
Output: 100%
Result: Proper level + saturation
```

### Example 2: Hot Drums
```
Input: 80-90%   (tame hot signal)
Drive: 40%
Mix: 50%
Output: 100%
Result: Controlled saturation
```

### Example 3: Mastering
```
Input: 100%     (unity)
Drive: 15%
Mix: 25%
Output: 95-105% (fine-tune final level)
Result: Subtle glue
```

### Example 4: Aggressive Saturation
```
Input: 130-150% (hit hard!)
Drive: 60%
Mix: 70%
Output: 80-90%  (compensate)
Result: Heavy character
```

---

## 🔊 Signal Flow with Extended Range

```
Audio Input
    ↓
[INPUT: 0-150%] ← Can boost or cut
    ↓
[DRIVE Saturation]
    ↓
[TONE EQ]
    ↓
[Compression]
    ↓
[MIX Blend]
    ↓
[OUTPUT: 0-150%] ← Can boost or cut
    ↓
Audio Output
```

---

## 💡 Usage Tips

### Input Gain:

**0-80%:** Cut Range
- Tame hot signals
- Prevent distortion
- Control dynamics

**80-100%:** Normal Range
- Standard operation
- Most common use

**100-120%:** Moderate Boost
- Quiet sources
- More saturation character
- Gentle push

**120-150%:** Heavy Boost
- Very quiet sources
- Aggressive saturation
- Creative effects

### Output Gain:

**0-80%:** Cut Range
- Compensate for hot processing
- Prevent clipping
- Parallel processing

**80-100%:** Normal Range
- Standard operation
- Level matching

**100-120%:** Moderate Boost
- Compensate for subtle processing
- Make up gain
- Final polish

**120-150%:** Heavy Boost
- Significant make-up gain
- Loud mastering
- Creative loudness

---

## 🎯 Default Settings Explained

### Why 100% Default?

**Before (80% default):**
- Confusing (why not 100%?)
- Not unity gain
- Non-standard

**After (100% default):**
- ✅ Unity gain (0dB)
- ✅ Intuitive
- ✅ Industry standard
- ✅ Easy to understand

### Workflow:

1. **Start at 100%** (unity)
2. **Adjust Input** if source is too quiet/hot
3. **Process** with Drive/Tone/Mix
4. **Adjust Output** to match input level
5. **Fine-tune** both for desired result

---

## 🔧 Technical Details

### Parameter Definition:
```cpp
// Input: 0-150%, default 100%
layout.add(std::make_unique<juce::AudioParameterFloat>(
    "input", "Input", 0.0f, 150.0f, 100.0f));

// Output: 0-150%, default 100%
layout.add(std::make_unique<juce::AudioParameterFloat>(
    "output", "Output", 0.0f, 150.0f, 100.0f));
```

### Processing:
```cpp
// Convert percentage to gain multiplier
float inputGain = inputValue / 100.0f;   // 100% = 1.0x
float outputGain = outputValue / 100.0f; // 100% = 1.0x

// Apply gains
signal *= inputGain;   // 0-1.5x
// ... processing ...
signal *= outputGain;  // 0-1.5x
```

---

## 📈 Comparison Chart

### Gain Curve:
```
Gain Multiplier
  ^
1.5x |                    ___/150%
     |                __/
1.25x|            __/
     |        __/
1.0x |____/‾‾‾‾ 100% (Unity)
     |  /
0.75x|/
     |
0.5x |
     |
  0x |________________________> Setting
     0%   50%   100%  125%  150%
```

---

## 🎨 UI Display

Knobs akan menampilkan:
- **0-150%** range
- **100%** di posisi tengah (12 o'clock)
- **0%** di posisi minimum (7 o'clock)
- **150%** di posisi maximum (5 o'clock)

---

## ⚠️ Important Notes

### Clipping Prevention:
- Watch output meter!
- Input 150% + Drive 100% = Very hot signal
- Use Output to compensate

### Gain Staging:
- Start with Input/Output at 100%
- Adjust as needed
- Match input/output levels

### Creative Use:
- Input 150% + Low Mix = Parallel saturation
- Input 150% + High Drive = Aggressive distortion
- Output 150% = Loud mastering (watch for clipping!)

---

## 🧪 Testing Scenarios

### Test 1: Unity Gain
```
Input: 100%
Drive: 0%
Mix: 100%
Output: 100%
Expected: No change in level
```

### Test 2: Maximum Boost
```
Input: 150%
Output: 150%
Expected: +7dB total boost
```

### Test 3: Quiet Source
```
Input: 130%
Drive: 25%
Mix: 40%
Output: 100%
Expected: Proper level + character
```

### Test 4: Hot Source
```
Input: 70%
Drive: 30%
Mix: 40%
Output: 100%
Expected: Controlled, no clipping
```

---

## ✨ Benefits Summary

✅ **More Flexible** - 0-150% range  
✅ **Intuitive** - 100% = unity gain  
✅ **Professional** - Industry standard  
✅ **Boost Capability** - Up to +3.5dB  
✅ **Cut Capability** - Down to -∞dB  
✅ **Better Gain Staging** - Full control  
✅ **Creative Options** - More headroom  

---

## 📦 Updated Files

```
✅ source/Parameters.cpp - Extended ranges
✅ VST3 Plugin - Rebuilt
✅ AU Plugin - Rebuilt
✅ Installer PKG - Updated
```

---

## 🎉 Result

Input dan Output sekarang memiliki **50% extra headroom** untuk gain staging yang lebih fleksibel!

**Perfect for:**
- Quiet sources (boost with Input)
- Hot sources (cut with Input)
- Make-up gain (boost with Output)
- Level matching (adjust Output)
- Creative processing (push limits!)

**Default: 100% = Unity Gain = Professional Standard** ✅
