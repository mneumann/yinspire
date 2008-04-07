CFLAGS=-c -I. -O3

all: ScheduleEntity.o Scheduler.o NeuralEntity.o Neuron.o Synapse.o \
  YinTokenizer.o YinParser.o Loader_Yin.o Main.o
	${CXX} -o yinspire $>

clean: 
	rm -f *.o yinspire

ScheduleEntity.o: Core/ScheduleEntity.cc
Scheduler.o: Core/Scheduler.cc
NeuralEntity.o: Core/NeuralEntity.cc
Neuron.o: Core/Neuron.cc
Synapse.o: Core/Synapse.cc
Main.o: Main.cc
YinTokenizer.o: Loaders/YinTokenizer.cc
YinParser.o: Loaders/YinParser.cc
Loader_Yin.o: Loaders/Loader_Yin.cc
