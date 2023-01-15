#!/bin/bash
PROBLEM=$1
TEST=$2

echo "running problem ${PROBLEM} with test set ${TEST}"
echo "COMPILING..."
rm -rf tmp
mkdir tmp
cd problem/$PROBLEM/src
cgal_create_CMakeLists -s a.out > /dev/null
cd ../../../tmp
cmake -DCMAKE_BUILD_TYPE=Release ../problem/$PROBLEM/src > /dev/null
make
cd ..
# clang++ -std=c++20 -Wno-deprecated-declarations $PROBLEM/src/main.cpp -o tmp/a.out
echo "RUNNING..."
time (cat problem/$PROBLEM/public/$TEST.in | tmp/a.out > tmp/run.out)
echo "DIFF..."
diff problem/$PROBLEM/public/$TEST.out tmp/run.out
