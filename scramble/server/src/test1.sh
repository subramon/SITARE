# add 3 letters
curl -s --url "localhost:8080/AddLetter" | jq .
curl -s --url "localhost:8080/AddLetter" | jq .
curl -s --url "localhost:8080/AddLetter" | jq .
# make word crust 
cmd='{"User":1,"Wplus":[ "crust" ],"Pminus": [ "c", "r", "u", "s", "t"]}'
enc=`echo $cmd | jq -rR @uri`
curl --url "localhost:8080/MakeWord?${enc}"  | jq .
# get state
curl --url "localhost:8080/GetState" | jq .
# echo PREMATURE; exit 1; 
# make word rustic
cmd='{"User":2,"Wplus":[ "rustic" ],"Pminus": [ "i" ], "Wminus" : [ "crust" ] }'
enc=`echo $cmd | jq -rR @uri`
curl --url "localhost:8080/MakeWord?${enc}"
# get state
curl --url "localhost:8080/GetState" | jq .
#-- make word plot
cmd='{"User":3,"Wplus":[ "plot" ],"Pminus": [ "p", "l", "o", "t" ] }'
enc=`echo $cmd | jq -rR @uri`
echo $cmd
echo $enc
curl --url "localhost:8080/MakeWord?${enc}"
# get state
curl --url "localhost:8080/GetState" | jq .
