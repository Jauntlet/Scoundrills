#!/bin/bash

launch=true
build_type="Debug"

# Loop through the command-line arguments
while [[ $# -gt 0 ]]; do
    case "$1" in
        --no-launch)
            launch=false
            ;;
        --release)
            build_type="Release"
            ;;
        *)
            echo "Unknown argument: $1"
            exit 1
            ;;
    esac
    shift
done

# Run the build steps based on the conditions
mkdir -p build
cd build && cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=$build_type -G Ninja && ninja

# Check the exit status of make
make_status=$?
if [ $make_status -eq 0 ] && [ "$launch" = true ]; then
    ./scoundrills
fi

cd ..