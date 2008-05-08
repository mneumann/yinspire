s = Yinspire_Simulator_new
Yinspire_Simulator_test(s)

rec = Recorder_new

Simulator_set_default_recorder(s, rec)

n1 = Yinspire_Simulator_num_entities(s)
Yinspire_Simulator_load_yin(s, "../../examples/nets/skorpion.yin")
n2 = Yinspire_Simulator_num_entities(s)

Yinspire_Simulator_load_yin(s, "../../examples/nets/spiketrains_angle_180.yin")
x = Yinspire_Simulator_run(s, 10.0)

data = Recorder_get_data(rec)

Recorder_clear(rec)

data2 = Recorder_get_data(rec)


s1 = Yinspire_Simulator_create_entity(s, "Synapse_Default", "s1")
#s3 = Yinspire_Simulator_create_entity(s, "Synapse_Default", "s1")
s2 = Yinspire_NeuralEntity_id(s1)
Yinspire_NeuralEntity_dump(s1)

#ents = Yinspire_Simulator_entity_ids(s)
#e2 = Yinspire_Simulator_get_entity(s, ents{1})

#e2_props = Yinspire_NeuralEntity_dump(e2)

#e2_props.abs_refr_duration = 4.5;

#Yinspire_NeuralEntity_load(e2, e2_props)

#e2_props2 = Yinspire_NeuralEntity_dump(e2)

#e2_id = Yinspire_NeuralEntity_id(e2)

#Yinspire_Simulator_destroy(s)
