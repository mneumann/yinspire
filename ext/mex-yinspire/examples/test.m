sim = Simulator_new

Simulator_load_yin(sim, '../../../examples/nets/skorpion.yin')
Simulator_load_yin(sim, '../../../examples/nets/spiketrains_angle_180.yin')

Simulator_create_entity(sim, 'Neuron_SRM01', 'id22')
Simulator_get_entity(sim, 'id22')

Simulator_run(sim, 1000.0)
Simulator_destroy(sim)
