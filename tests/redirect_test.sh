#!/bin/sh

cd "$(dirname "$0")"

if [ ! -d "../bin" ]; then
  ../make
fi

../bin/rshell << 'EOF'
(echo b && echo c) > file1
(ls -a && echo herro) >> file1
cat < file1 | tr a-z A-Z > file2
(echo c && echo d) | tr A-Z a-z >> file2
(echo a && echo b) | tr A-Z a-z > file2
cat < file1 | tr A-Z a-z | tee file2 | tr a-z A-Z > file3
cat < file1 | tr a-z A-Z > file1
exit
EOF