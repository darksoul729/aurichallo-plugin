#!/bin/bash

echo "🧪 Testing Auric Halo Standalone App"
echo "===================================="
echo ""

# Check if app exists
if [ ! -d "Builds/MacOSX/build/Release/AuricHalo.app" ]; then
    echo "❌ Standalone app not found!"
    echo "💡 Run: ./scripts/build_and_package_mac.sh"
    exit 1
fi

echo "✅ Standalone app found"
echo ""

# Show license file location
echo "📁 License file location:"
echo "   ~/Library/Application Support/AuricAudio/.aurichalo_lic"
echo ""

# Check if license exists
if [ -f ~/Library/Application\ Support/AuricAudio/.aurichalo_lic ]; then
    echo "✅ License file exists (plugin already activated)"
    echo ""
    echo "To test fresh install:"
    echo "   rm ~/Library/Application\\ Support/AuricAudio/.aurichalo_lic"
    echo ""
else
    echo "⚠️  No license file (plugin not activated)"
    echo ""
fi

# Launch app
echo "🚀 Launching Standalone App..."
echo ""
open Builds/MacOSX/build/Release/AuricHalo.app

echo "📋 Next steps:"
echo "1. License dialog should appear"
echo "2. Copy Hardware ID from dialog"
echo "3. Generate license key:"
echo "   python3 scripts/generate_license_key.py YOUR_HWID"
echo "4. Paste key in dialog and activate"
echo ""
echo "📖 Full guide: TEST_LICENSE.md"
