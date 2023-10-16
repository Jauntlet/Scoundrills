#!/bin/bash

# Define the AppDir directory
APPDIR="Scoundrills.AppDir"

# Clean up any previous build
rm -rf "$APPDIR"

# Create the AppDir structure
mkdir -p "$APPDIR/usr/bin"

# Copy your executable to the AppDir
cp build/scoundrills "$APPDIR/usr/bin"

# Copy the icon to the AppDir
cp build/Textures/Icon.png "$APPDIR/scoundrills.png"

# Create a desktop file
cp extras/scoundrills.desktop "$APPDIR/scoundrills.desktop"

# Give the executable execute permission
chmod +x "$APPDIR/usr/bin/scoundrills"

# Copy the contents of the "build" directory recursively
cp -r build/Fonts "$APPDIR/usr/bin"
cp -r build/Levels "$APPDIR/usr/bin"
cp -r build/Shaders "$APPDIR/usr/bin"
cp -r build/Textures "$APPDIR/usr/bin"

# Create aan apprun
cp extras/AppRun "$APPDIR/AppRun"

# Give the executable execute permission
chmod +x "$APPDIR/AppRun"

# Create the AppImage
./appimagetool-x86_64.AppImage "$APPDIR"

# Clean up newest build
rm -rf "$APPDIR"