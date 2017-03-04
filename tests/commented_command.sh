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
test text.txt #this should be false
#test text.txt #this should be false
echo comment; #(echo hello && echo works)
echo comment; (echo cut #commentcomment)
exit
EOF