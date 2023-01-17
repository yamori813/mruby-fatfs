##
## FatFs Test
##

assert("FatFs#hello") do
  t = FatFs.new "hello"
  assert_equal("hello", t.hello)
end

assert("FatFs#bye") do
  t = FatFs.new "hello"
  assert_equal("hello bye", t.bye)
end

assert("FatFs.hi") do
  assert_equal("hi!!", FatFs.hi)
end
