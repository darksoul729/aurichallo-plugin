# 🔧 Build Instructions - With License System

## ⚠️ Important: Projucer Required

Untuk build plugin dengan license system, Anda perlu:

### 1. Buka Projucer
```bash
# Buka Projucer application
# Biasanya di: /Applications/JUCE/Projucer.app
```

### 2. Open Project
- File → Open
- Pilih `AuricHalo.jucer`

### 3. Verify Files Added
Check di sidebar kiri, pastikan file ini ada:
- ✅ `source/LicenseManager.cpp`
- ✅ `source/LicenseManager.h`
- ✅ `source/UI/LicenseDialog.cpp`
- ✅ `source/UI/LicenseDialog.h`

### 4. Save & Generate Xcode Project
- Klik tombol "Save Project and Open in IDE"
- Atau: Cmd+Shift+S

### 5. Build di Xcode
- Xcode akan terbuka otomatis
- Product → Build (Cmd+B)

### 6. Create Installer
```bash
cd scripts
./create_mac_installer.sh
```

---

## 🚀 Alternative: Build Without License (Quick Test)

Jika mau test build cepat tanpa license system:

### Revert Changes Temporarily:

1. **Comment out license code di AuricHaloEditor.h:**
```cpp
// #include "UI/LicenseDialog.h"

// Comment these methods:
// void checkLicenseStatus();
// void showLicenseDialog();

// Comment these members:
// juce::TextButton licenseButton;
// std::unique_ptr<LicenseDialog> licenseDialog;
```

2. **Comment out license code di AuricHaloEditor.cpp:**
```cpp
// Comment semua yang berhubungan dengan licenseButton
// Comment checkLicenseStatus() dan showLicenseDialog()
```

3. **Comment out di AuricHaloProcessor.h:**
```cpp
// #include "LicenseManager.h"
// LicenseManager licenseManager;
// LicenseManager& getLicenseManager() { return licenseManager; }
```

4. **Build:**
```bash
./scripts/build_and_package_mac.sh
```

---

## ✅ Recommended Workflow

### For Development:
1. Build tanpa license dulu untuk test functionality
2. Setelah semua OK, add license system
3. Rebuild dengan Projucer

### For Release:
1. Pastikan license system aktif
2. Build via Projucer → Xcode
3. Test trial mode
4. Test license activation
5. Create installer
6. Distribute!

---

## 📝 Files Created for License System

```
source/
├── LicenseManager.h          ← Core license logic
├── LicenseManager.cpp
└── UI/
    ├── LicenseDialog.h       ← UI for activation
    └── LicenseDialog.cpp

scripts/
└── generate_license_key.py   ← Key generator

Documentation:
├── LICENSE_SYSTEM_GUIDE.md
├── QUICK_LICENSE_GUIDE.md
└── LICENSE_SYSTEM_SUMMARY.md
```

---

## 🎯 Next Steps

**Option A: Build with License (Recommended)**
1. Open Projucer
2. Verify files
3. Save & generate
4. Build in Xcode

**Option B: Build without License (Quick)**
1. Comment out license code
2. Build with script
3. Test plugin
4. Add license later

---

Choose your path and proceed! 🚀
