gcc -g -std=gnu99 -I../inc/ -I../../../../RSUTILS/inc/ -fPIC -shared \
  ../src/parse_aux.c \
  ../src/parse_null.c \
  ../src/parse_boolean.c \
  ../src/parse_number.c \
  ../src/parse_string.c \
  ../src/parse_json.c \
  ../src/free_json.c \
  ../../../../RSUTILS/src/txt_to_F8.c \
  -o libjson.so

export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:$PWD"
gcc -g -std=gnu99 -I../inc/ ut1.c libjson.so -o ut1
./ut1

#  parse_number.c \
#  parse_string.c \
#  parse_array.c \
#  parse_object.c \
#  parse_json.c \
