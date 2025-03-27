import scripal

scripal.libInit("", scripal.ENC_UTF8)
scripal.logInit("stdout")

print(scripal.getVersion())

obj = scripal.TFScripal("source", "match find(bow 'a'); ifMatch matchEnd find(eow); loop;")

result = obj.match("Find the words apple, ape and award.")

print ("match result: ", result , " , no. of results: " , obj.getResultSize())
for i in range(0, obj.getResultSize()):
  print(obj.getResultStr(i))


