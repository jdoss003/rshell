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
cat < file1 | tr a-z A-Z > file1 #file1 should be blank after this runs
cat < file1 | tr a-z A-Z >> file1
cat < file1 | tr A-Z a-z
cat < (echo a && echo b) | tr A-Z a-z > file2 #Should parentheses error
file1>file2 #Should error
file1<file2 #Shouldnt work. Should error. Throws waitpid error that breaks code.

exit
EOF