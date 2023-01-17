#!/bin/bash
PROBLEM=$1
TEST=$2
RED='\033[0;31m'
GREEN='\033[0;32m'
BOLD='\033[1m'
NC='\033[0m'

echo "running problem ${PROBLEM} with test set ${TEST}"
echo "COMPILING..."
rm -rf tmp
mkdir tmp
cd problem/$PROBLEM/src
cgal_create_CMakeLists -s a.out > /dev/null
cd ../../../tmp
cmake -DCMAKE_BUILD_TYPE=Release ../problem/$PROBLEM/src > /dev/null
make
if [ $? -ne 0 ]; then
    echo -e "${RED}${BOLD}make did not succeed. aborting...${NC}"
    exit
fi
cd ..
# clang++ -std=c++20 -Wno-deprecated-declarations $PROBLEM/src/main.cpp -o tmp/a.out
echo "RUNNING..."
time (cat problem/$PROBLEM/public/$TEST.in | tmp/a.out | tee tmp/run.out)
echo "DIFF..."
diff --color=auto problem/$PROBLEM/public/$TEST.out tmp/run.out
if [ $? -eq 0 ]; then
    echo -e "${GREEN}${BOLD}correct answer!${NC}"
else
    echo -e "${RED}${BOLD}wrong answer${NC}"
fi
