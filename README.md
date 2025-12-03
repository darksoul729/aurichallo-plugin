# Auric Halo

Professional audio plugin dengan EQ, compression, saturation, dan algoritma Halo signature.

## Cara Build

### Prerequisites
- Visual Studio 2022 (atau versi lain yang Anda punya)
- JUCE Framework (sudah terinstall di `../../JUCE/modules`)
- Projucer (untuk generate Visual Studio project)

### Langkah Build

1. **Buka Projucer**
   - Jalankan Projucer dari instalasi JUCE Anda
   
2. **Open Project**
   - File → Open → Pilih `AuricHalo.jucer`
   
3. **Set JUCE Path (jika perlu)**
   - File → Global Paths
   - Pastikan path JUCE modules sudah benar
   
4. **Generate Visual Studio Project**
   - Klik "Save Project and Open in IDE" atau tekan Ctrl+Shift+S
   - Projucer akan generate project di folder `Builds/VisualStudio2022/`
   
5. **Build di Visual Studio**
   - Visual Studio akan terbuka otomatis
   - Pilih configuration: Debug atau Release
   - Build → Build Solution (F7)

### Output Plugin
- **VST3**: `Builds/VisualStudio2022/x64/Debug/VST3/AuricHalo.vst3`
- **Standalone**: `Builds/VisualStudio2022/x64/Debug/Standalone Plugin/AuricHalo.exe`

## Features
- Custom Halo EQ algorithm
- Transparent compression
- Harmonic saturation
- Real-time metering
- Preset management

## Struktur Project
```
source/
├── AuricHaloProcessor.h/cpp  # Audio processor utama
├── AuricHaloEditor.h/cpp     # GUI editor
├── Parameters.h/cpp          # Parameter management
├── DSP/                      # DSP algorithms
│   ├── HaloEQ.*
│   ├── HaloCompressor.*
│   └── HaloSaturator.*
└── UI/                       # UI components
    ├── LookAndFeel.*
    └── Components/
        ├── HaloMeter.*
        └── HaloKnob.*
```

## Troubleshooting

**Error: Cannot find JUCE modules**
- Buka Projucer → File → Global Paths
- Set JUCE Modules path ke folder JUCE modules Anda

**Error: Missing header files**
- Pastikan semua file source ada di folder yang benar
- Re-save project di Projucer

## Architecture
Lihat [ARCHITECTURE.md](ARCHITECTURE.md) untuk dokumentasi teknis lengkap.
