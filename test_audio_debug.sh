#!/bin/bash

echo "========================================="
echo "AuricHalo Audio Input - Debug Test"
echo "========================================="
echo ""

echo "Step 1: Building DEBUG version..."
xcodebuild -project Builds/MacOSX/AuricHalo.xcodeproj \
  -scheme "AuricHalo - Standalone Plugin" \
  -configuration Debug \
  build 2>&1 | tail -3

echo ""
echo "Step 2: Installing debug app..."
sudo rm -rf /Applications/AuricHalo.app
sudo cp -R "Builds/MacOSX/build/Debug/AuricHalo.app" /Applications/
echo "✅ Debug version installed"

echo ""
echo "Step 3: Launching app..."
open /Applications/AuricHalo.app

sleep 3

echo ""
echo "========================================="
echo "📊 MONITORING DEBUG OUTPUT"
echo "========================================="
echo ""
echo "INSTRUCTIONS:"
echo "1. In AuricHalo, find audio settings"
echo "2. Select 'Built-in Microphone' as input"
echo "3. Enable Input channels 1+2"
echo "4. SPEAK LOUDLY or CLAP"
echo "5. Watch for debug messages below"
echo ""
echo "Press Ctrl+C to stop monitoring"
echo "========================================="
echo ""

# Monitor with better filtering
log stream --process AuricHalo --level debug --style compact 2>/dev/null | \
  grep --line-buffered -E "(PREPARE|AUDIO|Input|Output|channels|sample|Buffer|STANDALONE)" | \
  while IFS= read -r line; do
    echo ">>> $line"
  done
