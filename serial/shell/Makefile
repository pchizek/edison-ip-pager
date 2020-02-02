CXX = g++
CXXFLAGS = -Wall -g -static-libstdc++ -m32

seriaSend: main.o 
	$(CXX) $(CXXFLAGS) -o serialSend main.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp
