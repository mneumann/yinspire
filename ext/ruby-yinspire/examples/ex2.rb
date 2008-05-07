require 'yinspire'; include Yinspire

class MyRecorder < Recorder
  def record_fire(origin, at, weight)
    puts "#{ weight } ! #{ at }"
  end
end

Simulator.new do |sim|

  # Required to avoid the recorder instance from being garbage
  # collected.
  myrec = sim.register_recorder(MyRecorder.new)

  # Set default recorder
  sim.default_recorder = myrec

  # Create entities
  n1 = sim.create_entity 'Neuron_SRM01', 'n1'
  s1 = sim.create_entity 'Synapse_Default', 's1'

  # Connect them 
  n1.connect(s1)
  s1.connect(n1)

  # Set internal state of neuron
  n1.load :abs_refr_duration => 0.1,
          :const_threshold => 0, 
          :ref_weight => -0.05,
          :tau_m => 1,
          :hebb => false

  # Set internal state of synapse
  s1.load :weight => 0.06,
          :delay => 0.5
  
  # Iterate over each entity in the net printing it's id
  sim.each_entity do |e|
    p e.id
  end

  # Access entity by id 
  n1.id == sim['n1'].id

  # Stimulate n1
  n1.stimulate(0.0, Infinity)

  #
  # Run stimulation, stopping after every 1000.0 ticks. 
  #
  while sim.run_for(1000.0)
    p sim.current_time
  end

end
