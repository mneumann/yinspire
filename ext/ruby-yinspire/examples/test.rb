require 'yinspire'

include Yinspire

class MyRecorder < Recorder
  def record_fire(origin, at, weight)
    p origin, at, weight
  end
end

Simulator.new do |sim|
  myrec = sim.register(MyRecorder.new)

  sim.load_yin('../../../examples/nets/skorpion.yin')
  sim.load_yin('../../../examples/nets/spiketrains_angle_180.yin')

  sim.each_entity do |e|
    e.recorder = myrec if e.type =~ /Neuron_SRM/ and e.id =~ /Inter/
  end

  while sim.run_for(100.0)
    p sim.current_time
  end
end
