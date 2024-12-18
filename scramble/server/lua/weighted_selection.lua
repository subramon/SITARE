local letters = require 'scramble/server/lua/letters'
local function weighted_selection(A)
  assert(type(A) == "table")
  local rnum = math.random()
  for k, v in ipairs(letters) do 
    if ( rnum < A[v] ) then 
      return v 
    end
  end
end
return  weighted_selection
-- local test 
--[[
local mk_letter_weights = require 'mk_letter_weights'
local C = mk_letter_weights("../../word_list")
for i = 1, 1000 do 
  local x = weighted_selection(C)
  print(i, x) 
end
--]]
