FLAGS = -O2 -Wall -Wextra -Werror -std=c++11 -Iinclude
OPENCV = -lopencv_core -lopencv_highgui -lopencv_imgproc

simple: ./bin/simple.o ./bin/reader.o ./bin/image.o ./bin/seam.o
	g++ ./bin/simple.o ./bin/reader.o ./bin/image.o ./bin/seam.o $(OPENCV) -o simple

./bin/simple.o: bin ./src/simple.cpp ./include/basic/image.h ./include/basic/reader.h
	g++ -c -o ./bin/simple.o ./src/simple.cpp $(FLAGS)

./bin/reader.o: bin ./src/basic/reader.cpp ./include/basic/reader.h ./include/basic/image.h
	g++ -c -o ./bin/reader.o ./src/basic/reader.cpp $(FLAGS)

./bin/image.o: bin ./src/basic/image.cpp ./include/basic/image.h ./include/basic/seam.h
	g++ -c -o ./bin/image.o ./src/basic/image.cpp $(FLAGS)

./bin/seam.o: bin ./src/basic/seam.cpp 
	g++ -c -o ./bin/seam.o ./src/basic/seam.cpp $(FLAGS)

clean: 
	rm -rf ./bin
	rm -rf simple

bin:
	mkdir bin -p
