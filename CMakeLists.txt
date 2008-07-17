project (YINSPIRE)

option (YINSPIRE_CROSSCOMPILE_MINGW
  "Cross-compile Yinspire for Windows using mingw" 
  OFF)

option (YINSPIRE_DOUBLE_PRECISION
  "Use double instead of float type" 
  OFF)

if (YINSPIRE_CROSSCOMPILE_MINGW)
  set (CMAKE_SYSTEM_NAME Windows)
  set (CMAKE_C_COMPILER mingw32-gcc)
  set (CMAKE_CXX_COMPILER mingw32-g++)
  set (CMAKE_RANLIB mingw32-ranlib)
endif (YINSPIRE_CROSSCOMPILE_MINGW)

set (SRC_DIR "${CMAKE_SOURCE_DIR}/src/")

set (SRCS
  ${SRC_DIR}/Core/NeuralEntity.cc
  ${SRC_DIR}/Core/ScheduleEntity.cc
  ${SRC_DIR}/Core/Scheduler.cc
  ${SRC_DIR}/Dumpers/Dumper_Dot.cc
  ${SRC_DIR}/Dumpers/Dumper_Yin.cc
  ${SRC_DIR}/Loaders/Loader_Yin.cc
  ${SRC_DIR}/Loaders/YinParser.cc
  ${SRC_DIR}/Loaders/YinTokenizer.cc
  ${SRC_DIR}/Simulator.cc)

if (YINSPIRE_DOUBLE_PRECISION)
  add_definitions(-DYINSPIRE_DOUBLE_PRECISION)
endif (YINSPIRE_DOUBLE_PRECISION)

include_directories (${SRC_DIR})
add_library (yinspirelib STATIC ${SRCS})
add_executable (yinspire ${SRC_DIR}/Main.cc)
target_link_libraries (yinspire yinspirelib)

if (YINSPIRE_CROSSCOMPILE_MINGW)
  add_custom_command (TARGET yinspire POST_BUILD COMMAND mv ARGS 
    yinspire yinspire.exe)
endif (YINSPIRE_CROSSCOMPILE_MINGW)
