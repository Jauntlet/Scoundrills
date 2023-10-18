# Scoundrills

<h2>Compilation</h2>

<h3>Windows</h3>

Windows compilation currently entirely depends on Visual Studio 2019, so compilation is done through VS instead of CMake. We have every intention of eventually changing that in favor of CMake.

<h3>Linux</h3>

to compile on Debian:

get dependencies
```
sudo apt install make cmake libglm-dev libsdl2-dev libglew-dev g++
```
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
compile using CMake (run in the build folder). we export the compile commands for clangd, you can skip that argument step if you don't use anything like that.
```
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1
make
```
the executable is called "game", so run this command to launch it
```
./game
```

<h3>MacOS</h3>

macOS is currently unsupported, although eventually CMake will be changed to work with mac as well. none of us currently own a computer running macOS, so this may take some time.

<h2>Distribution</h2>

<h3>Linux</h3>

on linux the 4 of us decided appimages would be the most ideal method of distribution.<br>
these arent currently working 100% but right now are pretty good.<br>
we dont have any way of really moving the dependencies over yet. eventually, this will be all sorted out, but until then you'll have to accept that our appimages have external dependencies.
if there is an automatic way to do this, or you have any preferred method, please create an issue. or email me about it, my email is quickkennedy at gmail dot com

appimage generation steps are as follows:

1. remove extra development files from your build folder.<br>
    we automatically throw out cmake build files, but things like callgrinds and any folder YOU MAKE are still included.

2. edit the `generate_appimage.sh` to include your games executable name.<br>
    **your executables name must match this, otherwise the generated appimage may be incorrect**<br>
    youll be editing line 4:
    ```
    1 | ...
    2 |
    3 | # Define the game name
    4 | GAMENAME="game"
    5 | 
    6 | ...
    ```
    just replace the `"game"` with your executables name.<br>
    **the executable name should still have quotes around it.**

3. run the script<br>
    use `bash generate_appimage.sh` to run the script, or double click on the script, assuming your desktop environment supports that kind of thing.<br>
    **please note: you need internet connection the first time you run this script, as it needs to download the latest appimagekit release**

if all goes well, you'll now have a .appimage file that will run your game.