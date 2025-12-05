#!/bin/bash

echo "==================================="
echo "  MAC MICROPHONE TEST"
echo "==================================="
echo ""
echo "Testing if Mac can record audio..."
echo ""

# Test recording with built-in mic
echo "Recording 3 seconds from built-in mic..."
rec -d coreaudio "Built-in Microphone" test_recording.wav trim 0 3 2>/dev/null

if [ -f "test_recording.wav" ]; then
    echo "✅ Recording successful!"
    echo "Playing back..."
    play test_recording.wav 2>/dev/null
    rm test_recording.wav
else
    echo "❌ Recording failed"
    echo ""
    echo "Trying alternative method..."
    ffmpeg -f avfoundation -i ":0" -t 3 test_recording.wav 2>&1 | tail -5
    
    if [ -f "test_recording.wav" ]; then
        echo "✅ Recording successful with ffmpeg!"
        afplay test_recording.wav
        rm test_recording.wav
    fi
fi

echo ""
echo "==================================="
echo "If recording works, the problem is"
echo "in the plugin, not macOS permissions"
echo "==================================="
