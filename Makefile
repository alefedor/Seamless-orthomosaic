FLAGS = -O3 -Wall -Wextra -Werror -std=c++11 -Iinclude
OPENCV = -lopencv_core -lopencv_highgui -lopencv_imgproc

orthomosaic: ./bin/orthomosaic.o ./bin/reader.o ./bin/image.o ./bin/seam.o ./bin/best_neighbour.o ./bin/visualizer.o
	g++ ./bin/orthomosaic.o ./bin/reader.o ./bin/image.o ./bin/seam.o ./bin/best_neighbour.o  ./bin/visualizer.o $(OPENCV) -o orthomosaic

./bin/orthomosaic.o: bin ./src/orthomosaic.cpp ./include/basic/image.h ./include/basic/reader.h
	g++ -c -o ./bin/orthomosaic.o ./src/orthomosaic.cpp $(FLAGS)

./bin/reader.o: bin ./src/basic/reader.cpp ./include/basic/reader.h ./include/basic/image.h
	g++ -c -o ./bin/reader.o ./src/basic/reader.cpp $(FLAGS)

./bin/image.o: bin ./src/basic/image.cpp ./include/basic/image.h ./include/basic/seam.h
	g++ -c -o ./bin/image.o ./src/basic/image.cpp $(FLAGS)

./bin/seam.o: bin ./src/basic/seam.cpp ./include/basic/seam.h 
	g++ -c -o ./bin/seam.o ./src/basic/seam.cpp $(FLAGS)
	
./bin/best_neighbour.o: bin ./src/solutions/best_neighbour.cpp ./include/solutions/best_neighbour.h ./include/basic/image.h
	g++ -c -o ./bin/best_neighbour.o ./src/solutions/best_neighbour.cpp $(FLAGS)

./bin/visualizer.o: bin ./src/visual/visualizer.cpp ./include/visual/visualizer.h ./include/basic/image.h
	g++ -c -o ./bin/visualizer.o ./src/visual/visualizer.cpp $(FLAGS)

clean: 
	rm -rf ./bin
	rm -rf orthomosaic

bin:
	mkdir bin -p
