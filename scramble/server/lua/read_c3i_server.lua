local ffi          = require 'ffi'
local x            = require 'c3i_server';  ffi.cdef(x)

local function read_c3i_server_configs(
  cC -- this is struct with C  configs
  )
  cC = ffi.cast("c3i_server_config_t *", cC)
  assert(type(glC) == "table") -- this must exist in Lua state 
  --====================================
  local port = assert(glC.port)
  assert(type(port) == "number")
  assert(port > 0)
  cC[0].port = port 
  --====================================
  local nU = assert(glC.nU)
  assert(type(nU) == "number")
  assert(nU > 0)
  cC[0].nU = nU 
  --====================================
  local nP0 = assert(glC.nP0)
  assert(type(nP0) == "number")
  assert(nP0 > 0)
  cC[0].nP0 = nP0 
  return true
end
return read_c3i_server_configs 
