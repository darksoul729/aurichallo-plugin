#!/bin/bash
# Create installer background using ImageMagick or sips

WIDTH=620
HEIGHT=418

# Try to create with sips (built-in macOS)
if command -v sips &> /dev/null; then
    # Create a simple gradient background
    cat > /tmp/background.svg << 'SVGEOF'
<svg width="620" height="418" xmlns="http://www.w3.org/2000/svg">
  <defs>
    <linearGradient id="grad1" x1="0%" y1="0%" x2="0%" y2="100%">
      <stop offset="0%" style="stop-color:#1a1a1a;stop-opacity:1" />
      <stop offset="100%" style="stop-color:#0a0a0a;stop-opacity:1" />
    </linearGradient>
  </defs>
  <rect width="620" height="418" fill="url(#grad1)" />
  <text x="310" y="200" font-family="Arial" font-size="48" font-weight="bold" fill="#d4af37" text-anchor="middle">AURIC</text>
  <text x="310" y="250" font-family="Arial" font-size="56" font-weight="bold" fill="#a855f7" text-anchor="middle">HALO</text>
  <text x="310" y="290" font-family="Arial" font-size="16" fill="#888888" text-anchor="middle">Harmonic Saturator v1.1.0</text>
</svg>
SVGEOF
    
    # Convert SVG to PNG (requires rsvg-convert or similar)
    if command -v rsvg-convert &> /dev/null; then
        rsvg-convert /tmp/background.svg -o installer/resources/background.png
        echo "✓ Background created with rsvg-convert"
    else
        # Fallback: create simple solid color
        sips -s format png --resampleWidth 620 --resampleHeight 418 /System/Library/CoreServices/DefaultBackground.jpg --out installer/resources/background.png 2>/dev/null || true
        echo "✓ Background created (fallback)"
    fi
else
    echo "⚠️  sips not found, skipping background"
fi
