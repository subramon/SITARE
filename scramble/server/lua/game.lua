local cutils  = require 'libcutils'
local letters = require 'scramble/server/lua/letters'
local JSON    = require 'RSUTILS/lua/JSON'
local is_in   = require 'scramble/server/lua/is_in'
local mk_letter_weights  = require 'scramble/server/lua/mk_letter_weights'
local weighted_selection = require 'scramble/server/lua/weighted_selection'

local function cat_with_nulls(X, nX)
  local str = ""
  for i = 1, nX do 
    if ( X[i] ) then str = str .. X[i] end
  end
  return str
end

local function subtract_from(Pminus, P) 
  assert(type(Pminus) == "table")
  assert(type(P) == "table")
  local n1 = #Pminus
  local n2 = #P
  local nP = #P
  local tmp = {}
  for k, v in ipairs(P) do 
    tmp[k] = v
  end
  for k, v in ipairs(Pminus) do 
    local found = false
    for i = 1, nP do
      if ( tmp[i] == v ) then
        found = true
        local Pstr = cat_with_nulls(tmp, nP)
        -- print("Deleting from pool:  " .. v .. " in " .. Pstr)
        tmp[i] = nil
      end
    end
    if ( not found ) then 
      local Pstr = cat_with_nulls(tmp, nP)
      print("Did not find " .. v .. " in " .. Pstr)
    end 
    assert(found) 
  end
  local ret = {}
  for k, v in pairs(tmp) do 
    if ( tmp[k] ) then 
      ret[#ret+1] = v
    end
  end
  assert(#ret == (n2 - n1))
  return ret
end 
--============================================================
game = {} -- TODO Should this be a global?
game.H = {} --- history (of which words were made, how and by whom)
game.P = {} -- pool
game.U = {} -- users (names and scores)
game.W = {} -- words currently in play 
game.D = {} -- dictionary of acceptable words 
game.C = 1 -- clock 
aame.lw = {} -- letter weights

--=============================================================
local function check_Wminus(Wminus) -- check Wminus
  if ( not Wminus ) then return true end  -- degenerate case
  assert(type(Wminus) == "table")
  for k1, w1 in ipairs(Wminus) do 
    assert(type(w1) == "string")
    -- check that word is in play 
    local found = false
    for i = 1, #game.W do 
      if ( game.W[i] == w1) then
        found = true
        break
      end
    end
    assert(found)
    -- check that word is not in history
    local found = false
    for i = 1, #game.H do 
      if ( game.H[i].word == w1) then
        found = true
        break
      end
    end
    assert(not found)
    -- check no duplicates
    for k2, w2 in ipairs(Wminus) do 
      if ( k1 ~= k2 ) then assert(w1 ~= w2) end 
    end
  end
  return true
end
--=============================================================
local function check_Pminus(Pminus) -- check Pminus
  if ( not Pminus) then  return true end -- degenerate case
  --=======================
  assert(type(Pminus) == "table")
  for k, v in pairs(Pminus) do 
    assert(type(v) == "string")
    assert(#v == 1)
  end
  local newP = assert(subtract_from(Pminus, game.P))
  return true
end
--=============================================================
  -- check that Wplus contains words not created before 
local function check_Wplus(Wplus) 
  assert(type(Wplus) == "table")
  for k1, w1 in ipairs(Wplus) do 
    assert(type(w1) == "string")
    -- check that word exists in dictionary 
    assert(game.D[w1])
    -- check that word has not been made before
    for i = 1, #game.H do 
      assert(game.H[i].word ~= w1)
    end
    -- check that word is not in play (argue why this is not needed)
    for i = 1, #game.W do 
      assert(game.W[i] ~= w1)
    end
    -- check no duplicates
    for k2, w2 in ipairs(Wplus) do 
      if ( k1 ~= k2 ) then assert(w1 ~= w2) end 
    end
  end
  return true
end
--=============================================================
get_state = function()
  return JSON:encode(
    {
      clock = game.C,
      history = game.H, 
      pool = game.P, 
      users = game.U, 
      words = game.W}
    )
end

init = function (nP0, nU, word_list)
  print("Initializing game")
  -- nU =  number of users
  nU = nU or 4 
  assert(type(nU) == "number")
  assert(nU >= 1)
  local U = {}
  for k = 1, nU do 
    U[k] = { name = "user_" .. tostring(k), score = 0, }
  end 
  game.U  = U
  game.nU = #U

  -- nP0 = minimum number of letters in pool
  nP0 = nP0 or 4 
  assert(type(nP0) == "number")
  assert(nP0 >= 1)
  local P = {}
  for i = 1, nP0 do 
    local r = math.random(1, 26)
    P[#P+1] = assert(letters[r])
  end 
  game.P = P
  game.nP0 = nP0
  --=============================================
  assert(type(word_list) == "string")
  assert(cutils.isfile(word_list))
  local tmp= {}
    -- read the lines in table 'lines'
  for line in io.lines(word_list) do
    table.insert(tmp, line)
  end
  assert(#tmp > 0)
  game.D = {}
  for i, v in ipairs(tmp) do 
    assert(not game.D[v], "duplicate = " .. v) -- checks for no duplicates
    game.D[v] = true
  end
  --=============================================
  game.lw = mk_letter_weights(word_list) 
  assert(type(game.lw) == "table")
  --=============================================
  return true
end

add_letter = function()
  local version = 1
  local to_add
  if ( version == 0 ) then 
    local r = math.random(1, 26)
    to_add = assert(letters[r])
  elseif ( version == 1 ) then 
    to_add = weighted_selection(game.lw)
  else 
    error("Unknown version in add_letter()")
  end
  game.P[#game.P+1] = to_add
  return #game.P
end

local function core_make_word(u, Pminus, Wminus, Wplus)
  -- check inputs
  -- check u
  assert(type(u) == "number")
  assert((u >= 1) and ( u < game.nU))
  assert(check_Pminus(Pminus))
  assert(check_Wminus(Wminus))
  assert(check_Wplus(Wplus))
  -- Concatenate all letters in Pminus and in Wminus
  local all_str_minus = ""
  if ( Pminus ) then 
    all_str_minus = all_str_minus .. table.concat(Pminus)
  end
  if ( Wminus ) then 
    all_str_minus = all_str_minus .. table.concat(Wminus)
  end
  assert(#all_str_minus > 0)
  local all_str_plus = table.concat(Wplus) -- cat all letters in Wplus 
  assert(#all_str_plus == #all_str_minus)
  -- Check that letters in Pminus and in Wminus add up to letters in Wplus
  -- Create canonical form of all_str
  -- P := P - Pminus
  if ( Pminus ) then 
    game.P = subtract_from(Pminus, game.P)
    while ( #game.P < game.nP0 ) do 
      add_letter()
    end
  end 
  -- W := W - Wminus
  if ( Wminus ) then 
    game.W = subtract_from(Wminus, game.W)
  end
  -- W := W + Wplus
  for k, v in ipairs(Wplus) do 
    game.W[#game.W+1] = v
  end
  -- calculate score for this play 
  local s = 10 
  -- increment user score 
  game.U[u].score = game.U[u].score + s 
  -- record this move
  game.H[#game.H+1] = {
    user = u, Pminus = Pminus, Wminus = Wminus, Wplus = Wplus, score = s }
  -- increment clock 
  game.C = game.C + 1  
  return true
end

function make_word(str)
  assert(type(str) == "string")
  local J = JSON:decode(str)
  assert(type(J) == "table")
  local user   = assert(J.User)
  assert((user >= 1) and (user <= game.nU))
  local Pminus = J.Pminus
  local Wminus = J.Wminus
  local Wplus = J.Wplus
  assert(type(Wplus) == "table")
  local n_plus = 0
  for k, v in pairs(Wplus) do 
    assert(type(v) == "string"); n_plus = n_plus + 1 
  end 
  assert(n_plus > 0)
  local status = core_make_word(user, Pminus, Wminus, Wplus)
  return true
end

