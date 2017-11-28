// g++ -std=c++11 main.cpp key_output.cpp screen_reader.cpp dinossaur.cpp population.cpp -o dinossaur -I/usr/include/eigen3/ -lX11 -lXtst `pkg-config gtkmm-2.4 --cflags --libs`
#include <chrono>
#include <iostream>
#include <string>
#include <cstdlib>
#include <thread>
#include <vector>

#include "population.hpp"
#include "screen_reader.hpp"
#include "key_output.hpp"
#include "layer.hpp"

using std::cout;
using std::endl;
using std::stod;
using std::stoi;
using std::vector;

std::chrono::steady_clock::time_point get_time() {
    return std::chrono::steady_clock::now();
}

void sleep_for(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void print_info(int generation, int pop_member, const NeuralNetwork &nn) {
    cout << "Generation: " << generation << endl;
    cout << "Member: " << pop_member << endl;
    cout << nn << endl;
}

int main(int argc, char** argv) {
    std::srand(std::time(0));
    sleep_for(2000);

    int population_size = 4;
    if (argc >= 2) population_size = std::stoi(argv[1]);
    if (argc >= 3) NeuralNetwork::mutation_rate = stod(argv[2]);

    ScreenReader screen(argc, argv);

    // DEFINES NEURAL NETWORK STRUCTURE
    int INPUT_LAYER = 3;
    int HIDDEN_LAYER = 4;
    int OUTPUT_LAYER = 2;

    cout << "Population: " << population_size << endl;
    cout << "Mutation Rate: " <<  NeuralNetwork::mutation_rate*100 << "%" << endl;

    NeuralNetwork::set_structure({INPUT_LAYER, HIDDEN_LAYER, OUTPUT_LAYER});
    Population population(population_size);

    int generation = 1;
    while (true) {
        int pop_member = 0;
        for (auto &nn : population) {
            print_info(generation, ++pop_member, nn);
            Obstacle obstacle; // defined in screen_reader.hpp
            Keyboard::start_new_game();
            auto clock_start = get_time();
            while (true) {
                obstacle = screen.next_obstacle();
                double dinosaur_speed = (std::chrono::duration_cast<std::chrono::milliseconds>(get_time() - clock_start).count())/10000;

                vector<double> r = nn.decision({ obstacle.distance, obstacle.size, dinosaur_speed });

                int jump_power = (r[0] * 200) + 50; // translate the range 0-1(double) to 50-250(integer) (hold jump key for x milliseconds)
                if (r[1] > .5)
                    Keyboard::press("Up", jump_power);
                else
                    Keyboard::press("Down", 50);

                if (screen.is_game_over())
                    break;
            }
            nn.fitness = std::chrono::duration_cast<std::chrono::milliseconds>(get_time() - clock_start).count();
            cout << "Fitness: " << nn.fitness << "\n" << endl;
        }
        cout << "-------------------------------------------------------------\n" << endl;
        Keyboard::reload_page();
        if (generation == 10) break;
        generation++;
        population.make_population();
    }

    return 0;
}
