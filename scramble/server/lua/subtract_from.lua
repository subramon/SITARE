
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
        break
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
  if ( #ret ~= (n2 - n1) ) then
    print("Pminus = ", table.concat(Pminus, ":"))
    print("P      = ", table.concat(P, ":"))
  end
  assert(#ret == (n2 - n1))
  return ret
end 
return subtract_from
