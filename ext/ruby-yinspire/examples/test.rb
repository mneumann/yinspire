require 'yinspire'

Yinspire::Simulator.new do |sim|
  sim.load_yin('../../../examples/nets/skorpion.yin')
  sim.load_yin('../../../examples/nets/spiketrains_angle_180.yin')

  sim.each_entity do |e|
    #p e
  end

  while sim.run_for(100.0)
    p sim.current_time
  end
end
