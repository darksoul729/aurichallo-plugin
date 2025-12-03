#!/bin/bash
set -e

echo "🍎 Building Auric Halo for macOS..."
echo "=================================="

# Check if we're on macOS
if [[ "$OSTYPE" != "darwin"* ]]; then
    echo "❌ This script must be run on macOS"
    exit 1
fi

# Check if Xcode is installed
if ! command -v xcodebuild &> /dev/null; then
    echo "❌ Xcode not found. Please install Xcode from App Store"
    exit 1
fi

# Navigate to project root
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT"

echo "📁 Project root: $PROJECT_ROOT"

# Check if Xcode project exists
XCODE_PROJECT="Builds/MacOSX/AuricHalo.xcodeproj"
if [ ! -d "$XCODE_PROJECT" ]; then
    echo "❌ Xcode project not found at: $XCODE_PROJECT"
    echo "💡 Please open AuricHalo.jucer in Projucer and save project first"
    exit 1
fi

echo "🔨 Building with Xcode..."
cd "Builds/MacOSX"

# Clean previous build
echo "🧹 Cleaning previous build..."
xcodebuild clean -project AuricHalo.xcodeproj -scheme "AuricHalo - All" -configuration Release

# Build all targets
echo "⚙️  Building Release configuration..."
xcodebuild build -project AuricHalo.xcodeproj -scheme "AuricHalo - All" -configuration Release

# Check build success
if [ $? -eq 0 ]; then
    echo ""
    echo "✅ Build completed successfully!"
    echo ""
    echo "📦 Plugin locations:"
    echo "VST3: ~/Library/Audio/Plug-Ins/VST3/AuricHalo.vst3"
    echo "AU:   ~/Library/Audio/Plug-Ins/Components/AuricHalo.component"
    echo "AAX:  ~/Library/Application Support/Avid/Audio/Plug-Ins/AuricHalo.aaxplugin"
    echo "App:  /Applications/AuricHalo.app"
    echo ""
    echo "🎉 Ready to test in your DAW!"
else
    echo "❌ Build failed!"
    exit 1
fi
