# !bin/bush
rm -r "build"
mkdir build
cd build
cmake ../src
cmake --build .

