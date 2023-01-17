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
f = FatFs.new "fat.img"
f.dir "0:"
```

## License
under the MIT License:
- see LICENSE file
