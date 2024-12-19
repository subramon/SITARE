local ffi       = require 'ffi'
local cutils    = require 'libcutils'
local JSON      = require 'RSUTILS/lua/JSON'
local stringify = require 'RSUTILS/lua/stringify'
local tbl_to_C_2d = require 'RSUTILS/lua/tbl_to_C_2d'
local x         = require 'scramble/client/lua/game_state';  ffi.cdef(x)

function read_state(
  json_response,
  cS -- state in C 
  )
  assert(type(json_response) == "string")
  local J = JSON:decode(json_response)
  assert(type(J) == "table")

  assert(cS)
  cS = ffi.cast("game_state_t *", cS)
  --==================================================
  local clock = J.clock
  assert(type(clock) == "number")
  assert(clock> 0)
  cS[0].clock = clock
  --==================================================
  local all_Wplus = {}
  local all_Wminus = {}
  local H = assert(J.history)
  assert(type(H) == "table")
  for i = 1, #H do 
    local Wplus = H[i].Wplus
    assert(type(Wplus) == "table")
    assert(#Wplus >= 1)
    for _, w in ipairs(Wplus) do
      all_Wplus[#all_Wplus+1] = w
    end 
    local Wplus = H[i].Wplus
    if ( Wminus ) then 
      assert(type(Wminus) == "table")
      assert(#Wminus >= 1)
      for _, w in ipairs(Wminus) do
        all_Wminus[#all_Wminus+1] = w
      end 
    end
  end 
  -- subtract all_Wminus from all_Wplus to create curr_words
  -- all_Wminus becomes prev_words
  if ( #all_Wminus > 0 ) then
    cS[0].prev_words, cS[0].nprev = tbl_to_C_2d(all_Wminus)
  end
  -- add letters from ppol
  local P = assert(J.pool)
  assert(type(P) == "table")
  assert(#P > 1)
  cS[0].letters, cS[0].nlttr = tbl_to_C_2d(P)

  -- TODO fill in the blanks
  return true
end
return read_state
