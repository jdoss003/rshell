#!/bin/sh

cd "$(dirname "$0")"

if [ ! -d "../bin" ]; then
  ../make
fi

../bin/rshell << 'EOF'
mkdir file1
touch text.txt
test -e file1
test file1
[ -e file1 ]
test -e file2
test file2
[ -e file2 ]
test -f text.txt
test -d text.txt
test -d file1
test -f file1
test -d file1 && test -f text.txt
test -f file1 && test -f text.txt
test -f file1 || test -f text.txt
[ file1 ]
[ file2 ]
test -f file1 || echo works
exit
EOF
rm -rf file1
rm -f test.txt