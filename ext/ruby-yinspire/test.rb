require 'Yinspire'

include Yinspire

sim = Simulator.new

#sim.each_entity do |i|
#  p i.id
#end
#p sim.get_entity('hallo')
#p sim.get_entity('hallo')
#p sim.get_entity('hallo')
#p sim.get_entity('hallo')
puts "-----------------"
e = sim.create_entity("Neuron_SRM01", "toll")
#e.id = "toll"
#e = sim.create_entity("Synapse_Default", "hallo")
puts "-----------------"

exit
p e.id
p e.type



sim.load_yin('../../examples/nets/skorpion.yin')
sim.load_yin('../../examples/nets/spiketrains_angle_180.yin')
sim.each_entity do |i|
 # p i
 # p i.id
end

e1 = sim.get_entity('e1')
p e1
p e1.id
p e1.type
#p e1
#p e1.id
p sim.get_entity('e1111')

p sim.run(1.0/0.0)
sim.destroy
