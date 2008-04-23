s = Yinspire_Simulator_new
Yinspire_Simulator_test(s)

n1 = Yinspire_Simulator_num_entities(s)
Yinspire_Simulator_load_yin(s, "../examples/nets/skorpion.yin")
n2 = Yinspire_Simulator_num_entities(s)

Yinspire_Simulator_load_yin(s, "../examples/nets/spiketrains_angle_180.yin")
x = Yinspire_Simulator_run(s, 100.0)

ents = Yinspire_Simulator_entity_ids(s)
e2 = Yinspire_Simulator_get_entity_by_id(s, ents{1})

e2_props = Yinspire_NeuralEntity_dump(e2)

e2_props.abs_refr_duration = 4.5;

Yinspire_NeuralEntity_load(e2, e2_props)

e2_props2 = Yinspire_NeuralEntity_dump(e2)

e2_id = Yinspire_NeuralEntity_get_id(e2)

Yinspire_Simulator_delete(s)
