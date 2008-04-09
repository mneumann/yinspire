defines = ['YINSPIRE__EXPENSIVE_RECORD', 'NDEBUG']
env = Environment(CPPPATH='src', CPPDEFINES=defines, CCFLAGS='-O3')

if ARGUMENTS.get('crossmingw', 0):
  env.Tool('crossmingw', toolpath = ['scons-tools'])

sources = Split("""
  src/Core/NeuralEntity.cc
  src/Core/ScheduleEntity.cc
  src/Core/Scheduler.cc
  src/Dumpers/Dumper_Dot.cc
  src/Dumpers/Dumper_Yin.cc
  src/Loaders/Loader_Yin.cc
  src/Loaders/YinParser.cc
  src/Loaders/YinTokenizer.cc
""")

lib = env.Library('yinspire', sources)
env.Program('yinspire', ['src/Main.cc', lib])
