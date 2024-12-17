local root = assert(os.getenv("SCRAMBLE_SRC_ROOT"))
return { 
  nP0 = 4,
  nU  = 4,
  port = 8080,
  word_list = root .. "/word_list",
}
