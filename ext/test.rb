require 'x264'

encoder = X264::Encoder.config do|e|
  puts (e.public_methods - Object.public_methods).inspect
  e.rc_bitrate = 300
  e.rc_method = X264::RC_CQP
end

#encoder.encode_frame_from_yuv(yuv)

puts "done"
