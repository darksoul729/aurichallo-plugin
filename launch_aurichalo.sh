#!/bin/bash
# Launcher for AuricHalo with proper microphone permissions

# Kill any existing instance
pkill -9 AuricHalo 2>/dev/null

# Launch from Terminal (this inherits Terminal's mic permission)
/Applications/AuricHalo.app/Contents/MacOS/AuricHalo &

echo "AuricHalo launched!"
echo "Audio input should work now."
