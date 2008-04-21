#
# Constructs N nets that can be simulated at least until MAX_TIME.
#
# Usage:
#
#   find_nets.rb N MAX_TIME
#

N = Integer(ARGV.shift || raise)
MAX_TIME = Integer(ARGV.shift || raise)

Dir.mkdir "nets"

i = 1
while i <= N
  p i

  `ruby gereon2005.rb nets/tmp.net nets/tmp.spikes`
  `ruby ../../tools/Netfile_to_Yin.rb < nets/tmp.net > nets/tmp.yin`
  `ruby ../../tools/SpikeTrains_To_Yin.rb < nets/tmp.spikes >> nets/tmp.yin`
  `../../build/Release/yinspire --stop-at #{MAX_TIME} --record nets/tmp.out nets/tmp.yin`
  id, time = `tail -n 1 nets/tmp.out`.split("!")

  if (MAX_TIME - time.to_f).abs < 2.0
    `mv nets/tmp.net nets/#{i}.net`
    `mv nets/tmp.spikes nets/#{i}.spikes`
    `mv nets/tmp.yin nets/#{i}.yin`
    i += 1
  end
end

`rm -f nets/tmp.*`
