# game

<h2>compilation</h2>

<h3>windows</h3>

currently working on compiling using cmake with windows, for now the visual studio project is entirely broken after changing the root folders name. stay tuned for more information on windows!

<h3>linux</h3>

to compile on debian:

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
