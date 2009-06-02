require 'mkmf'

if find_executable('pkg-config')
  $CFLAGS << ' ' + `pkg-config x264 --cflags`.strip
  $LDFLAGS << ' ' + `pkg-config x264 --libs`.strip
end

dir_config("x264")

$CFLAGS << " -W -Wall"

if have_library("x264") and find_header('x264.h') then
 
$objs = %w(x264.o)

create_makefile("x264")

else
  STDERR.puts "missing library x264"
  exit 1
end
