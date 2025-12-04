#!/bin/bash

# Test Script for AuricHalo Standalone Input Fix
# This script launches the app and monitors debug output

echo "========================================="
echo "AuricHalo Standalone Input Test"
echo "========================================="
echo ""

# Check if app exists
if [ ! -d "/Applications/AuricHalo.app" ]; then
    echo "❌ ERROR: AuricHalo.app not found in /Applications/"
    echo "Run: sudo cp -R 'Builds/MacOSX/build/Release/AuricHalo.app' /Applications/"
    exit 1
fi

echo "✅ Found AuricHalo.app"
echo ""

# Launch the app
echo "🚀 Launching AuricHalo standalone..."
open /Applications/AuricHalo.app

# Wait for app to start
sleep 2

echo ""
echo "📊 Monitoring debug output..."
echo "   (Press Ctrl+C to stop)"
echo ""
echo "INSTRUCTIONS:"
echo "1. In AuricHalo, click 'Options' or open Audio Settings"
echo "2. Select 'Loopback Audio' as Input Device"
echo "3. Enable Input channels 1+2"
echo "4. Play your guitar"
echo "5. Watch for 'Max sample value' below - should be > 0"
echo ""
echo "========================================="
echo ""

# Monitor console output
log stream --predicate 'process == "AuricHalo"' --level debug --style compact 2>/dev/null | grep -E "(STANDALONE|PREPARE|AUDIO DEBUG|Max sample|Input channels|Active input)"
