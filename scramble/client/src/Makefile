CFLAGS := -g -O0 # optimizations cause buffer overflow on maas
# CFLAGS := -O3

CFLAGS += -DDEBUG
CFLAGS += -fopenmp
CFLAGS += -fstrict-aliasing 
CFLAGS += -std=gnu99 -Wall -fPIC -W -Waggregate-return -Wcast-align 
CFLAGS += -Wstrict-aliasing
# CFLAGS += -Wincompatible-pointer-types # comment for CentOS
CFLAGS += -Wmissing-prototypes -Wnested-externs -Wshadow -Wwrite-strings 
CFLAGS += -Wunused-variable -Wunused-parameter 
CFLAGS += -Wstrict-prototypes -Wmissing-prototypes -Wpointer-arith 
CFLAGS += -Wmissing-declarations -Wredundant-decls -Wnested-externs 
CFLAGS += -Wshadow -Wcast-qual -Wcast-align -Wwrite-strings 
CFLAGS += -Wold-style-definition 
CFLAGS += -Wattributes
CFLAGS += -Wunused-label 
# CFLAGS += -Wint-conversion # comment for CentOS
CFLAGS += -Wconversion
# CFLAGS += -Wnull-dereference  # comment for CentOS
CFLAGS += -Wpedantic 
CFLAGS += -Wuninitialized
#---------------
# WHY DID WE UNDO THIS CHECK? CFLAGS += -Wno-incompatible-pointer-types
# WHY DID WE UNDO THIS CHECK? CFLAGS += -Wno-cast-align
# WHY DID WE UNDO THIS CHECK? CFLAGS += -Wno-aggregate-return
# CFLAGS += -Wno-discarded-qualifiers
# WHY DID WE UNDO THIS CHECK? CFLAGS += -Wno-ignored-qualifiers
# not for mac CFLAGS += -Wsuggest-attribute=noreturn -Wduplicated-cond 
# not for mac CFLAGS += -Wmisleading-indentation 
# not for mac CFLAGS += -Wduplicated-branches -Wrestrict
# to use address sanitizer
# CFLAGS += -fsanitize=address -fno-omit-frame-pointer 
# CFLAGS += -fsanitize=undefined 
# CFLAGS +=  -static-libasan # for address sanitizer
# Got following error when I put static-libasan at end of LIBS
# ==1645146==Your application is linked against incompatible ASan runtimes.
# Something to do with order of linking???
#
#

INCS := -I../inc/ 
INCS += -I${RSUTILS_SRC_ROOT}/inc/ 
INCS += -I${RSLUTILS_SRC_ROOT}/inc/ 

# LUAJIT_SO := /usr/lib/x86_64-linux-gnu/libluajit-5.1.so.2
LUAJIT_SO := /usr/local/lib/libluajit-5.1.so
LIBS := -lm 
LIBS += -lpthread 
LIBS += -ljansson
LIBS += -levent 
LIBS += -lcurl 

MYLIBS += ${RSUTILS_SRC_ROOT}/src/librsutils.so 
MYLIBS += ${RSLUTILS_SRC_ROOT}/src/librslutils.so 


all: game_client  ../lua/game_state.lua


../lua/game_state.lua : ../inc/game_state.h
	echo " return [[ " > ../lua/game_state.lua 
	luajit ${RSUTILS_SRC_ROOT}/lua/cli_for_cdef.lua  \
		../inc/game_state.h \
		>> ../lua/game_state.lua 
	echo " ]] " >> ../lua/game_state.lua 

CC=gcc

.c.o :
	$(CC) -c -o $@ $< $(CFLAGS)  $(INCS)

game_client : \
	main.o \
	bridge_read_state.o \
	bridge_anagram.o \
	shuffle.o \
	setup_curl.o \
	execute.o \
	canonicalize.o \
	letter_counter.o \
	make_new_words.o \
	http_make_word.o \
	select_strings.o \
	genearator.o \
	${MYLIBS} \
	
	gcc \
	main.o \
	bridge_read_state.o \
	bridge_anagram.o \
	shuffle.o \
	setup_curl.o \
	execute.o \
	canonicalize.o \
	letter_counter.o \
	make_new_words.o \
	http_make_word.o \
	select_strings.o \
	genearator.o \
	\
	-o game_client  \
	${MYLIBS} \
	${LIBS} ${LUAJIT_SO}

clean :
	rm -f *.o *.so game_client _*  ../lua/game_state.lua
