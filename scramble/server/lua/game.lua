local letters = require 'scramble/server/lua/letters'
local JSON    = require 'RSUTILS/lua/JSON'
local is_in   = require 'scramble/server/lua/is_in'

game = {} -- TODO Should this be a global?
game.H = {} --- history (words and creator)
game.P = {} -- pool
game.U = {} -- users (names and scores)

pr = function()
  return JSON:encode(
    {users = game.U, history = game.H, pool = game.P, }
    )
end

init = function (U, nP0)
  print("Initializing game")
  local nU
  if ( U ) then 
    assert(type(U) == "table")
    assert(#U > 0)
    for k1 = 1, #U do 
      local n1 = assert(U[k1].name)
      assert(type(n1) == "string")
      assert(#n1) >= 1)
      for k2 = k1+1, #U do 
        local n2 = assert(U[k2].name)
        assert(n1 ~= n2)
      end
    end
  else
    U = {}
    for k = 1, 4 do 
      U[k] = { name = "user_" .. tostring(i), score = 0, }
    end
  end 
  game.U  = U
  game.nU = #U

  nP0 = nP0 or 4 
  -- nP0 = initial number of letters in pool
  assert(type(nP0) == "number")
  assert(nP0 >= 1)
  for i = 1, nP0 do 
    local r = math.random(1, 26)
    game.P[#game.P+1] = assert(letters[r])
  end 
  --=============================================
  game.H = {}
  game.nH = #H
end

add_to_pool = function()
  local r = math.random(1, 26)
  game.P[#game.P+1] = assert(letters[r])
  return #game.P
end

local function core_make_word(u, Pminus, Wminus, Wplus)
  -- check inputs
  -- check u
  assert(type(u) == "number")
  assert((u >= 1) and ( u < game.nU))
  -- check Pminus
  if ( Pminus) then 
    -- make a copy of P because we will modify it
    local tmp = {}
    local nP = #game.P
    for k, v in ipairs(game.P) do 
      tmp[k] = v
    end
    --=======================
    assert(type(Pminus) == "table")
    for k, v in pairs(Pminus) do 
      assert(type(v) == "string")
      assert(#v == 1)
      local found = false
      for i = 1, nP do
        if ( tmp[i] == v ) then
          found = true
          tmp[i] = nil
        end
      end
      assert(found) 
    end
  end

  -- START: check Wminus
  if ( Wminus ) then
    assert(type(Wminus) == "table")
    for k1, w1 in ipairs(Wminus) do 
      assert(type(w1) == "string")
      -- check that word is in history 
      local found = false
      for 1 = 1, #H do 
        if ( H[i].word == w1) then
          found = true
          break
        end
      end
      assert(found)
      -- check no duplicates
      for k2, w2 in ipairs(Wminus) do 
        if ( k1 ~= k2 ) then assert(w1 ~= w2) end 
      end
    end
  end
  -- STOP : check Wminus
  -- START: check Wplus
  -- check that Wplus contains strings not in history 
  if ( Wplus ) then
    assert(type(Wplus) == "table")
    for k1, w1 in ipairs(Wplus) do 
      assert(type(w1) == "string")
      -- check that word has not been made before
      for 1 = 1, #H do 
        assert(H[i].word ~= w1)
      end
      -- check no duplicates
      for k2, w2 in ipairs(Wplus) do 
        if ( k1 ~= k2 ) then assert(w1 ~= w2) end 
      end
    end
  end
  -- Concatenate all letters in Pminus and in Wminus
  all_str = ""
  if ( Pminus ) then 
    all_str = all_str .. table.concat(Pminus)
  end
  if ( Wminus ) then 
    all_str = all_str .. table.concat(Wminus)
  end
  print("all_str = " .. all_str)
  -- Check that letters in Pminus and in Wminus add up to letters in Wplus
  -- Create canonical form of all_str
  local tmp = {}
  if ( Pminus ) then 
    for k1, v1 in pairs(Pminus) do 
      if ( 
  -- STOP : check Wplus
  return true
end

function make_word(str)
  print("Lua to make word " .. str)
  assert(type(str) == "string")
  local J = JSON:decode(str)
  assert(type(J) == "table")
  local user   = assert(J.user)
  local Pminus = J.Pminus
  local Wminus = J.Wminus
  local Wplus = J.Wplus
  assert(core_make_word(user, Pminus, Wminus, Wplus))
  return true
end

