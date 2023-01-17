# mruby-fatfs   [![build](https://github.com/yamori813/mruby-fatfs/actions/workflows/ci.yml/badge.svg)](https://github.com/yamori813/mruby-fatfs/actions/workflows/ci.yml)
FatFs class
## install by mrbgems
- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :github => 'yamori813/mruby-fatfs'
end
```
## example
```ruby
p FatFs.hi
#=> "hi!!"
t = FatFs.new "hello"
p t.hello
#=> "hello"
p t.bye
#=> "hello bye"
```

## License
under the MIT License:
- see LICENSE file
