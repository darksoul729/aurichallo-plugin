#!/bin/bash
set -e

echo "🍎 Auric Halo - Complete macOS Build & Package"
echo "=============================================="

# Navigate to script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Step 1: Build the project
echo "Step 1: Building project..."
./one_click_build_mac.sh

if [ $? -ne 0 ]; then
    echo "❌ Build failed, stopping..."
    exit 1
fi

echo ""
echo "Step 2: Creating installer..."
./create_mac_installer.sh

if [ $? -ne 0 ]; then
    echo "❌ Installer creation failed"
    exit 1
fi

echo ""
echo "🎉 Complete! Build and packaging finished successfully!"
echo ""
echo "Next steps:"
echo "1. Test the .pkg installer"
echo "2. Test plugins in various DAWs"
echo "3. Consider code signing for distribution"