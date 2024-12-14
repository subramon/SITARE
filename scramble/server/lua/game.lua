local function is_in (x, X)
  assert(x)
  assert(type(X) == "table")
  for _, v in pairs(X) do 
    if ( v == x ) then return true
    end
  end
  return false
end
local letters = require 'scramble/server/lua/letters'
local JSON    = require 'RSUTILS/lua/JSON'

foo = function()
  print("hello world")
  return "hello world from Lua"
end
game = {} -- TODO Should this be a global?
game.H = {} --- history
game.W = {} -- words made by users 
game.S = {} -- scores of users
game.P = {} -- pool

pr = function()
  return JSON:encode(
    {users = game.U, 
    history = game.H, 
    pool = game.P, 
    scores = game.S})
end

init = function (nU, nP0)
  print("Initializing game")
  nU = nU or 4 
  nP0 = nP0 or 4 
  game.nU = nU
  -- nU = number of users
  assert(type(nU) == "number")
  assert(nU >= 1)
  -- nP0 = initial number of letters in pool
  assert(type(nP0) == "number")
  assert(nP0 >= 1)
  game.nH = {}
  for i = 1, nU do 
    game.W[i] = {} -- none of the user own any words
    game.S[i] = 0 -- none of the user have scored any points
  end
  for i = 1, nP0 do 
    local r = math.random(1, 26)
    game.P[#game.P+1] = assert(letters[r])
  end 
  return H, U, S, P 
end

add_to_pool = function()
  local r = math.random(1, 26)
  game.P[#game.P+1] = assert(letters[r])
  return #game.P
end

game.make_word = function(u, Pminus, Wminus, Wplus)
  -- check inputs
  -- check u
  assert(type(u) == "number")
  assert((u >= 1) && ( u < game.nU))
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
      for 1 = 1, nP do
        if ( tmp[i] == v ) then
          found = true
          tmp[i] = nil
        end
      end
      assert(found) 
    end
  end

  -- START: check Wminus
  -- STOP : check Wminus
  -- START: check Wplus
  -- check that Wplus contains strings not in history 
  if ( Wplus ) then
    assert(type(Wplus) == "table")
    for k, w in ipairs(Wplus) do 
      assert(type(w) == "string")
      assert(not is_in(w, game.H)) 
    end
  end
  -- Check that letters in Pminus and in Wminus add up to letters in Wplus
  -- STOP : check Wplus
end

