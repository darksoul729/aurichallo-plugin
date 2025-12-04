# Fitur Baru: Input/Output Meters & Oversampling

## ✅ Fitur yang Ditambahkan

### 1. Input/Output Level Meters
- **Lokasi**: Bar vertikal di kiri (Input) dan kanan (Output) UI
- **Fitur**:
  - 20 segmen LED-style meter
  - Color gradient: Hijau → Kuning → Merah
  - Smooth decay animation
  - Real-time level monitoring
  - Label "IN" dan "OUT"

### 2. Oversampling
- **Pilihan**: Off, 2x, 4x
- **Lokasi**: ComboBox di bagian bawah tengah UI
- **Manfaat**:
  - Mengurangi aliasing pada saturation
  - Kualitas audio lebih baik pada drive tinggi
  - Harmonik lebih bersih
  - Trade-off: CPU usage lebih tinggi

## 📁 File yang Ditambahkan

```
source/UI/LevelMeter.h          - Header untuk komponen meter
source/UI/LevelMeter.cpp        - Implementasi level meter dengan LED segments
```

## 🔧 File yang Dimodifikasi

```
source/AuricHaloProcessor.h     - Tambah oversampling & level metering
source/AuricHaloProcessor.cpp   - Implementasi oversampling & capture levels
source/AuricHaloEditor.h        - Tambah UI components (meters & selector)
source/AuricHaloEditor.cpp      - Layout & update meters
source/Parameters.cpp           - Tambah parameter "oversampling"
AuricHalo.jucer                 - Registrasi file baru ke project
```

## 🎨 UI Layout

```
┌─────────────────────────────────────────────┐
│  [License]                                  │
│                                             │
│  ║        AURIC HALO                    ║  │
│  ║                                      ║  │
│  ║  [Tone]  [DRIVE]  [Mix]             ║  │
│  I                                      O  │
│  N         (Main Knob)                  U  │
│  P                                      T  │
│  U          [Output]                    P  │
│  T                                      U  │
│  ║                                      T  │
│  ║  [Halo Engine]                       ║  │
│  ║                                      ║  │
│  ║      [Oversampling: 2x ▼]           ║  │
└─────────────────────────────────────────────┘
```

## 🚀 Cara Menggunakan

### Level Meters
- Meters otomatis menampilkan level input dan output
- Hijau = level aman
- Kuning = mendekati clipping
- Merah = high level (perhatikan output gain)

### Oversampling
1. Klik dropdown "Oversampling" di bawah tengah
2. Pilih:
   - **Off**: No oversampling (CPU terendah)
   - **2x**: 2x oversampling (recommended, balance bagus)
   - **4x**: 4x oversampling (kualitas tertinggi, CPU tinggi)
3. Setting tersimpan dengan preset

## ⚡ Performance

| Mode | CPU Usage | Kualitas | Rekomendasi |
|------|-----------|----------|-------------|
| Off  | Rendah    | Baik     | Mixing/tracking banyak track |
| 2x   | Sedang    | Sangat Baik | **Default - recommended** |
| 4x   | Tinggi    | Excellent | Mastering, final bounce |

## 🔊 Technical Details

### Oversampling Implementation
- Menggunakan `juce::dsp::Oversampling`
- Filter: Half-band polyphase IIR (steep)
- Processing chain di oversampled rate:
  1. Upsample
  2. Saturation → EQ → Compression
  3. Downsample

### Level Metering
- Update rate: 30 Hz
- Decay rate: 0.95 (smooth falloff)
- Peak detection per channel
- Thread-safe atomic variables

## 📝 Build Notes

Build berhasil dengan:
- Xcode (macOS) - Debug & Release
- VST3, AU, Standalone formats
- File sudah teregistrasi di AuricHalo.jucer

## 🎯 Next Steps (Optional)

Fitur tambahan yang bisa dikembangkan:
- [ ] Peak hold indicator pada meters
- [ ] Numeric dB display
- [ ] Oversampling quality selector (linear phase vs minimum phase)
- [ ] Meter calibration (dBFS scale)
- [ ] Clip indicator dengan reset button
