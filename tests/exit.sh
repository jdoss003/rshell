#!/bin/sh

cd "$(dirname "$0")"

if [ ! -d "../bin" ]; then
  ../make
fi

../bin/rshell <<< 'echo this should exit && exit'

../bin/rshell <<< 'echoe this should exit || exit'

../bin/rshell <<< 'exit && echo this shouldnt run'

../bin/rshell <<< 'exit || echo this shouldnt run'

../bin/rshell <<< 'exit;'

../bin/rshell <<< '   exit  '

../bin/rshell <<< ';exit'
