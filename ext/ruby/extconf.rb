require 'mkmf'

ROOT_DIR = File.expand_path(File.join(File.dirname(__FILE__), "..", ".."))

($CFLAGS ||= "") << " -I#{ROOT_DIR}/src"
($LDFLAGS ||= "") << " -L#{ROOT_DIR}/build/Release -lyinspirelib -lstdc++"

create_makefile('Yinspire')
