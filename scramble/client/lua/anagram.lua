local function anagram(can_str)
  assert(type(can_str_to_anagram) == "table") -- global
  assert(type(can_str) == "string")
  local W = can_str_to_anagram[can_str]
  if ( W == nil ) then
    return {}
  else
    return W
  end
end
return  anagram
