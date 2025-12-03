#!/bin/bash
# Post-install script for macOS

echo "Installing Auric Halo plugins..."

# Validate plugins
pluginval --validate /Library/Audio/Plug-Ins/VST3/AuricHalo.vst3 || true
pluginval --validate /Library/Audio/Plug-Ins/Components/AuricHalo.component || true

echo "Installation complete!"
exit 0
