CXX     = g++
DEBUG   = -g

#CXXFLAGS  = -Llibs/ -Wl,-rpath=libs/ $(DEBUG) -std=c++11
CXXFLAGS  = $(DEBUG) -std=c++11
#LIBS = -lftd2xx -lusb-1.0 -lpthread -ldl
LIBS = -lpthread -ldl

all:
	#$(CXX) $(CXXFLAGS) main.cpp RNGesturesTask.cpp RNUtils.cpp SerialPort.cpp DorisLipSync.cpp TextToSpeech.cpp RNTourDialog.cpp -o tourSpeech $(LIBS)
	$(CXX) $(CXXFLAGS) main.cpp  RNUtils.cpp RNTourDialog.cpp -o tourSpeech $(LIBS)
clear:
	rm *.o tourSpeech