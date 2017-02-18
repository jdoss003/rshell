#!/bin/sh

cd "$(dirname "$0")"

if [ ! -d "../bin" ]; then
  ../make
fi

../bin/rshell << 'EOF'
echo test exit as arg
echo test quotes && echo "this is in quotes" || echo quotes failed
echo nonfail condition test || echo fail; echo worked
echo pass condition test && echo this should work
echo pass condition that's empty && ; echo worked?
echo fail condition test; echoe bad input || echo worked
echoe bad input && echo pass || echo fail ; echo worked
ls -a; echo hello && mkdir test || echo world; ls -a; git status
echo this has a lot of arguments like too many of them &&   echo  this    has    too   many     spaces
exit
EOF

rm -rf test