#!/bin/bash
# AuricHalo Launcher - Use this to launch with audio input working

pkill -9 AuricHalo 2>/dev/null
/Applications/AuricHalo.app/Contents/MacOS/AuricHalo &

echo "✅ AuricHalo launched!"
echo "Audio input should work now."
echo ""
echo "You can close this Terminal window."
