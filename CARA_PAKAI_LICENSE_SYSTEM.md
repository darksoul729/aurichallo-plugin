# 🔐 Cara Pakai License System - Auric Halo

## ✅ Build Selesai!

Installer: `AuricHalo-1.0.0-macOS.pkg` (7.1MB)

---

## 🎯 Cara Kerja License System

### 1. First Time Use (Trial Mode)

Saat user pertama kali install dan buka plugin:
- ✅ Plugin otomatis masuk **Trial Mode**
- ✅ **14 hari** trial gratis
- ✅ Semua fitur unlocked
- ✅ Button "License" di pojok kanan atas menunjukkan: **"Trial: 14d"**

### 2. Setelah Trial Expired

Setelah 14 hari:
- ❌ Plugin akan **mute audio** (tidak process)
- ⚠️ Dialog license muncul otomatis
- 💡 User diminta masukkan license key

---

## 💰 Workflow Penjualan

### Step 1: Customer Beli Plugin

Customer beli dari website/Gumroad Anda.

### Step 2: Customer Install Plugin

```bash
# Customer download dan install
AuricHalo-1.0.0-macOS.pkg
```

### Step 3: Customer Buka Plugin

- Plugin masuk trial mode
- Customer test selama 14 hari

### Step 4: Customer Mau Beli License

Customer klik button **"License"** atau **"Activate"** di plugin.

### Step 5: Customer Dapat Hardware ID

Di License Dialog, customer akan lihat:
```
Hardware ID: A1B2C3D4E5F6G7H8
```

Customer **copy Hardware ID** ini.

### Step 6: Customer Kirim Hardware ID ke Anda

Via email:
```
To: support@auricaudio.com
Subject: License Request

Hi, saya sudah beli Auric Halo.
Hardware ID saya: A1B2C3D4E5F6G7H8

Terima kasih!
```

### Step 7: Anda Generate License Key

Di Terminal:

```bash
cd /path/to/aurichallo-plugin
python3 scripts/generate_license_key.py A1B2C3D4E5F6G7H8
```

Output:
```
License Key: AH01-A1B2-C3D4-0001-XXXX
```

### Step 8: Kirim License Key ke Customer

Email reply:
```
Hi,

Terima kasih sudah membeli Auric Halo!

License Key Anda:
AH01-A1B2-C3D4-0001-XXXX

Cara Aktivasi:
1. Buka plugin Auric Halo
2. Klik button "License"
3. Paste license key di atas
4. Klik "Activate"

Selamat menikmati!

Best regards,
Auric Audio Team
```

### Step 9: Customer Activate

Customer:
1. Buka plugin
2. Klik "License"
3. Paste key: `AH01-A1B2-C3D4-0001-XXXX`
4. Klik "Activate"
5. ✅ **Success!** Plugin sekarang licensed selamanya

---

## 🔑 Generate License Keys

### Manual (Per Customer)

```bash
python3 scripts/generate_license_key.py HARDWARE_ID
```

### Batch (Multiple Customers)

Buat file `customers.txt`:
```
A1B2C3D4E5F6G7H8
B2C3D4E5F6G7H8I9
C3D4E5F6G7H8I9J0
```

Generate semua:
```bash
while read hwid; do
    python3 scripts/generate_license_key.py "$hwid"
done < customers.txt
```

Semua keys tersimpan di `license_keys.txt`.

---

## 📊 Track Licenses

File `license_keys.txt` berisi record semua keys:

```
A1B2C3D4,AH01-A1B2-C3D4-0001-XXXX
B2C3D4E5,AH01-B2C3-D4E5-0002-YYYY
C3D4E5F6,AH01-C3D4-E5F6-0003-ZZZZ
```

Format: `HardwareID,LicenseKey`

Gunakan spreadsheet untuk track:
- Hardware ID
- License Key
- Customer Name
- Email
- Purchase Date
- Order ID

---

## 🧪 Testing License System

### Test Trial Mode

1. Install plugin
2. Buka plugin
3. Check button "License" → harus show "Trial: 14d"
4. Test semua fitur → harus work

### Test License Activation

1. Get your Hardware ID dari plugin
2. Generate test key:
   ```bash
   python3 scripts/generate_license_key.py YOUR_HWID
   ```
3. Masukkan key di plugin
4. Check button "License" → harus show "Licensed" (hijau)

### Test Expired Trial

Edit file license:
```bash
# Location
~/Library/Application Support/AuricAudio/.aurichalo_lic

# Atau hapus untuk reset trial
rm ~/Library/Application\ Support/AuricAudio/.aurichalo_lic
```

---

## 💡 Tips Penjualan

### Pricing Strategy

- **Trial**: 14 hari gratis (cukup untuk evaluate)
- **Price**: $29-$99 (typical untuk audio plugin)
- **Launch Discount**: 20-30% off untuk early adopters

### Payment Platforms

**Gumroad** (Recommended):
- Easy setup
- Handle payments
- Email delivery
- License key field built-in
- 10% fee

**Alternatives**:
- PayPal + Manual email
- Stripe + Custom website
- Paddle (payment processor)

### Marketing

- Emphasize **14-day FREE trial**
- "Try before you buy"
- Demo video di YouTube
- Before/After audio examples
- User testimonials

---

## 🔒 Security

### License Key Protection

- ✅ Hardware locked (1 key = 1 computer)
- ✅ Checksum validation
- ✅ Encrypted storage
- ✅ Obfuscated code

### Limitations

- ❌ Tidak 100% anti-piracy
- ❌ User bisa share Hardware ID
- ✅ Tapi cukup untuk prevent casual piracy

### Best Practices

1. **Track licenses** - Know which keys sent to which customers
2. **Limit activations** - 1-2 computer changes allowed
3. **Provide support** - Help legitimate customers
4. **Don't be too strict** - Balance security vs user experience

---

## 📞 Customer Support

### Common Issues

**"Invalid License Key"**
- Check typo (copy-paste recommended)
- Verify Hardware ID match
- Regenerate if needed

**"License Not Working After Computer Change"**
- Generate new key for new Hardware ID
- Consider allowing 1-2 computer changes per customer

**"Trial Expired"**
- Encourage purchase
- Provide purchase link
- Offer discount if appropriate

---

## 🚀 Automation (Advanced)

### Auto-Generate Keys

Setup web server dengan API:

```python
# Flask example
@app.route('/generate_license', methods=['POST'])
def generate_license():
    hwid = request.json['hardware_id']
    email = request.json['email']
    
    # Generate key
    key = generate_license_key(hwid)
    
    # Save to database
    save_license(hwid, key, email)
    
    # Send email
    send_license_email(email, key)
    
    return {'license_key': key}
```

### Integration dengan Gumroad

Gumroad webhook → Your server → Auto-generate & email key

---

## 📈 Analytics

Track:
- Total licenses sold
- Trial conversion rate
- Support requests
- Refund rate
- Popular DAWs used

---

## ✅ Checklist Launch

Before releasing:

- [ ] Test trial mode
- [ ] Test license activation
- [ ] Test expired trial
- [ ] Generate test keys
- [ ] Setup payment platform
- [ ] Create customer email templates
- [ ] Write user manual
- [ ] Create demo video
- [ ] Setup support email
- [ ] Prepare marketing materials

---

## 🎉 Ready to Sell!

Installer: `AuricHalo-1.0.0-macOS.pkg`

Features:
- ✅ 14-day FREE trial
- ✅ License activation system
- ✅ Hardware lock protection
- ✅ VST3 format (universal)
- ✅ Professional installer

**Good luck dengan penjualan plugin Anda!** 💰🎊
