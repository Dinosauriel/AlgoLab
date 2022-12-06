#!/bin/bash
PROBLEM=rubeus_hagrid
TEST=test4

mkdir tmp
clang++ -std=c++20 $PROBLEM/src/main.cpp -o tmp/a.out
cat $PROBLEM/public/$TEST.in | tmp/a.out > tmp/run.out
diff $PROBLEM/public/$TEST.out tmp/run.out