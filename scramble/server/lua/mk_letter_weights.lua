local cutils  = require 'libcutils'
local letters = require  'letters'

local function mk_letter_weights(word_list)
  assert(type(word_list) == "string")
  assert(cutils.isfile(word_list))
  local tmp= {}
    -- read the lines in table 'lines'
  for line in io.lines(word_list) do
    table.insert(tmp, line)
  end
  assert(#tmp > 0)
  -- create letter probabilities
  letter_counts = {}
  for i, w in ipairs(tmp) do
    for k = 1, #w do
      local letter = string.sub(w, k, k)
      if letter_counts[letter] then
        letter_counts[letter] = letter_counts[letter] + 1 
      else 
        letter_counts[letter] = 0
      end
    end
  end
  -- normalize weights 
  local sum = 0
  for k, v in pairs(letter_counts) do 
    sum = sum + v 
  end 
  for k, v in pairs(letter_counts) do 
    letter_counts[k] = letter_counts[k] / sum
  end 
  -- make cumulative counts 
  local cum_counts = {}
  local cum_count = 0
  for k, v in ipairs(letters) do 
    cum_count = cum_count + letter_counts[v]
    cum_counts[v] = cum_count
    prev_v = v
    -- print("cum count of " .. v .. " = " .. cum_count)
  end
  -- START for debugging 
  local num_letters = 0
  for k, v in pairs(letter_counts) do 
    num_letters = num_letters + 1
  end 
  assert(num_letters == 26)
  -- STOP  for debugging 
  --=============================================
  return cum_counts, letter_counts
end
return mk_letter_weights
--[[
local C, L = mk_letter_weights("../../word_list")
-- for k, v in pairs(C) do print(k, v) end 
--]]
