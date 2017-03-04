#!/bin/sh

cd "$(dirname "$0")"

if [ ! -d "../bin" ]; then
  ../make
fi

../bin/rshell << 'EOF'
(echo hello && echo works);
echo start && (echo pass && echo pass2)
echoe start || (echoe pass || echo works)
echo comment; #(echo hello && echo works)
echo start && (echo pass && (echoe fail || echo works))
echo start && (echoe fail || (echo pass || echo fail))
echo start && (echoe fail || (echo pass || echo fail)
echo start && echoe fail || (echo pass || echo fail))
(echo shouldnt) (echo work)
(((((((())))))))
(((((((()))))))
()()()()()
exit
EOF