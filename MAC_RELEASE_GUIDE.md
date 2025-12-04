# 🍎 Panduan Release Mac Installer - Auric Halo

## Langkah-langkah Release ke Mac Installer

### 1️⃣ Build Release Version

Pastikan Anda sudah build versi Release (bukan Debug):

```bash
cd scripts
chmod +x one_click_build_mac.sh
./one_click_build_mac.sh
```

Ini akan:
- Clean previous build
- Build Release configuration
- Install plugins ke lokasi system

### 2️⃣ Buat Installer PKG

Setelah build sukses, buat installer:

```bash
chmod +x create_mac_installer.sh
./create_mac_installer.sh
```

Atau jalankan semuanya sekaligus:

```bash
chmod +x build_and_package_mac.sh
./build_and_package_mac.sh
```

### 3️⃣ Hasil Installer

File installer akan dibuat di root project:

```
AuricHalo-1.0.0-macOS.pkg
```

Installer ini akan menginstall:
- ✅ VST3 → `~/Library/Audio/Plug-Ins/VST3/`
- ✅ AU → `~/Library/Audio/Plug-Ins/Components/`
- ✅ AAX → `~/Library/Application Support/Avid/Audio/Plug-Ins/`
- ✅ Standalone → `/Applications/`

---

## 🧪 Testing Installer

### Test di Mac Anda sendiri:

```bash
# 1. Hapus plugin lama (optional)
rm -rf ~/Library/Audio/Plug-Ins/VST3/AuricHalo.vst3
rm -rf ~/Library/Audio/Plug-Ins/Components/AuricHalo.component

# 2. Install dari PKG
open AuricHalo-1.0.0-macOS.pkg

# 3. Test di DAW
# - Logic Pro (AU)
# - Ableton Live (VST3)
# - Pro Tools (AAX)
```

### Test di Mac lain (Clean Install):

1. Copy `AuricHalo-1.0.0-macOS.pkg` ke Mac lain
2. Double-click installer
3. Follow wizard
4. Test di DAW

---

## 🔐 Code Signing (Untuk Distribusi Publik)

Jika mau distribusi ke user lain, perlu code signing agar tidak kena Gatekeeper warning.

### Opsi 1: Ad-hoc Signing (Untuk testing)

```bash
# Sign VST3
codesign --force --deep --sign - ~/Library/Audio/Plug-Ins/VST3/AuricHalo.vst3

# Sign AU
codesign --force --deep --sign - ~/Library/Audio/Plug-Ins/Components/AuricHalo.component

# Sign PKG
productsign --sign - AuricHalo-1.0.0-macOS.pkg AuricHalo-1.0.0-macOS-signed.pkg
```

### Opsi 2: Developer ID (Untuk distribusi publik)

Butuh Apple Developer Account ($99/tahun):

```bash
# Sign dengan Developer ID
codesign --force --deep --sign "Developer ID Application: Your Name" AuricHalo.vst3

# Sign PKG
productsign --sign "Developer ID Installer: Your Name" AuricHalo-1.0.0-macOS.pkg AuricHalo-1.0.0-macOS-signed.pkg

# Notarize (submit ke Apple)
xcrun notarytool submit AuricHalo-1.0.0-macOS-signed.pkg --keychain-profile "AC_PASSWORD" --wait

# Staple notarization
xcrun stapler staple AuricHalo-1.0.0-macOS-signed.pkg
```

---

## 📦 Distribusi

### Opsi 1: Direct Download (Paling Mudah)

1. Upload `AuricHalo-1.0.0-macOS.pkg` ke website/cloud
2. User download & install
3. Jika tidak signed, user perlu:
   - Right-click → Open (first time)
   - Atau: System Settings → Privacy & Security → Allow

### Opsi 2: DMG (Lebih Professional)

Buat disk image untuk distribusi:

```bash
# Create DMG
hdiutil create -volname "Auric Halo" -srcfolder AuricHalo-1.0.0-macOS.pkg -ov -format UDZO AuricHalo-1.0.0-macOS.dmg
```

User tinggal:
1. Open DMG
2. Drag installer ke Applications
3. Run installer

### Opsi 3: Mac App Store

Untuk distribusi via App Store (lebih kompleks):
- Butuh Mac App Store guidelines compliance
- Apple review process
- 30% revenue share

---

## 🎯 Checklist Release

Sebelum distribusi, pastikan:

### Build Quality
- [ ] Build dalam Release mode (bukan Debug)
- [ ] Test semua plugin formats (VST3, AU, AAX)
- [ ] Test standalone app
- [ ] No crash atau error

### Compatibility Testing
- [ ] Test di Intel Mac
- [ ] Test di Apple Silicon Mac (M1/M2/M3)
- [ ] Test di berbagai macOS versions (10.13+)
- [ ] Test di berbagai DAWs:
  - [ ] Logic Pro
  - [ ] Ableton Live
  - [ ] Pro Tools
  - [ ] Cubase
  - [ ] Studio One
  - [ ] Reaper

### Installer Testing
- [ ] Installer berjalan tanpa error
- [ ] Plugins terinstall di lokasi yang benar
- [ ] Plugins muncul di DAW setelah rescan
- [ ] Uninstall berfungsi (jika ada)

### Documentation
- [ ] README included
- [ ] User manual (optional)
- [ ] License agreement
- [ ] System requirements jelas

---

## 🚀 Quick Commands

```bash
# Setup permissions (first time only)
cd scripts
chmod +x *.sh

# Build + Create Installer (All-in-one)
./build_and_package_mac.sh

# Atau step-by-step:
./one_click_build_mac.sh          # Build saja
./create_mac_installer.sh         # Installer saja

# Test installer
open AuricHalo-1.0.0-macOS.pkg

# Check file size
ls -lh AuricHalo-1.0.0-macOS.pkg
```

---

## 📊 File Sizes (Estimasi)

- VST3: ~5-10 MB
- AU: ~5-10 MB
- AAX: ~5-10 MB
- Standalone: ~5-10 MB
- **Total PKG: ~20-40 MB**

---

## ⚠️ Troubleshooting

### "Xcode project not found"
```bash
# Generate Xcode project dulu di Projucer
# Open AuricHalo.jucer → Save Project (Cmd+Shift+S)
```

### "Build failed"
```bash
# Clean build
cd Builds/MacOSX
xcodebuild clean -project AuricHalo.xcodeproj -scheme "AuricHalo - All"

# Rebuild
xcodebuild build -project AuricHalo.xcodeproj -scheme "AuricHalo - All" -configuration Release
```

### "Plugin blocked by Gatekeeper"
```bash
# Temporary fix (untuk testing)
sudo spctl --master-disable

# Atau right-click plugin → Open

# Permanent fix: Code signing (lihat section di atas)
```

### "Plugin not showing in DAW"
1. Check plugin ada di folder yang benar
2. Rescan plugins di DAW
3. Check DAW preferences → Plugin paths
4. Restart DAW

---

## 💡 Tips

1. **Always test installer di clean Mac** sebelum distribusi
2. **Keep installer size kecil** - compress jika perlu
3. **Provide clear installation instructions** untuk user
4. **Consider creating DMG** untuk professional presentation
5. **Code signing recommended** untuk distribusi publik

---

## 📞 Resources

- [Apple Developer](https://developer.apple.com/)
- [JUCE Forum](https://forum.juce.com/)
- [Notarization Guide](https://developer.apple.com/documentation/security/notarizing_macos_software_before_distribution)

---

## ✅ Status

- ✅ Build scripts ready
- ✅ Installer script ready
- ✅ Documentation complete
- 🎯 Ready to release!

**Next**: Run `./build_and_package_mac.sh` dan test installer!
