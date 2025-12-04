# 🧪 Test License System - Standalone App

## 1. Jalankan Standalone App

```bash
# Dari project folder
open Builds/MacOSX/build/Release/AuricHalo.app

# Atau dari installed location
open /Applications/AuricHalo.app
```

## 2. Yang Akan Terjadi

Saat app terbuka:
- ⚠️ **Dialog license muncul** otomatis
- 📋 **Hardware ID ditampilkan** di dialog
- 🔒 Plugin **tidak bisa dipakai** sampai activate

## 3. Get Hardware ID

Di License Dialog, Anda akan lihat:
```
Hardware ID: XXXXXXXXXXXXXXXX
```

**Copy Hardware ID** ini untuk generate license key.

## 4. Generate Test License Key

Di Terminal:

```bash
# Get your Hardware ID dari plugin
# Contoh: A1B2C3D4E5F6G7H8

python3 scripts/generate_license_key.py A1B2C3D4E5F6G7H8
```

Output:
```
License Key: AH01-XXXX-XXXX-XXXX-YYYY
```

## 5. Activate License

1. **Copy license key** dari terminal
2. **Paste** di License Dialog
3. **Klik "Activate"**
4. ✅ **Success!** Plugin sekarang licensed

## 6. Verify Activation

Setelah activate:
- ✅ Dialog license tertutup
- ✅ Plugin UI muncul
- ✅ Button "License" di pojok kanan atas jadi **hijau** dan text "Licensed"
- ✅ Plugin bisa dipakai

## 7. Test Audio Processing

Di Standalone App:
1. **Settings** → Audio Settings
2. Pilih **Audio Device** (Built-in Output)
3. Test dengan **microphone** atau **audio file**
4. Adjust **knobs** (Tone, Drive, Mix, Output)
5. Check apakah **audio processing** bekerja

## 8. Test License Persistence

1. **Close** standalone app
2. **Open** lagi
3. ✅ Plugin harus langsung buka (tidak minta license lagi)
4. ✅ Button "License" harus tetap hijau "Licensed"

## 9. Test Invalid License Key

1. **Deactivate** license (hapus file):
   ```bash
   rm ~/Library/Application\ Support/AuricAudio/.aurichalo_lic
   ```

2. **Open** plugin lagi
3. **Masukkan invalid key**: `XXXX-XXXX-XXXX-XXXX`
4. ❌ Harus dapat error: "Invalid license key"

## 10. Test License for Different Computer

License key **hardware locked**, jadi:
- ✅ Key hanya work di 1 computer
- ❌ Key tidak work di computer lain
- 💡 Setiap computer butuh key sendiri

---

## 🔍 Troubleshooting

### Plugin Tidak Muncul

```bash
# Check if app exists
ls -la Builds/MacOSX/build/Release/AuricHalo.app

# Check installed location
ls -la /Applications/AuricHalo.app
```

### Dialog License Tidak Muncul

```bash
# Check license file
cat ~/Library/Application\ Support/AuricAudio/.aurichalo_lic

# Delete to reset
rm ~/Library/Application\ Support/AuricAudio/.aurichalo_lic
```

### License Key Tidak Valid

- Check typo (copy-paste recommended)
- Verify Hardware ID match
- Regenerate key dengan Hardware ID yang benar

### Audio Tidak Keluar

- Check Audio Settings di app
- Pilih correct audio device
- Check volume levels

---

## 📝 Quick Test Commands

```bash
# 1. Open standalone
open Builds/MacOSX/build/Release/AuricHalo.app

# 2. Get Hardware ID dari plugin UI

# 3. Generate key (ganti HWID dengan Hardware ID Anda)
python3 scripts/generate_license_key.py YOUR_HWID

# 4. Activate di plugin

# 5. Test persistence
rm ~/Library/Application\ Support/AuricAudio/.aurichalo_lic
open Builds/MacOSX/build/Release/AuricHalo.app
```

---

## ✅ Test Checklist

- [ ] Standalone app opens
- [ ] License dialog appears automatically
- [ ] Hardware ID displayed
- [ ] Can copy Hardware ID
- [ ] Generate license key works
- [ ] License activation works
- [ ] Plugin unlocks after activation
- [ ] Button shows "Licensed" (green)
- [ ] Audio processing works
- [ ] License persists after restart
- [ ] Invalid key rejected
- [ ] Plugin locked without license

---

## 🎯 Expected Behavior

### Without License:
- ⚠️ Dialog muncul otomatis
- 🔒 Plugin locked
- 🔇 Audio muted
- ❌ Cannot use plugin

### With Valid License:
- ✅ Dialog tertutup
- ✅ Plugin unlocked
- 🎵 Audio processing works
- ✅ Can use all features

---

Good luck testing! 🚀
