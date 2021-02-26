CXX = g++
CXXFLAGS = -IC:/SFML/include -LC:/SFML/lib -DSFML_STATIC -IC:/boost_1_72_0 -LC:/boost_1_72_0/stage/lib
LIBS = -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lfreetype -lopengl32 -lwinmm -lgdi32 -Wl,-Bstatic -lstdc++ -lpthread

all: output

output: main.o readdata.o classes.o
	$(CXX) $(CXXFLAGS) main.o readdata.o classes.o -o build/readstats  $(LIBS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp  $(LIBS)
	
readdata.o: readdata.cpp readdata.h
	$(CXX) -c readdata.cpp $(LIBS)

classes.o: classes.cpp classes.h
	$(CXX) -c classes.cpp $(LIBS)
