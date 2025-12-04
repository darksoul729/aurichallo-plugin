# AuricHalo macOS Installer - Complete Guide

## ✅ Installer Berhasil Dibuat!

**File:** `Release/AuricHalo-1.1.0-macOS.pkg`  
**Size:** ~5.3 MB  
**Format:** macOS PKG (Universal - Apple Silicon & Intel)

---

## 📦 Isi Installer

Installer ini mencakup:

1. **VST3 Plugin** → `/Library/Audio/Plug-Ins/VST3/`
   - Untuk: Ableton Live, Reaper, FL Studio, Bitwig, dll
   
2. **Audio Unit (AU)** → `/Library/Audio/Plug-Ins/Components/`
   - Untuk: Logic Pro, GarageBand, MainStage
   
3. **Standalone App** → `/Applications/`
   - Aplikasi mandiri tanpa DAW

---

## 🚀 Cara Menggunakan Installer

### Untuk End User:

1. **Download** file `AuricHalo-1.1.0-macOS.pkg`
2. **Double-click** untuk membuka installer
3. **Ikuti wizard:**
   - Welcome screen
   - License agreement (klik "Agree")
   - Pilih komponen yang ingin diinstall:
     - ✓ VST3 Plugin
     - ✓ Audio Unit
     - ✓ Standalone Application
   - Klik "Install"
4. **Masukkan password** macOS jika diminta
5. **Selesai!** Plugin siap digunakan

### Setelah Install:

1. **Restart DAW** Anda
2. **Rescan plugins** (jika perlu)
3. **Load AuricHalo** dari plugin list

---

## 🔧 Build Installer (Untuk Developer)

### Prerequisites:
- Xcode Command Line Tools
- macOS 10.13+
- Plugins sudah di-build (Release mode)

### Build Command:
```bash
bash installer/build-installer.sh
```

### Output:
```
Release/AuricHalo-1.1.0-macOS.pkg
```

---

## 📁 Struktur Installer

```
installer/
├── build-installer.sh          # Main build script
├── scripts/
│   ├── vst3/
│   │   └── postinstall        # VST3 post-install script
│   └── au/
│       └── postinstall        # AU post-install script (clear cache)
├── resources/
│   ├── Welcome.rtf            # Welcome screen
│   ├── License.rtf            # License agreement
│   ├── ReadMe.rtf             # Installation guide
│   └── background.png         # Installer background
└── build/                     # Temporary build files
    ├── distribution.xml       # Package definition
    ├── AuricHalo-VST3.pkg
    ├── AuricHalo-AU.pkg
    └── AuricHalo-Standalone.pkg
```

---

## 🎨 Customization

### Edit Welcome Screen:
```bash
open installer/resources/Welcome.rtf
```

### Edit License:
```bash
open installer/resources/License.rtf
```

### Edit ReadMe:
```bash
open installer/resources/ReadMe.rtf
```

### Change Background:
Replace `installer/resources/background.png` dengan image 620x418px

---

## 🧪 Testing Installer

### Test Installation:
```bash
# Open installer
open Release/AuricHalo-1.1.0-macOS.pkg

# Or test from command line
sudo installer -pkg Release/AuricHalo-1.1.0-macOS.pkg -target /
```

### Verify Installation:
```bash
# Check VST3
ls -la /Library/Audio/Plug-Ins/VST3/AuricHalo.vst3

# Check AU
ls -la /Library/Audio/Plug-Ins/Components/AuricHalo.component

# Check Standalone
ls -la /Applications/AuricHalo.app

# Validate AU
auval -v aufx AuHa Aurc
```

### Uninstall (Manual):
```bash
sudo rm -rf /Library/Audio/Plug-Ins/VST3/AuricHalo.vst3
sudo rm -rf /Library/Audio/Plug-Ins/Components/AuricHalo.component
sudo rm -rf /Applications/AuricHalo.app
killall -9 AudioComponentRegistrar
rm -rf ~/Library/Caches/AudioUnitCache
```

---

## 📝 Post-Install Scripts

### VST3 Post-Install:
- Sets correct permissions (755)
- Kills running DAWs to prevent file locks

### AU Post-Install:
- Sets correct permissions (755)
- Clears Audio Unit cache
- Kills AudioComponentRegistrar
- Validates plugin with `auval`

---

## 🔐 Code Signing (Optional)

Untuk distribusi profesional, sign installer dengan Developer ID:

```bash
# Sign individual packages
productsign --sign "Developer ID Installer: Your Name" \
  Release/AuricHalo-1.1.0-macOS.pkg \
  Release/AuricHalo-1.1.0-macOS-Signed.pkg

# Verify signature
pkgutil --check-signature Release/AuricHalo-1.1.0-macOS-Signed.pkg

# Notarize (untuk macOS 10.15+)
xcrun notarytool submit Release/AuricHalo-1.1.0-macOS-Signed.pkg \
  --apple-id "your@email.com" \
  --team-id "TEAMID" \
  --password "app-specific-password" \
  --wait

# Staple notarization
xcrun stapler staple Release/AuricHalo-1.1.0-macOS-Signed.pkg
```

---

## 🌐 Distribution

### Upload Locations:
- Website download page
- GitHub Releases
- Plugin marketplaces
- Cloud storage (Dropbox, Google Drive)

### Recommended Naming:
```
AuricHalo-1.1.0-macOS.pkg           # Main installer
AuricHalo-1.1.0-macOS-Signed.pkg    # Signed version
AuricHalo-1.1.0-macOS-Intel.pkg     # Intel only (if needed)
AuricHalo-1.1.0-macOS-AppleSilicon.pkg  # ARM only (if needed)
```

---

## 📊 Installer Features

✅ **Custom Welcome Screen** - Branding & version info  
✅ **License Agreement** - EULA with accept/decline  
✅ **ReadMe** - Installation instructions  
✅ **Component Selection** - Choose VST3/AU/Standalone  
✅ **Post-Install Scripts** - Auto-configure plugins  
✅ **Universal Binary** - Apple Silicon & Intel support  
✅ **System-wide Install** - Available to all users  
✅ **Professional Appearance** - Custom background & styling  

---

## 🐛 Troubleshooting

### Installer won't open:
```bash
# Remove quarantine attribute
xattr -d com.apple.quarantine Release/AuricHalo-1.1.0-macOS.pkg
```

### "Unidentified Developer" warning:
- Right-click → Open
- Or: System Preferences → Security → Allow

### Plugins not showing after install:
1. Restart DAW
2. Rescan plugins in DAW preferences
3. Check installation paths
4. Clear AU cache (for Logic/GarageBand)

### Build script fails:
```bash
# Check permissions
chmod +x installer/build-installer.sh
chmod +x installer/scripts/*/postinstall

# Check if plugins exist
ls -la Builds/MacOSX/build/Release/
```

---

## 📞 Support

For installation issues:
1. Check installation paths
2. Verify system requirements
3. Review console logs
4. Contact support with:
   - macOS version
   - DAW name & version
   - Error messages
   - Installation log

---

## 🎉 Success!

Installer PKG lengkap sudah siap untuk distribusi!

**Next Steps:**
1. Test installer di clean macOS
2. Test di berbagai DAW
3. (Optional) Code sign & notarize
4. Upload & distribute
5. Update website/documentation

---

## 📄 Files Created

```
✅ Release/AuricHalo-1.1.0-macOS.pkg    (5.3 MB)
✅ installer/build-installer.sh
✅ installer/scripts/vst3/postinstall
✅ installer/scripts/au/postinstall
✅ installer/resources/Welcome.rtf
✅ installer/resources/License.rtf
✅ installer/resources/ReadMe.rtf
✅ installer/resources/background.png
```

**Ready for distribution! 🚀**
