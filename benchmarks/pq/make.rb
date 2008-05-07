require 'time'

COMPILER='g++'
COMPILER_NAME = `#{COMPILER} --version`.split("\n").first.gsub(/\s/, '_')
COMPILER_OPTFLAGS = "-O3"
COMPILE_DATE = Time.now.iso8601  
UNAME = `uname -p -r -s`.split("\n").first.gsub(/\s/, '_')

# dev.cpu.0.freq 
CPUFREQ = `sysctl machdep.tsc_freq`.split(": ").last.chomp

ROOT = File.join(Dir.pwd, '../..')

CFLAGS =  %{ -DNDEBUG -no-integrated-cpp -B #{ROOT}/tools -Winline -Wall}
CFLAGS << %{ -I/usr/local/include -I#{ROOT}/src}
CFLAGS << %{ #{COMPILER_OPTFLAGS}}
CFLAGS << %{ -D_COMPILER_OPTFLAGS_='"#{COMPILER_OPTFLAGS}"'}
CFLAGS << %{ -D_COMPILER_NAME_='"#{COMPILER_NAME}"'}
CFLAGS << %{ -D_COMPILE_DATE_='"#{COMPILE_DATE}"'}
CFLAGS << %{ -D_UNAME_='"#{UNAME}"'}
CFLAGS << %{ -D_CPUFREQ_='"#{CPUFREQ}"'}

command = %{#{COMPILER} #{CFLAGS} Bench.cc -o bench} 
system command
