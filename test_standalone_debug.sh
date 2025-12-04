#!/bin/bash

echo "========================================="
echo "AuricHalo Standalone - Debug Monitor"
echo "========================================="
echo ""

# Kill any existing instance
killall AuricHalo 2>/dev/null

echo "🚀 Launching AuricHalo..."
open /Applications/AuricHalo.app

sleep 2

echo ""
echo "📊 Debug Output (Press Ctrl+C to stop):"
echo "========================================="
echo ""

# Monitor console with better filtering
log stream --process AuricHalo --level debug --style compact 2>/dev/null | \
  grep --line-buffered -E "(STANDALONE|PREPARE|AUDIO DEBUG|Max sample|Input channels|Active input|Buffer|channels:|samples:)"
