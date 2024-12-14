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
    ]]
ffi.cdef(hdr)
local make_so_cmd = [[
gcc -DDEBUG -fPIC -shared ../src/letter_counter.c -I../inc/ -I../../../inc/ -o liblc.so
]]
assert(cutils.delete("liblc.so"))
os.execute(make_so_cmd)
assert(cutils.isfile("liblc.so"))

local lib = ffi.load("liblc.so")
local cfn = assert(lib.letter_counter)
local prfn = assert(lib.pr_letter_counter)

local good_strs = { "abc", "aaaa", }
local bad_strs = { "XXXX", "!&#^&!", }

local NUM_ALPHABET = 26 
counts = ffi.C.malloc(NUM_ALPHABET * ffi.sizeof("uint8_t"))
for k, str in pairs(good_strs) do 
  local status = cfn(str, counts)
  assert(status == 0)
  prfn(counts)
  ffi.fill(counts, NUM_ALPHABET * ffi.sizeof("uint8_t"), 0)
end

for k, str in pairs(bad_strs) do 
  local status = cfn(str, counts)
  assert(status ~= 0)
end
  
print("All done")


