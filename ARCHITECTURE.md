# Auric Halo Architecture

## Overview
Auric Halo is a professional audio plugin built with JUCE, featuring custom DSP algorithms for EQ, compression, and saturation.

## Signal Flow
```
Input → HaloEQ → HaloCompressor → HaloSaturator → Output
                      ↓
                  HaloMeter (visualization)
```

## Module Breakdown

### DSP Chain
- **HaloEQ**: Multi-band parametric EQ with custom "halo" frequency response
- **HaloCompressor**: Transparent dynamics processor with auto-makeup
- **HaloSaturator**: Harmonic enhancement with adjustable drive

### UI Components
- **HaloMeter**: Real-time level and frequency visualization
- **HaloKnob**: Custom rotary controls with value display
- **LookAndFeel**: Unified visual styling

### Parameter Management
All plugin parameters are centralized in `Parameters.h/cpp` using JUCE's AudioProcessorValueTreeState.

## Build System
CMake-based build with CPack for cross-platform installers.
