#!/bin/bash

# Define the game name
GAMENAME="game"

# Define the AppDir directory
APPDIR="$GAMENAME.AppDir"

# Clean up any previous build
rm -rf "$APPDIR"

# Create the AppDir structure
mkdir -p "$APPDIR"

# Copy your executable to the AppDir
cp build/$GAMENAME "$APPDIR/$GAMENAME"

# Copy the icon to the AppDir
cp build/Textures/Icon.png "$APPDIR/$GAMENAME.png"

# Give the executable execute permission
chmod +x "$APPDIR/$GAMENAME"

# Copy the contents of the "build" directory recursively
cp -r build/* "$APPDIR"

# clean up build files
rm -r "$APPDIR/cmake_install.cmake"
rm -r "$APPDIR/CMakeFiles"
rm -r "$APPDIR/Makefile"
rm -r "$APPDIR/compile_commands.json"
rm -r "$APPDIR/CMakeCache.txt"

# Create an apprun
cat <<EOF > "$APPDIR/AppRun"
#!/bin/sh

# find what we execute
SELF=\$(readlink -f "\$0")

# find the directory we execute in
HERE=\${SELF%/*}

# go to that directory
cd \${HERE}

# run the game
./$GAMENAME
EOF

cat <<EOF > "$APPDIR/README.md"
Hi! This is the readme file for your generated appimage. You'll only really see this if you've extracted it to figure out what exactly we do and why we do it. appimages are terribly documented (from what we could find) and dont seem to really mention any standards. The jaunlet team apologizes for any issues caused by this, and asks that if you have any improvements to our appimage generation that you make a commit fixing our script.<br>
https://github.com/XanderMooney/Scoundrills/tree/Jauntlet-Template
EOF

# Give the executable execute permission
chmod +x "$APPDIR/AppRun"


wget -nc https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage

chmod +x appimagetool-x86_64.AppImage

# Create a desktop file
cat <<EOF > "$APPDIR/$GAMENAME.desktop"
[Desktop Entry]
Name=$GAMENAME
Exec=$GAMENAME
Icon=$GAMENAME
Type=Application
Categories=Game;
EOF

# Create the AppImage
./appimagetool-x86_64.AppImage "$APPDIR"

# Clean up newest build
rm -rf "$APPDIR"