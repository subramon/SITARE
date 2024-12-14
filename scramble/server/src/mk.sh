#!/bin/bash
set -e 

INCS="-I$HOME/RSUTILS/inc/ -I$HOME/RSLUTILS/inc/ "
FLAGS=" -g "
SRCS="main.c $HOME/RSLUTILS/src/mk_lua_state.c "
LIBS="/usr/local/lib/libluajit-5.1.so"

gcc ${INCS} ${FLAGS} ${SRCS} -o run ${LIBS}

echo "All done"
