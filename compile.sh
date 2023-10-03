
mkdir build
if [ "$1" == "--no-launch" ]; then
    cd build && cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug && make && cd ..
else
    cd build && cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug && make && ./scoundrills && cd ..
fi