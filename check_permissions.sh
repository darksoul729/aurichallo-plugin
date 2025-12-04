#!/bin/bash

echo "========================================="
echo "macOS Audio Permissions Checker"
echo "========================================="
echo ""

echo "🔍 Checking Audio Input Permissions..."
echo ""

# Check TCC database for microphone permissions
# Note: This requires Full Disk Access to read TCC database
echo "📋 Apps with Microphone Access:"
echo ""

# Try to read TCC database (may require sudo)
if [ -f ~/Library/Application\ Support/com.apple.TCC/TCC.db ]; then
    echo "✅ TCC database found"
    echo ""
    echo "To check permissions manually:"
    echo "1. Open System Settings"
    echo "2. Go to Privacy & Security"
    echo "3. Click on Microphone"
    echo "4. Look for 'AuricHalo' in the list"
    echo "5. Make sure it's CHECKED (enabled)"
else
    echo "⚠️  Cannot read TCC database directly"
fi

echo ""
echo "========================================="
echo "🎤 Testing System Audio Input"
echo "========================================="
echo ""

# Check available audio devices
echo "Available Audio Input Devices:"
system_profiler SPAudioDataType | grep -A 5 "Input"

echo ""
echo "========================================="
echo "📝 Manual Permission Check Steps:"
echo "========================================="
echo ""
echo "1. Open System Settings (System Preferences)"
echo "2. Click 'Privacy & Security'"
echo "3. Scroll down and click 'Microphone'"
echo "4. Look for 'AuricHalo' in the list"
echo "5. If found, make sure checkbox is CHECKED"
echo "6. If NOT found, launch AuricHalo once to trigger permission request"
echo ""
echo "Then run: ./test_builtin_mic.sh"
echo ""

# Try to trigger permission dialog
echo "🚀 Launching AuricHalo to trigger permission dialog..."
echo "   (If prompted, click ALLOW)"
echo ""

open /Applications/AuricHalo.app

sleep 2

echo ""
echo "✅ Done! Check if permission dialog appeared."
echo "   If yes, click ALLOW and run: ./test_builtin_mic.sh"
echo ""
