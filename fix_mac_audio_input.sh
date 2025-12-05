#!/bin/bash

echo "=========================================="
echo "  FIX MAC AUDIO INPUT - AURIC HALO"
echo "=========================================="
echo ""

echo "✅ Changes applied:"
echo "  1. Created custom StandaloneApp.cpp"
echo "  2. Disabled default JUCE standalone"
echo "  3. Updated .jucer configuration"
echo ""

echo "📋 NEXT STEPS:"
echo ""
echo "1. Open Projucer:"
echo "   - Find Projucer in your JUCE installation"
echo "   - Usually in: /Applications/JUCE/Projucer.app"
echo "   - Or: ~/JUCE/Projucer.app"
echo ""
echo "2. In Projucer:"
echo "   - File → Open → AuricHalo.jucer"
echo "   - Click 'Save Project' (Cmd+S)"
echo "   - This will regenerate Xcode project with our fixes"
echo ""
echo "3. Build Standalone:"
echo "   cd Builds/MacOSX"
echo "   xcodebuild -project AuricHalo.xcodeproj \\"
echo "     -scheme 'AuricHalo - Standalone Plugin' \\"
echo "     -configuration Release \\"
echo "     clean build"
echo ""
echo "4. Install:"
echo "   sudo cp -R build/Release/AuricHalo.app /Applications/"
echo ""
echo "5. Test:"
echo "   - Open /Applications/AuricHalo.app"
echo "   - Select Built-in Microphone as input"
echo "   - Speak/clap → Input meter should move!"
echo ""
echo "=========================================="
echo ""
echo "🔍 What was fixed:"
echo ""
echo "The problem: JUCE's default standalone wrapper"
echo "initializes with 0 input channels for effect plugins."
echo ""
echo "The fix: Our custom StandaloneApp.cpp forces"
echo "audio input to be enabled with this code:"
echo ""
echo "  deviceManager.initialise("
echo "      2,  // ← FORCE 2 input channels (was 0)"
echo "      2,  // 2 output channels"
echo "      ..."
echo "  );"
echo ""
echo "This is the same fix that makes it work on Windows!"
echo ""
echo "=========================================="
echo ""

# Check if Projucer exists in common locations
PROJUCER_PATHS=(
    "/Applications/JUCE/Projucer.app/Contents/MacOS/Projucer"
    "~/JUCE/Projucer.app/Contents/MacOS/Projucer"
    "/Applications/Projucer.app/Contents/MacOS/Projucer"
)

PROJUCER_FOUND=""
for path in "${PROJUCER_PATHS[@]}"; do
    expanded_path="${path/#\~/$HOME}"
    if [ -f "$expanded_path" ]; then
        PROJUCER_FOUND="$expanded_path"
        break
    fi
done

if [ -n "$PROJUCER_FOUND" ]; then
    echo "✅ Found Projucer at: $PROJUCER_FOUND"
    echo ""
    read -p "Do you want to open Projucer now? (y/n) " -n 1 -r
    echo ""
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        open -a "$PROJUCER_FOUND" AuricHalo.jucer
        echo "✅ Projucer opened!"
        echo "   Remember to click 'Save Project' (Cmd+S)"
    fi
else
    echo "⚠️  Projucer not found in common locations."
    echo "   Please locate and open it manually."
fi

echo ""
echo "=========================================="
