#
# Generates a net as described in chapter 5
# "Geschwindigkeitsbetrachtungen" of [Gereon Weiss 2005].
#
# The net is print to STDOUT, while the spikes are print to STDERR,
# unless filenames are given as parameters.
#
# [Gereon Weiss 2005]: Study thesis - Ein geschwindigkeitsoptimierter
# Simulator fuer gepulste Neuronale Netze.
#

NET = if a = ARGV.shift then File.open(a, 'w+') else STDOUT end
SPIKES = if a = ARGV.shift then File.open(a, 'w+') else STDERR end

NET.puts "NEURONTYPE Input 20 -0.05 30 3 1" 
NET.puts "NEURONTYPE Type1 20 -0.05 30 3 1"
NET.puts "NEURONTYPE Type2 30 -0.05 30 3 1"
NET.puts "SYNAPSETYPE Syn1 0.06 1.5"
NET.puts "SYNAPSETYPE Syn2 0.09 0.8"
NET.puts "SYNAPSETYPE Syn3 0.03 0.8"
NET.puts "SYNAPSETYPE Syn4 0.07 0.8"

SYNAPSE_CONN = {
  :Input => {
    :Input => :Syn1,
    :Type1 => :Syn1,
    :Type2 => :Syn2 },
  :Type1 => {
    :Input => :Syn1,
    :Type1 => :Syn1,
    :Type2 => :Syn2 },
  :Type2 => {
    :Input => :Syn3,
    :Type1 => :Syn3,
    :Type2 => :Syn4 }
}

neurons = Array.new(1000)

100.times do |i|
  j = i + 0
  NET.puts "NEURONS n_#{j} #{ neurons[j] = :Input }" 
end

700.times do |i|
  j = i + 100
  NET.puts "NEURONS n_#{j} #{ neurons[j] = :Type1 }" 
end

200.times do |i|
  j = i + 100 + 700
  NET.puts "NEURONS n_#{j} #{ neurons[j] = :Type2 }" 
end

# synapses
syn = 0
1000.times do |from|
  100.times do |j|
    to = rand(1000)
    NET.puts "CONNECT n_#{from} TO n_#{to} SYNAPSES #{SYNAPSE_CONN[neurons[from]][neurons[to]]}"
  end
end

#
# Input neurons get stimuli at 48 Hz for 10 seconds
# Note that inspire uses milliseconds, i.e. 10 seconds == 10_000
# Yinspire "ticks".
#
100.times do |i|
  arr = (0 ... (10*48)).map { rand() * 10_000 }.sort.map {|f| "%.5f" % f}
  SPIKES.puts "n_#{i} #{arr.join(' ')}"
end
