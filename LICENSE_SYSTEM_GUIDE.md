# 🔐 Auric Halo - License System Guide

## Overview

Plugin Auric Halo sekarang dilengkapi dengan sistem lisensi untuk melindungi produk Anda saat dijual.

### Features:
- ✅ **Trial Mode**: 14 hari trial gratis
- ✅ **License Key Activation**: Customer masukkan license key untuk aktivasi
- ✅ **Hardware Lock**: License tied to customer's computer
- ✅ **Encrypted Storage**: License data disimpan dengan encryption
- ✅ **Easy Management**: Tool untuk generate license keys

---

## 📋 Cara Kerja

### 1. First Time Use (Trial)
- User install plugin
- Plugin otomatis masuk **Trial Mode** (14 hari)
- User bisa pakai semua fitur selama trial
- Setelah 14 hari, plugin akan minta license key

### 2. Purchase & Activation
- User beli license dari website Anda
- User dapat **License Key** via email
- User masukkan license key di plugin
- Plugin teractivate dan bisa dipakai selamanya

### 3. License Key Format
```
XXXX-XXXX-XXXX-XXXX
```
Contoh: `AH01-A1B2C3D4-0001-F5E6`

- **AH01**: Product ID (Auric Halo v1)
- **A1B2C3D4**: Hardware ID (unique per computer)
- **0001**: Sequential number
- **F5E6**: Checksum (validation)

---

## 🔑 Generate License Keys

### Untuk Setiap Customer:

1. **Customer memberikan Hardware ID**
   - Customer buka plugin
   - Klik "Purchase License" atau "About"
   - Copy Hardware ID (contoh: `A1B2C3D4E5F6G7H8`)

2. **Anda generate license key**
   ```bash
   cd scripts
   python3 generate_license_key.py A1B2C3D4E5F6G7H8
   ```

3. **Kirim license key ke customer**
   - Via email
   - Via download page
   - Via payment platform (Gumroad, etc.)

### Batch Generation

Untuk generate banyak keys sekaligus:

```bash
# Generate untuk multiple hardware IDs
python3 generate_license_key.py HWID1
python3 generate_license_key.py HWID2
python3 generate_license_key.py HWID3
```

Semua keys akan tersimpan di `license_keys.txt`

---

## 💰 Integration dengan Payment Platform

### Opsi 1: Manual (Simple)

1. Customer beli via Gumroad/PayPal
2. Customer email Anda dengan Hardware ID
3. Anda generate & kirim license key
4. Customer activate

### Opsi 2: Semi-Automated

1. Setup Gumroad dengan "License Key" field
2. Customer beli → dapat email otomatis
3. Email berisi instruksi untuk dapat Hardware ID
4. Customer reply dengan Hardware ID
5. Anda generate & kirim key

### Opsi 3: Fully Automated (Advanced)

Butuh web server dengan API:
1. Customer beli via website Anda
2. Customer masukkan Hardware ID saat checkout
3. Server auto-generate license key
4. Customer langsung dapat key

---

## 🛠️ Customization

### Ubah Trial Period

Edit `source/LicenseManager.h`:
```cpp
static constexpr int TRIAL_DAYS = 14;  // Ubah jadi 7, 30, dll
```

### Ubah Product ID

Edit `source/LicenseManager.h`:
```cpp
static constexpr const char* PRODUCT_ID = "AURICHALO";
```

Dan `scripts/generate_license_key.py`:
```python
PRODUCT_ID = "AURICHALO"
```

### Ubah Purchase URL

Edit `source/UI/LicenseDialog.cpp`:
```cpp
juce::URL purchaseURL("https://www.auricaudio.com/purchase");
```

---

## 📊 License Management

### Track Licenses

File `license_keys.txt` berisi semua generated keys:
```
A1B2C3D4,AH01-A1B2-C3D4-0001-F5E6
E5F6G7H8,AH01-E5F6-G7H8-0002-A1B2
```

Format: `HardwareID,LicenseKey`

### Prevent Piracy

License system ini menggunakan:
- ✅ Hardware locking (key hanya work di 1 computer)
- ✅ Checksum validation
- ✅ Encrypted storage
- ✅ Obfuscated code

**Note**: Tidak ada sistem yang 100% anti-piracy, tapi ini cukup untuk mencegah casual piracy.

---

## 🧪 Testing

### Test Trial Mode

1. Build plugin
2. Install plugin
3. Buka plugin → akan show "Trial: 14 days remaining"
4. Test semua fitur

### Test License Activation

1. Generate test license key:
   ```bash
   # Get your Hardware ID dari plugin
   python3 scripts/generate_license_key.py YOUR_HWID
   ```

2. Masukkan key di plugin
3. Plugin akan show "Licensed"

### Test Expired Trial

Edit trial date di file:
```
~/Library/Application Support/AuricAudio/.aurichalo_lic
```

Atau hapus file untuk reset trial.

---

## 📝 Customer Instructions

Buat file `LICENSE_INSTRUCTIONS.txt` untuk customer:

```
AURIC HALO - LICENSE ACTIVATION

Thank you for purchasing Auric Halo!

ACTIVATION STEPS:
1. Open Auric Halo plugin in your DAW
2. Click "Activate License" button
3. Enter your license key (sent via email)
4. Click "Activate"

Your license key format: XXXX-XXXX-XXXX-XXXX

TROUBLESHOOTING:
- Make sure you copied the entire key
- Keys are case-insensitive
- Each key works on ONE computer only
- Contact support@auricaudio.com for help

TRIAL VERSION:
- 14 days free trial
- All features unlocked
- No credit card required
```

---

## 🚀 Deployment Checklist

Before releasing:

- [ ] Test trial mode
- [ ] Test license activation
- [ ] Test expired trial
- [ ] Update purchase URL in code
- [ ] Create customer instructions
- [ ] Setup payment platform (Gumroad/etc)
- [ ] Test on clean Mac (no previous install)
- [ ] Prepare email template for license keys

---

## 💡 Tips

### Pricing Strategy
- Trial: 14 hari gratis (cukup untuk evaluate)
- Price: $29-$99 (typical untuk audio plugin)
- Discount: 20-30% untuk launch

### Support
- Respond cepat ke license issues
- Provide clear instructions
- Consider "reset license" untuk customer yang ganti computer

### Marketing
- Emphasize trial period di website
- "Try before you buy"
- Money-back guarantee (optional)

---

## 🔒 Security Notes

**DO NOT**:
- Share `generate_license_key.py` publicly
- Commit `license_keys.txt` to git
- Share encryption keys

**DO**:
- Keep license generator private
- Backup `license_keys.txt`
- Track which keys sent to which customers

---

## 📞 Support

Jika customer punya masalah:

1. **Invalid License Key**
   - Check typo
   - Verify Hardware ID match
   - Regenerate if needed

2. **License Not Working After Computer Change**
   - Generate new key for new Hardware ID
   - Consider allowing 1-2 computer changes

3. **Trial Expired**
   - Encourage purchase
   - Provide purchase link

---

## ✅ Next Steps

1. **Build plugin** dengan license system
2. **Test thoroughly**
3. **Setup payment platform**
4. **Create marketing materials**
5. **Launch!**

Good luck with your plugin sales! 🎉
