========================================
  AURIC HALO - INSTALLER 64-BIT
========================================

UNTUK CLIENT:
-------------

File installer: AuricHalo-1.0.0-Win64-Setup.exe

CARA INSTALL:
1. Double-click file installer
2. Klik "Next" beberapa kali
3. Pilih plugin yang mau diinstall:
   ✓ VST3 (untuk semua DAW: FL Studio, Ableton, Cubase, dll)
   ✓ AAX (untuk Pro Tools 2019+)
   ✓ Standalone (aplikasi sendiri)
4. Klik "Install"
5. Selesai!

SETELAH INSTALL:
- VST3 otomatis terinstall ke: C:\Program Files\Common Files\VST3\
- AAX otomatis terinstall ke: C:\Program Files\Common Files\Avid\Audio\Plug-Ins\
- Standalone terinstall ke: C:\Program Files\Auric Audio\Auric Halo\

CARA PAKAI DI DAW:
- FL Studio: Rescan plugin di Plugin Manager
- Ableton: Rescan di Preferences → Plug-ins
- Pro Tools: Setup → Plug-ins → Rescan
- Cubase/Nuendo: Plugin Manager → Rescan

SYSTEM REQUIREMENTS:
- Windows 10/11 (64-bit) ← WAJIB 64-BIT!
- DAW 64-bit (FL Studio, Ableton, Pro Tools, dll)
- VST3 atau AAX support

TIDAK RIBET:
✓ Installer otomatis install ke folder yang benar
✓ Tidak perlu copy-paste manual
✓ Tidak perlu setting path
✓ Tinggal rescan di DAW
✓ Langsung bisa dipakai!


========================================

UNTUK DEVELOPER:
----------------

CARA BUILD INSTALLER:

1. PASTIKAN PROJUCER SUDAH SETUP 64-BIT:
   - Buka AuricHalo.jucer
   - Visual Studio 2022 → Configurations
   - Debug: Platform = x64
   - Release: Platform = x64
   - Plugin Formats: centang AAX
   - Save (Ctrl+Shift+S)

2. BUILD PLUGIN:
   Cara 1 (Otomatis):
   - Jalankan: scripts\build_all_and_installer.bat
   - Tunggu selesai
   - Installer otomatis dibuat!

   Cara 2 (Manual):
   - Buka Builds\VisualStudio2022\AuricHalo.sln
   - Configuration: Release
   - Platform: x64
   - Build → Build Solution
   - Jalankan: scripts\build_installer_windows.bat

3. TEST INSTALLER:
   - Install di komputer kamu
   - Test di DAW (FL Studio, Pro Tools, dll)
   - Test di komputer client

4. KIRIM KE CLIENT:
   - File: Release\AuricHalo-1.0.0-Win64-Setup.exe
   - Kirim via email, Google Drive, WeTransfer, dll
   - Client tinggal double-click dan install!

VERIFIKASI 64-BIT:
- File installer: AuricHalo-1.0.0-Win64-Setup.exe (ada "Win64")
- Build path: x64\Release\VST3 (ada "x64")
- AAX path: x64\Release\AAX (ada "x64")
- Installer cek Windows 64-bit otomatis

TROUBLESHOOTING:
- Jika AAX tidak ke-build: Pastikan AAX SDK sudah terinstall
- Jika build error: Pastikan platform x64, bukan Win32
- Jika installer error: Pastikan Inno Setup sudah terinstall

========================================

CHANGELOG:
----------

v1.0.0 (2025-12-03)
- ✓ Installer 64-bit
- ✓ VST3 64-bit support
- ✓ AAX 64-bit support (Pro Tools 2019+)
- ✓ Standalone 64-bit
- ✓ Otomatis install ke folder standard
- ✓ UI modern 3D dengan glow effect
- ✓ Pesan installer dalam Bahasa Indonesia
- ✓ Cek Windows 64-bit otomatis

========================================

SUPPORT:
--------
Email: support@auricaudio.com
Website: https://www.auricaudio.com

========================================
