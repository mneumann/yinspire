class SpikeTrains_To_Yin
  def convert(input, out=STDOUT)
    input.read.each_line do |line|
      id, *spikes = line.split
      out.puts "STIMULATE #{id} ! {" 
      spikes.each {|s| out.puts "  #{s}"}
      out.puts "}"
    end
  end
end

if __FILE__ == $0
  SpikeTrains_To_Yin.new.convert(STDIN, STDOUT)
end
