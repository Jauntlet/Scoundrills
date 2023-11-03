# this script was made by ponyfan88 (jack kennedy)
# it is to run valgrind to test the program. do what you need to test for performance in the program.
# once the program finishes, valgrind will write its profile to callgrind.out.* where * is some number i dont care about.
# kcachegrind will then open it (its the only program i like for these, if you find a better one NOT based in the terminal you can email me)
# once you close kcachegrind you can continue with your life.
# please note: this WILL remove every callgrind in the build folder before generating a new one, so dont even think about saving them.

cd build && rm callgrind.out.*
valgrind --tool=callgrind ./scoundrills
cd ..
kcachegrind build/callgrind.out.*
echo ----------
echo go outside