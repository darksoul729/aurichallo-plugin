#!/bin/bash

echo "========================================="
echo "AuricHalo - Built-in Microphone Test"
echo "========================================="
echo ""

# Kill existing instance
killall AuricHalo 2>/dev/null
sleep 1

echo "📋 Pre-flight Checks:"
echo ""

# Check if app exists
if [ -d "/Applications/AuricHalo.app" ]; then
    echo "✅ AuricHalo.app found"
else
    echo "❌ AuricHalo.app NOT found"
    exit 1
fi

# Check macOS microphone permissions
echo ""
echo "🔒 Checking macOS Microphone Permissions..."
echo "   If prompted, ALLOW microphone access for AuricHalo"
echo ""

# Launch app
echo "🚀 Launching AuricHalo..."
open /Applications/AuricHalo.app

sleep 3

echo ""
echo "========================================="
echo "📊 MONITORING DEBUG OUTPUT"
echo "========================================="
echo ""
echo "INSTRUCTIONS:"
echo "1. In AuricHalo, open Audio Settings"
echo "2. Select 'Built-in Microphone' as Input Device"
echo "3. Enable Input channels 1+2"
echo "4. SPEAK LOUDLY or CLAP near your Mac"
echo "5. Watch the output below for 'Max sample value'"
echo ""
echo "Expected: Max sample value should be > 0.0 when you make noise"
echo ""
echo "========================================="
echo ""

# Monitor with better filtering
log stream --process AuricHalo --level debug --style compact 2>/dev/null | \
  grep --line-buffered -E "(STANDALONE|PREPARE|AUDIO DEBUG|Max sample|Input channels|Active input|Buffer|Error|error)" | \
  while IFS= read -r line; do
    # Highlight important values
    if echo "$line" | grep -q "Max sample value"; then
      echo ">>> $line <<<"
    else
      echo "$line"
    fi
  done
