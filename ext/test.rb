require 'x264'

puts X264.inspect
puts X264::Encoder.inspect

puts X264::Level.inspect
puts X264::Picture.inspect
puts X264::Nal.inspect

encoder = X264::Encoder.new({})

encoder.reconfig {}
