defines = '-DYINSPIRE__EXPENSIVE_RECORD -DNDEBUG'
env = Environment(CPPPATH='src', CCFLAGS='-O3 ' + defines)
sources = Split("""
  src/Main.cc
  src/Core/NeuralEntity.cc
  src/Core/Neuron.cc
  src/Core/ScheduleEntity.cc
  src/Core/Scheduler.cc
  src/Core/Synapse.cc
  src/Dumpers/Dumper_Dot.cc
  src/Dumpers/Dumper_Yin.cc
  src/Loaders/Loader_Yin.cc
  src/Loaders/YinParser.cc
  src/Loaders/YinTokenizer.cc
""")
env.Program('yinspire', sources)
