#!/bin/bash

echo "=========================================="
echo "  GRANT MICROPHONE PERMISSION MANUALLY"
echo "=========================================="
echo ""
echo "1. Opening System Settings..."
open "x-apple.systempreferences:com.apple.preference.security?Privacy_Microphone"

sleep 2

echo ""
echo "2. In System Settings:"
echo "   - Click the lock icon (bottom left) to unlock"
echo "   - Enter your password"
echo "   - Look for 'AuricHalo' in the list"
echo "   - If not there, click '+' button and add:"
echo "     Builds/MacOSX/build/Release/AuricHalo.app"
echo "   - Check the box next to 'AuricHalo'"
echo ""
echo "3. Close System Settings"
echo ""
echo "4. Restart AuricHalo:"
echo "   pkill AuricHalo"
echo "   open Builds/MacOSX/build/Release/AuricHalo.app"
echo ""
echo "=========================================="
