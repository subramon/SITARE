local ffi = require 'ffi'
local cutils = require 'libcutils'
local wordfile = "../../wordlist"
assert(cutils.isfile(wordfile))

-- http://lua-users.org/wiki/FileInputOutput

-- see if the file exists
local function file_exists(file)
  local f = io.open(file, "rb")
  if f then f:close() end
  return f ~= nil
end

-- get all lines from a file, returns an empty
-- list/table if the file does not exist
local function lines_from(file)
  if not file_exists(file) then return {} end
  local lines = {}
  for line in io.lines(file) do
    -- we are interested in words with at least 3 letters
    if ( #line >= 3 ) then 
      lines[#lines + 1] = line
    end
  end
  return lines
end

local words = lines_from(wordfile)
print("Read " .. #words  .. " words")
--==================================
local hdr = [[
extern void free(void *ptr);
extern void *malloc(size_t size);
extern int
canonicalize_1(
    const char * const in_str, // input 
    uint8_t *cstr
    );
    ]]
ffi.cdef(hdr)

local src_files = {
  "../../canonical_form/src/canonicalize.c",
  "../../canonical_form/src/letter_counter.c",
}
local str_src_files = table.concat(src_files, " ")

local incs = { 
  "-I../../canonical_form/inc/", 
  "-I../../../inc/", 
}
local str_incs = table.concat(incs, " ")

local flags = "-g -DDEBUG -fPIC -shared "
local make_so_cmd = string.format(
  "gcc %s %s %s -o liblc.so", flags, str_src_files, str_incs)
assert(cutils.delete("liblc.so"))
-- print(make_so_cmd)
os.execute(make_so_cmd)
assert(cutils.isfile("liblc.so"))
local lib = ffi.load("liblc.so")
local canfn = assert(lib.canonicalize_1)

local W = {} -- given the canonical form of a word, returns anagrams
local NUM_ALPHABET = 26 
local len = 2*NUM_ALPHABET+1
can_str = ffi.C.malloc(len * ffi.sizeof("char"))
for k, str in pairs(words) do 
  ffi.fill(can_str, len * ffi.sizeof("char"), 0)
  -- print(k, str)
  local status = canfn(str, can_str)
  assert(status == 0)
  local lstr = ffi.string(can_str) -- C string to Lua string 
  if ( not W[lstr] ) then
    W[lstr] = { str }
  else
    local x = W[lstr]
    assert(type(x) == "table")
    x[#x+1] = str
    W[lstr] = x
  end
end
local nW = 0 
for k, v in pairs(W) do nW = nW + 1 end 
-- print anagarams
for k, v in pairs(W) do 
  assert(type(v) == "table")
  if ( #v > 1 ) then
    for k2, v2 in pairs(v) do 
      assert(type(v2) == "string")
    end
    local x = table.concat(v, " ")
    print(k, x)
  end
end
print("#W = ", nW)
ffi.C.free(can_str)
print("All done")
