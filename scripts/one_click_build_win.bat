@echo off
echo Building Auric Halo for Windows...

if not exist build mkdir build
cd build

cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release

cpack -G NSIS

echo Build complete! Installer created in build directory.
pause
