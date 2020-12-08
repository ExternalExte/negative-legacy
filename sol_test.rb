#!/usr/bin/env ruby
print `docker run -it --rm -v "$PWD":/usr/src -w /usr/src ruby:2.0.0 ruby #{ARGV[0]}`
