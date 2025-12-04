#!/bin/bash

# AuricHalo macOS Installer Builder
# Version 1.1.0

set -e

echo "═══════════════════════════════════════════════════════════════"
echo "  AuricHalo v1.1.0 - macOS Installer Builder"
echo "═══════════════════════════════════════════════════════════════"
echo ""

# Configuration
PRODUCT_NAME="AuricHalo"
VERSION="1.1.0"
IDENTIFIER="com.auricaudio.aurichalo"
BUILD_DIR="$(pwd)/installer/build"
SCRIPTS_DIR="$(pwd)/installer/scripts"
RESOURCES_DIR="$(pwd)/installer/resources"
RELEASE_DIR="$(pwd)/Release/macOS-v${VERSION}"
OUTPUT_DIR="$(pwd)/Release"

# Clean previous build
echo "🧹 Cleaning previous build..."
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
mkdir -p "$BUILD_DIR/VST3"
mkdir -p "$BUILD_DIR/AU"
mkdir -p "$BUILD_DIR/Standalone"

# Copy plugins
echo "📦 Copying plugins..."
if [ -d "Builds/MacOSX/build/Release/AuricHalo.vst3" ]; then
    cp -R "Builds/MacOSX/build/Release/AuricHalo.vst3" "$BUILD_DIR/VST3/"
    echo "  ✓ VST3 copied"
else
    echo "  ⚠️  VST3 not found"
fi

if [ -d "Builds/MacOSX/build/Release/AuricHalo.component" ]; then
    cp -R "Builds/MacOSX/build/Release/AuricHalo.component" "$BUILD_DIR/AU/"
    echo "  ✓ AU copied"
else
    echo "  ⚠️  AU not found"
fi

if [ -d "Builds/MacOSX/build/Release/AuricHalo.app" ]; then
    cp -R "Builds/MacOSX/build/Release/AuricHalo.app" "$BUILD_DIR/Standalone/"
    echo "  ✓ Standalone copied"
else
    echo "  ⚠️  Standalone not found"
fi

# Create component packages
echo ""
echo "📦 Building component packages..."

# VST3 Package
pkgbuild --root "$BUILD_DIR/VST3" \
         --identifier "${IDENTIFIER}.vst3" \
         --version "$VERSION" \
         --install-location "/Library/Audio/Plug-Ins/VST3" \
         --scripts "$SCRIPTS_DIR/vst3" \
         "$BUILD_DIR/AuricHalo-VST3.pkg"
echo "  ✓ VST3 package created"

# AU Package
pkgbuild --root "$BUILD_DIR/AU" \
         --identifier "${IDENTIFIER}.component" \
         --version "$VERSION" \
         --install-location "/Library/Audio/Plug-Ins/Components" \
         --scripts "$SCRIPTS_DIR/au" \
         "$BUILD_DIR/AuricHalo-AU.pkg"
echo "  ✓ AU package created"

# Standalone Package
pkgbuild --root "$BUILD_DIR/Standalone" \
         --identifier "${IDENTIFIER}.app" \
         --version "$VERSION" \
         --install-location "/Applications" \
         "$BUILD_DIR/AuricHalo-Standalone.pkg"
echo "  ✓ Standalone package created"

# Create distribution XML
echo ""
echo "📝 Creating distribution definition..."
cat > "$BUILD_DIR/distribution.xml" << EOF
<?xml version="1.0" encoding="utf-8"?>
<installer-gui-script minSpecVersion="1">
    <title>AuricHalo v${VERSION}</title>
    <organization>${IDENTIFIER}</organization>
    <domains enable_localSystem="true"/>
    <options customize="always" require-scripts="false" hostArchitectures="arm64,x86_64"/>
    
    <!-- Welcome -->
    <welcome file="Welcome.rtf" mime-type="text/rtf"/>
    
    <!-- License -->
    <license file="License.rtf" mime-type="text/rtf"/>
    
    <!-- README -->
    <readme file="ReadMe.rtf" mime-type="text/rtf"/>
    
    <!-- Background -->
    <background file="background.png" alignment="bottomleft" scaling="none"/>
    
    <!-- Choices -->
    <choices-outline>
        <line choice="vst3"/>
        <line choice="au"/>
        <line choice="standalone"/>
    </choices-outline>
    
    <choice id="vst3" title="VST3 Plugin" description="Install VST3 plugin for Ableton Live, Reaper, FL Studio, etc." start_selected="true">
        <pkg-ref id="${IDENTIFIER}.vst3"/>
    </choice>
    
    <choice id="au" title="Audio Unit (AU)" description="Install Audio Unit for Logic Pro, GarageBand, etc." start_selected="true">
        <pkg-ref id="${IDENTIFIER}.component"/>
    </choice>
    
    <choice id="standalone" title="Standalone Application" description="Install standalone application." start_selected="true">
        <pkg-ref id="${IDENTIFIER}.app"/>
    </choice>
    
    <!-- Package References -->
    <pkg-ref id="${IDENTIFIER}.vst3" version="${VERSION}" onConclusion="none">AuricHalo-VST3.pkg</pkg-ref>
    <pkg-ref id="${IDENTIFIER}.component" version="${VERSION}" onConclusion="none">AuricHalo-AU.pkg</pkg-ref>
    <pkg-ref id="${IDENTIFIER}.app" version="${VERSION}" onConclusion="none">AuricHalo-Standalone.pkg</pkg-ref>
    
</installer-gui-script>
EOF

# Build final product
echo ""
echo "🔨 Building final installer..."
productbuild --distribution "$BUILD_DIR/distribution.xml" \
             --resources "$RESOURCES_DIR" \
             --package-path "$BUILD_DIR" \
             --version "$VERSION" \
             "$OUTPUT_DIR/AuricHalo-${VERSION}-macOS.pkg"

echo ""
echo "═══════════════════════════════════════════════════════════════"
echo "  ✅ Installer created successfully!"
echo "═══════════════════════════════════════════════════════════════"
echo ""
echo "📦 Output: $OUTPUT_DIR/AuricHalo-${VERSION}-macOS.pkg"
echo ""
echo "📊 Package size:"
du -h "$OUTPUT_DIR/AuricHalo-${VERSION}-macOS.pkg"
echo ""
echo "🧪 To test installer:"
echo "   open $OUTPUT_DIR/AuricHalo-${VERSION}-macOS.pkg"
echo ""
