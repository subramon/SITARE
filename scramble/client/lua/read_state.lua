local ffi       = require 'ffi'
local cutils    = require 'libcutils'
local JSON      = require 'RSUTILS/lua/JSON'
local stringify = require 'RSUTILS/lua/stringify'
local tbl_to_C_2d = require 'RSUTILS/lua/tbl_to_C_2d'
local subtract_from = require 'subtract_from' -- scramble/server/lua
local x         = require 'game_state';  ffi.cdef(x)

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
  local used_words = {}
  local H = assert(J.history)
  assert(type(H) == "table")
  for i = 1, #H do 
    local Wplus = H[i].Wplus
    assert(type(Wplus) == "table")
    assert(#Wplus >= 1)
    for _, w in ipairs(Wplus) do
      used_words[#used_words+1] = w
    end 
  end 
  if ( #used_words > 0 ) then 
    cS[0].used_words, cS[0].nused = tbl_to_C_2d(used_words)
  end
  --==================================================
  local curr_words = assert(J.words)
  assert(type(curr_words) == "table")
  if ( #curr_words > 0 ) then 
    cS[0].curr_words, cS[0].ncurr = tbl_to_C_2d(curr_words)
  end
  --==================================================
  -- add letters from ppol
  local P = assert(J.pool)
  assert(type(P) == "table")
  assert(#P > 1)
  cS[0].letters, cS[0].nlttr = tbl_to_C_2d(P)

  -- TODO fill in the blanks
  return true
end
return read_state
