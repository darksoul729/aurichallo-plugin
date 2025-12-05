#!/bin/bash

echo "========================================"
echo "  AURIC HALO - macOS BUILD SCRIPT"
echo "========================================"
echo ""

# Get script directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_DIR="$SCRIPT_DIR/.."

cd "$PROJECT_DIR"

# Check if Xcode project exists
if [ ! -d "Builds/MacOSX/AuricHalo.xcodeproj" ]; then
    echo "ERROR: Xcode project not found!"
    echo "Please regenerate with Projucer first."
    exit 1
fi

echo "[1/4] Building VST3..."
xcodebuild -project Builds/MacOSX/AuricHalo.xcodeproj \
           -scheme "AuricHalo - VST3" \
           -configuration Release \
           -arch x86_64 -arch arm64 \
           build \
           | grep -E "error:|warning:|Build succeeded"

if [ $? -ne 0 ]; then
    echo "ERROR: VST3 build failed!"
    exit 1
fi

echo ""
echo "[2/4] Building AU (Audio Unit)..."
xcodebuild -project Builds/MacOSX/AuricHalo.xcodeproj \
           -scheme "AuricHalo - AU" \
           -configuration Release \
           -arch x86_64 -arch arm64 \
           build \
           | grep -E "error:|warning:|Build succeeded"

if [ $? -ne 0 ]; then
    echo "ERROR: AU build failed!"
    exit 1
fi

echo ""
echo "[3/4] Building Standalone..."
xcodebuild -project Builds/MacOSX/AuricHalo.xcodeproj \
           -scheme "AuricHalo - Standalone" \
           -configuration Release \
           -arch x86_64 -arch arm64 \
           build \
           | grep -E "error:|warning:|Build succeeded"

if [ $? -ne 0 ]; then
    echo "ERROR: Standalone build failed!"
    exit 1
fi

echo ""
echo "[4/4] Verifying builds..."
echo ""

# Check VST3
if [ -d "$HOME/Library/Audio/Plug-Ins/VST3/AuricHalo.vst3" ]; then
    echo "✓ VST3: $HOME/Library/Audio/Plug-Ins/VST3/AuricHalo.vst3"
else
    echo "✗ VST3 not found!"
fi

# Check AU
if [ -d "$HOME/Library/Audio/Plug-Ins/Components/AuricHalo.component" ]; then
    echo "✓ AU: $HOME/Library/Audio/Plug-Ins/Components/AuricHalo.component"
else
    echo "✗ AU not found!"
fi

# Check Standalone
if [ -d "Builds/MacOSX/build/Release/AuricHalo.app" ]; then
    echo "✓ Standalone: Builds/MacOSX/build/Release/AuricHalo.app"
else
    echo "✗ Standalone not found!"
fi

echo ""
echo "========================================"
echo "  BUILD COMPLETE!"
echo "========================================"
echo ""
echo "Next steps:"
echo "1. Test plugins in your DAW"
echo "2. Sign with Developer ID (for distribution)"
echo "3. Notarize with Apple (for distribution)"
echo "4. Create installer package"
echo ""
