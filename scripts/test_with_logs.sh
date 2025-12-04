#!/bin/bash

echo "🧪 Testing License Activation with Logs"
echo "========================================"
echo ""

# Reset license
rm -f ~/Library/Application\ Support/AuricAudio/.aurichalo_lic
echo "✅ License reset"
echo ""

# Launch app in background
echo "🚀 Launching standalone app..."
open Builds/MacOSX/build/Release/AuricHalo.app

echo ""
echo "📋 To see debug logs:"
echo "1. Open Console.app"
echo "2. Search for 'AuricHalo' or 'License'"
echo "3. Or run: log stream --predicate 'process == \"AuricHalo\"' --level debug"
echo ""
echo "🔑 Test license activation:"
echo "1. Copy Hardware ID from dialog"
echo "2. Generate key: python3 scripts/generate_license_key.py HWID"
echo "3. Paste key and click Activate"
echo "4. Check console for debug messages"
