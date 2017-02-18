#!/bin/sh

cd "$(dirname "$0")"

if [ ! -d "../bin" ]; then
  ../make
fi

../bin/rshell << 'EOF'
echo test;
echo second
echo "quotes"
echo \"all one quote\"
echo "one quote
echo /"one quote
mkdir test
git status
touch test.txt
ls -a
echoe fail
||||||||&&&&&&&&;;;;
|||||||&&&&&&&;;;
|| || || || && && && ; ;
|| || || || && && && ; echo test
exit
EOF

rm -f test.txt
rm -rf test