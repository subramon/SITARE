-- test for letter_counter 
local cutils = require 'libcutils'
local ffi    = require 'ffi'
local hdr = [[
extern void *malloc(size_t size);
extern int
letter_counter(
    const char * const in_str, // input 
    uint8_t *count
    );
extern int
pr_letter_counter(
    uint8_t *count
    );
extern int
canonicalize_1(
    const char * const in_str, // input 
    uint8_t *cstr
    );
    ]]
ffi.cdef(hdr)
local make_so_cmd = [[
gcc -DDEBUG -fPIC -shared ../src/canonicalize.c ../src/letter_counter.c -I../inc/ -I../../../inc/ -o liblc.so
]]
assert(cutils.delete("liblc.so"))
os.execute(make_so_cmd)
assert(cutils.isfile("liblc.so"))

local lib = ffi.load("liblc.so")
local lcfn = assert(lib.letter_counter)
local canfn = assert(lib.canonicalize_1)

local good_strs = { "helloworld", "thequickbrownfoxjumpedoverthelazydog", }

local NUM_ALPHABET = 26 
local len = 2*NUM_ALPHABET+1
can_str = ffi.C.malloc(len * ffi.sizeof("char"))
for k, str in pairs(good_strs) do 
  ffi.fill(can_str, len * ffi.sizeof("char"), 0)
  local status = canfn(str, can_str)
  assert(status == 0)
  print(ffi.string(can_str))
end
  
print("All done")


