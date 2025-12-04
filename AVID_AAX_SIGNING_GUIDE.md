# 🔐 Avid AAX Signing - Generate Keys

## Step 1: Generate Private Key

Buka Terminal dan jalankan:

```bash
# Generate private key (2048-bit RSA)
openssl genrsa -out private_key.pem 2048
```

## Step 2: Generate Public Key dari Private Key

```bash
# Extract public key dari private key
openssl rsa -in private_key.pem -pubout -out public_key.pem
```

## Step 3: Verify Keys

```bash
# Check private key
openssl rsa -in private_key.pem -check

# View public key
cat public_key.pem
```

## Step 4: Upload Public Key ke Avid

1. Login ke **Avid Developer Portal**
2. Go to **AAX Developer** section
3. Upload `public_key.pem`
4. Submit for approval

## ⚠️ PENTING - Backup Keys!

```bash
# Backup ke folder aman
mkdir -p ~/Documents/AuricHalo_Keys
cp private_key.pem ~/Documents/AuricHalo_Keys/
cp public_key.pem ~/Documents/AuricHalo_Keys/

# JANGAN commit ke git!
# JANGAN share private key!
```

## File yang Dihasilkan:

- `private_key.pem` - **RAHASIA!** Simpan aman, jangan share
- `public_key.pem` - Upload ke Avid

## Format Public Key:

Public key akan terlihat seperti ini:

```
-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA...
...
-----END PUBLIC KEY-----
```

## Next Steps Setelah Upload:

1. **Avid akan review** (1-2 minggu)
2. **Setelah approved**, download PACE tools
3. **Sign AAX plugin** dengan private key
4. **Wrap dengan iLok** protection
5. **Test di Pro Tools**

## Sign AAX Plugin (Setelah Approved):

```bash
# Install PACE tools (dari Avid)
# Sign plugin
wraptool sign \
  --verbose \
  --account YOUR_ACCOUNT \
  --wcguid YOUR_GUID \
  --signid YOUR_SIGNID \
  --in AuricHalo.aaxplugin \
  --out AuricHalo_signed.aaxplugin
```

## Troubleshooting:

### "openssl command not found"
```bash
# Install openssl via Homebrew
brew install openssl
```

### "Permission denied"
```bash
# Fix permissions
chmod 600 private_key.pem
chmod 644 public_key.pem
```

## 🔒 Security Best Practices:

1. ✅ **Backup private key** ke multiple locations
2. ✅ **Encrypt backup** jika di cloud
3. ✅ **Never commit** ke git
4. ✅ **Never share** private key
5. ✅ **Use password** untuk private key (optional tapi recommended)

## Generate dengan Password (Recommended):

```bash
# Generate private key dengan password
openssl genrsa -aes256 -out private_key_encrypted.pem 2048

# Extract public key
openssl rsa -in private_key_encrypted.pem -pubout -out public_key.pem
```

Setiap kali sign plugin, akan diminta password.

---

## Quick Commands:

```bash
# Generate keys (simple)
openssl genrsa -out private_key.pem 2048
openssl rsa -in private_key.pem -pubout -out public_key.pem

# View public key
cat public_key.pem

# Backup
cp *.pem ~/Documents/AuricHalo_Keys/
```

Upload `public_key.pem` ke Avid Developer Portal!
