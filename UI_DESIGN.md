# Auric Halo - UI Design Documentation

## Design Concept
Premium dark interface dengan halo glow effect yang responsif terhadap parameter Drive.

## Color Palette
- **Background**: `#0a0a0a` (Almost black)
- **Gold/Brass**: `#d4af37` (Titles, labels, borders)
- **Purple**: `#b494ff` (Subtitle, glow effects)
- **Orange/Gold Glow**: `#ffa366` (Halo outer glow)
- **Dark Grey**: `#1a1a1a` (Knob backgrounds)

## Layout

```
┌─────────────────────────────────────────┐
│              AURIC                      │
│              HALO                       │
│        HARMONIC SATURATOR               │
│                                         │
│   [TONE]         [DRIVE]      [MIX]    │
│    knob       (main knob)      knob    │
│                                         │
│                              [OUTPUT]   │
│  HALO ENGINE                   knob    │
│  [ON/OFF]                               │
└─────────────────────────────────────────┘
```

## Components

### 1. Title Section
- **AURIC**: 42pt bold, gold color
- **HALO**: 56pt bold, gold color
- **HARMONIC SATURATOR**: 16pt, purple color

### 2. Knobs
**Main Drive Knob (Center)**
- Size: 200x220px
- Features:
  - Halo glow effect (intensity based on value)
  - Gold ring border
  - Purple-to-orange gradient arc
  - Gold pointer indicator
  - 12 decorative dots around perimeter

**Secondary Knobs (Tone, Mix, Output)**
- Size: 100x120px
- Same styling as main knob
- 8 decorative dots around perimeter
- Gold labels below

### 3. Halo Engine Toggle
- Position: Bottom left
- Two states: ON (active) / OFF (inactive)
- Gold border and text
- Controls the halo glow effect visibility

## Halo Glow Effect

The signature feature - multi-layer glow around the Drive knob:

**Inner Layers (Purple)**
- 5 concentric circles
- Alpha: 0.15 * intensity * (layer/5)
- Color: RGB(180, 100, 255)

**Outer Layers (Orange/Gold)**
- 3 concentric circles
- Alpha: 0.2 * intensity * (layer/3)
- Color: RGB(255, 180, 100)

**Intensity Calculation**
```cpp
float intensity = driveValue / 100.0f;
```

## Knob Design Details

### Visual Elements
1. **Outer Ring**: Dark grey (#1a1a1a)
2. **Inner Circle**: Almost black (#0a0a0a)
3. **Gold Border**: 2px stroke (#d4af37)
4. **Value Arc**: Gradient from purple to orange
5. **Pointer**: Gold rectangular indicator
6. **Center Dot**: Gold circle (6px radius)

### Interaction
- Drag vertically or horizontally to adjust
- No text box (clean look)
- Smooth rotation animation
- Range: 0-100

## Typography
- **Font Family**: Arial
- **Title (AURIC)**: 42pt Bold
- **Title (HALO)**: 56pt Bold
- **Subtitle**: 16pt Regular
- **Labels**: 14pt Regular
- **Button Text**: 14pt Bold

## Decorative Elements
- Subtle vignette effect on background
- Decorative dots around each knob
- Smooth gradients on value arcs
- Multi-layer glow effects

## Responsive Behavior
- Window size: 700x500px (fixed)
- All elements positioned absolutely
- Glow intensity responds to Drive parameter
- Button state changes color on toggle

## Implementation Notes

### Key Files
- `AuricHaloEditor.cpp`: Main UI layout and painting
- `LookAndFeel.cpp`: Custom knob rendering
- `HaloKnob.cpp`: Knob component wrapper

### Performance Considerations
- Glow effect only renders when Halo Engine is ON
- Use of colour gradients for smooth transitions
- Efficient path rendering for arcs and circles

## Future Enhancements
- Animated particles around active knobs
- Spectrum analyzer in background
- Preset browser UI
- Resizable window support
- Custom font loading
