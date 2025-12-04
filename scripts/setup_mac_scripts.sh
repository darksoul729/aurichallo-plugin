#!/bin/bash

echo "🔧 Setting up macOS build scripts..."

# Make all scripts executable
chmod +x one_click_build_mac.sh
chmod +x build_and_package_mac.sh  
chmod +x create_mac_installer.sh

echo "✅ All scripts are now executable!"
echo ""
echo "Available scripts:"
echo "• ./one_click_build_mac.sh        - Build project only"
echo "• ./create_mac_installer.sh       - Create installer only"  
echo "• ./build_and_package_mac.sh      - Build + create installer"
echo ""
echo "Usage:"
echo "cd AuricHalo/scripts"
echo "./build_and_package_mac.sh"