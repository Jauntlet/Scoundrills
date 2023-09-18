# game

<h2>compilation</h2>

<h3>windows</h3>

currently working on compiling using cmake with windows, but for now you can open up the solution with visual studio 2019 and compile. eventually, this will be unsupported in favor of a cross platform cmake solution.

<h3>linux</h3>

to compile on debian:

get dependencies
```
sudo apt install make cmake libglm-dev libsdl2-dev libglew-dev g++
```

from here you have two options. either run the compile.sh file in the root of the program (`bash compile.sh`), or you can manually do the following:

create and move into "build" folder
```
mkdir build
cd build
```
compile using cmake (run in build folder). we export the compile commands for clangd, you can skip that argument step if you dont use anything like that.
```
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1
make
```
the executable is called "game", so run this command to launch it
```
./game
```

<h3>macos</h3>

macos is currently unsupported, although eventually cmake will be changed to work with mac as well. none of us currently own a computer running macos, so this may take some time.