#!/bin/sh

cd "$(dirname "$0")"

if [ ! -d "../bin" ]; then
  ../make
fi

../bin/rshell << 'EOF'
echo this is it#bombino chomchino's willy wak attack don't take that flak
echo next is just a comment
#comment
echo next is just a comment with real input
#echo none of this should word


exit
EOF