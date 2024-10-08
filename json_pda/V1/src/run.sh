gcc -std=gnu99 -I../inc/ -fPIC -shared \
  parse_aux.c \
  parse_null.c \
  parse_boolean.c \
  parse_number.c \
  parse_string.c \
  parse_array.c \
  parse_object.c \
  parse_json.c \
  -o libjson.so

export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:$PWD"
gcc -std=gnu99 -I../inc/ ../test/ut1.c libjson.so -o ut1
./ut1
