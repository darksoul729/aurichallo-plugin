# 🚀 QUICK START - Auric Halo

## Cara Tercepat untuk Build

### Opsi 1: Double-click (Paling Mudah)
```
1. Double-click: open_in_projucer.bat
2. Di Projucer, klik tombol Visual Studio (atau Ctrl+Shift+S)
3. Di Visual Studio, tekan F7 untuk build
```

### Opsi 2: Manual
```
1. Buka Projucer.exe
2. File → Open → Pilih AuricHalo.jucer
3. Klik "Save Project and Open in IDE"
4. Di Visual Studio: Build → Build Solution (F7)
```

## Hasil Build

Plugin akan ada di folder:
```
Builds/VisualStudio2022/x64/Debug/VST3/AuricHalo.vst3
Builds/VisualStudio2022/x64/Release/VST3/AuricHalo.vst3
```

## Testing Plugin

### Test di Standalone
```
Builds/VisualStudio2022/x64/Debug/Standalone Plugin/AuricHalo.exe
```

### Test di DAW
Copy file `.vst3` ke folder VST3 Anda:
```
C:\Program Files\Common Files\VST3\
```

## Struktur Code

```
source/
├── AuricHaloProcessor.cpp    ← Audio processing logic
├── AuricHaloEditor.cpp       ← GUI layout & controls
├── Parameters.cpp            ← Plugin parameters
├── DSP/
│   ├── HaloEQ.cpp           ← EQ algorithm
│   ├── HaloCompressor.cpp   ← Compressor
│   └── HaloSaturator.cpp    ← Saturation
└── UI/
    ├── LookAndFeel.cpp      ← Visual styling
    └── Components/
        ├── HaloMeter.cpp    ← Level meter
        └── HaloKnob.cpp     ← Rotary knobs
```

## Modifikasi Code

1. Edit file `.cpp` atau `.h` di folder `source/`
2. Save changes
3. Di Visual Studio: Build → Build Solution (F7)
4. Test plugin

**PENTING**: Jangan edit file di folder `Builds/` atau `JuceLibraryCode/` - file ini auto-generated!

## Tips Development

- **Debug Mode**: Lebih lambat tapi bisa debug dengan breakpoints
- **Release Mode**: Optimized, untuk distribusi final
- **Hot Reload**: Beberapa DAW support reload plugin tanpa restart

## Next Steps

1. ✅ Build project (ikuti langkah di atas)
2. 📝 Modifikasi DSP algorithms di `source/DSP/`
3. 🎨 Customize UI di `source/UI/`
4. 🎛️ Tambah parameters di `Parameters.cpp`
5. 🎵 Test di DAW favorit Anda

## Butuh Bantuan?

- Baca: `SETUP_INSTRUKSI.txt` untuk troubleshooting
- Baca: `ARCHITECTURE.md` untuk detail teknis
- Baca: `README.md` untuk dokumentasi lengkap
