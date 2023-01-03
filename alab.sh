#!/bin/bash
PROBLEM=strikesback
TEST=test4

echo "COMPILING..."
mkdir tmp
cd $PROBLEM/src
cgal_create_CMakeLists -s a.out
cd ../../tmp
cmake -DCMAKE_BUILD_TYPE=Release ../$PROBLEM/src
make
cd ..
# clang++ -std=c++20 -Wno-deprecated-declarations $PROBLEM/src/main.cpp -o tmp/a.out
echo "RUNNING..."
time (cat $PROBLEM/public/$TEST.in | tmp/a.out > tmp/run.out)
echo "DIFF..."
diff $PROBLEM/public/$TEST.out tmp/run.out
