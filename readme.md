# Scoundrills

<h2>Compilation</h2>

<h3>Windows</h3>

Windows compilation currently entirely depends on Visual Studio 2019, so compilation is done through VS instead of CMake. We have every intention of eventually changing that in favor of CMake.

<h3>Linux</h3>

to compile on Debian:

get dependencies
```
sudo apt install ninja-build cmake libglm-dev libsdl2-dev libglew-dev g++ libfreetype-dev
```
it is worth noting that libfreetype should be installed under `/usr/include/freetype2/` or **must** have its path manually changed in `CMakeLists.txt`
clone and enter the repository
```
git clone https://github.com/XanderMooney/Scoundrills
cd Scoundrills
```

from here you have two options. either run the compile.sh file in the root of the program (`bash compile.sh`), or you can manually do the following:

create and move into the "build" folder
```
mkdir build
cd build
```
compile using CMake and ninja (run in the build folder). we export the compile commands for clangd, you can skip that argument step if you don't use anything like that.
```
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1
ninja
```
the executable is called "scoundrills", so run this command to launch it
```
./scoundrills
```

<h3>MacOS</h3>

macOS is currently unsupported, although eventually CMake will be changed to work with mac as well. none of us currently own a computer running macOS, so this may take some time.
