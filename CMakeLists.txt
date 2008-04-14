project (YINSPIRE)

option (YINSPIRE_EXPENSIVE_RECORD
  "Compiles in calls to record every stimulation/processing of a NeuralEntity"
  OFF)

option (YINSPIRE_CROSSCOMPILE_MINGW
  "Cross-compile Yinspire for Windows using mingw" 
  OFF)

if (YINSPIRE_EXPENSIVE_RECORD)
  add_definitions (-DYINSPIRE__EXPENSIVE_RECORD)
endif (YINSPIRE_EXPENSIVE_RECORD)

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
  ${SRC_DIR}/Loaders/YinTokenizer.cc)
 
include_directories (${SRC_DIR})
add_library (yinspirelib STATIC ${SRCS})
add_executable (yinspire ${SRC_DIR}/Main.cc)
target_link_libraries (yinspire yinspirelib)

if (YINSPIRE_CROSSCOMPILE_MINGW)
  add_custom_command (TARGET yinspire POST_BUILD COMMAND mv ARGS 
    yinspire yinspire.exe)
endif (YINSPIRE_CROSSCOMPILE_MINGW)