CXX = g++
OBJS = main.o neural_network.o key_output.o screen_reader.o population.o neuron.o layer.o session.o
FLAGS = -std=c++14 -Wall -Werror -g -lX11 -lXtst
# `pkg-config gtkmm-2.4 --cflags --libs`
all: dinosaur

dinosaur: $(OBJS)
	$(CXX) $(FLAGS) $^ -o $@

key_output.o: key_output.cpp key_output.hpp
	$(CXX) $(FLAGS) -c $^

layer.o: layer.cpp layer.hpp
	$(CXX) $(FLAGS) -c $^

neuron.o: neuron.cpp neuron.hpp
	$(CXX) $(FLAGS) -c $^

screen_reader.o: screen_reader.cpp screen_reader.hpp
	$(CXX) $(FLAGS) -c $^

population.o: population.cpp population.hpp
	$(CXX) $(FLAGS) -c $^

session.o: session.cpp session.hpp
	$(CXX) $(FLAGS) -c $^

neural_network.o: neural_network.cpp neural_network.hpp
		$(CXX) $(FLAGS) -c $^

main.o: main.cpp
	$(CXX) $(FLAGS) -c $^

clean:
	rm *.o *.gch
