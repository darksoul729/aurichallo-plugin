#!/bin/bash
set -e

echo "📦 Creating macOS Installer for Auric Halo..."
echo "============================================="

# Check if we're on macOS
if [[ "$OSTYPE" != "darwin"* ]]; then
    echo "❌ This script must be run on macOS"
    exit 1
fi

# Navigate to project root
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT"

# Variables
PRODUCT_NAME="Auric Halo"
VERSION="1.0.0"
IDENTIFIER="com.auricaudio.aurichalo"
INSTALLER_DIR="installer_temp"
PKG_NAME="AuricHalo-${VERSION}-macOS.pkg"

echo "🗂️  Preparing installer files..."

# Create temporary installer directory
rm -rf "$INSTALLER_DIR"
mkdir -p "$INSTALLER_DIR"

# Create directory structure
mkdir -p "$INSTALLER_DIR/VST3"
mkdir -p "$INSTALLER_DIR/Applications"

# Copy built plugins (if they exist)
BUILD_DIR="Builds/MacOSX/build/Release"

if [ -d "$BUILD_DIR/AuricHalo.vst3" ]; then
    echo "📋 Copying VST3..."
    cp -R "$BUILD_DIR/AuricHalo.vst3" "$INSTALLER_DIR/VST3/"
fi

if [ -d "$BUILD_DIR/AuricHalo.app" ]; then
    echo "📋 Copying Standalone App..."
    cp -R "$BUILD_DIR/AuricHalo.app" "$INSTALLER_DIR/Applications/"
fi

# Create postinstall script
cat > "$INSTALLER_DIR/postinstall" << 'EOF'
#!/bin/bash

echo "Installing Auric Halo VST3 plugin..."

# Install VST3
if [ -d "$1/VST3/AuricHalo.vst3" ]; then
    mkdir -p "$HOME/Library/Audio/Plug-Ins/VST3"
    cp -R "$1/VST3/AuricHalo.vst3" "$HOME/Library/Audio/Plug-Ins/VST3/"
    echo "✅ VST3 installed"
fi

# Install Standalone App
if [ -d "$1/Applications/AuricHalo.app" ]; then
    cp -R "$1/Applications/AuricHalo.app" "/Applications/"
    echo "✅ Standalone App installed"
fi

echo "🎉 Auric Halo installation complete!"
echo ""
echo "Plugin location:"
echo "VST3: ~/Library/Audio/Plug-Ins/VST3/AuricHalo.vst3"
echo "App:  /Applications/AuricHalo.app"
echo ""
echo "Please rescan plugins in your DAW."
echo ""
echo "Compatible DAWs:"
echo "- Ableton Live"
echo "- FL Studio"
echo "- Cubase"
echo "- Studio One"
echo "- Reaper"
echo "- Bitwig"
echo "- And many more VST3-compatible DAWs"

exit 0
EOF

chmod +x "$INSTALLER_DIR/postinstall"

# Create package
echo "🔨 Creating PKG installer..."

pkgbuild \
    --root "$INSTALLER_DIR" \
    --identifier "$IDENTIFIER" \
    --version "$VERSION" \
    --install-location "/tmp/AuricHalo" \
    --scripts "$INSTALLER_DIR" \
    "$PKG_NAME"

# Clean up
rm -rf "$INSTALLER_DIR"

if [ -f "$PKG_NAME" ]; then
    echo ""
    echo "✅ Installer created successfully!"
    echo "📦 File: $PKG_NAME"
    echo "📏 Size: $(du -h "$PKG_NAME" | cut -f1)"
    echo ""
    echo "🚀 Ready for distribution!"
    echo ""
    echo "To test:"
    echo "1. Double-click $PKG_NAME"
    echo "2. Follow installation wizard"
    echo "3. Launch your DAW and rescan plugins"
else
    echo "❌ Failed to create installer"
    exit 1
fi