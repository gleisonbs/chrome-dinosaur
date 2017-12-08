// g++ -std=c++11 main.cpp key_output.cpp screen_reader.cpp dinossaur.cpp population.cpp -o dinossaur -I/usr/include/eigen3/ -lX11 -lXtst `pkg-config gtkmm-2.4 --cflags --libs`
#include <chrono>
#include <iostream>
#include <string>
#include <cstdlib>
#include <string>
#include <thread>
#include <vector>

#include "population.hpp"
#include "screen_reader.hpp"
#include "key_output.hpp"
#include "layer.hpp"
#include "session.hpp"

using std::cout;
using std::endl;
using std::string;
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

    // DEFINES THE NEURAL NETWORK STRUCTURE THAT WILL COMPOSE THE POPULATION (SEE Population::Population(int, double))
    int INPUT_LAYER = 3;
    int HIDDEN_LAYER = 4;
    int OUTPUT_LAYER = 2;

    NeuralNetwork::set_structure({INPUT_LAYER, HIDDEN_LAYER, OUTPUT_LAYER});
    int population_size = 4;
    double mutation_rate = 0.02;

    for (int i = 1; i < argc; i++) {
        string s(argv[i]);
        if (s == "-m") {
            mutation_rate = stod(argv[i+1]);
        }
        else if (s == "-p") {
            population_size = stoi(argv[i+1]);
        }
    }

    Population population(population_size, mutation_rate);
    for (int i = 1; i < argc; i++) {
        string s(argv[i]);
        if (s == "-c")
            population = load_session();
    }

    sleep_for(2000); // give 2 secods for the user to click the screen
    ScreenReader screen;
    Keyboard keyboard;
    Obstacle obstacle;

    while (true) { // GENERATION LOOP
        int pop_member = 0;
        save_session(population, {INPUT_LAYER, HIDDEN_LAYER, OUTPUT_LAYER});
        for (auto &nn : population) { // POPULATION MEMBER LOOP
            print_info(population.generation(), ++pop_member, nn);
            keyboard.start_new_game();
            std::chrono::steady_clock::time_point clock_start;
            bool first_obstacle_found = false;
            while (true) { // ACTION LOOP
                obstacle = screen.next_obstacle();
                // ONLY START CALCULATING THE FITNESS AFTER ENCOUNTERING THE FIRST OBSTACLE
                if (not first_obstacle_found and obstacle.distance > 0) {
                    first_obstacle_found = true;
                    clock_start = get_time();
                }

                double dinosaur_speed = (std::chrono::duration_cast<std::chrono::milliseconds>(get_time() - clock_start).count())/10000;
                vector<double> r = nn.decision({ obstacle.distance, obstacle.size, dinosaur_speed });

                int jump_power = (r[0] * 200) + 50; // translate the range 0-1(double) to 50-250(integer) (hold jump key for x milliseconds)
                if (r[1] > .5)
                    keyboard.press("Up", jump_power);
                else
                    keyboard.press("Down", 50);

                if (screen.is_game_over())
                    break;
            }
            nn.fitness = std::chrono::duration_cast<std::chrono::milliseconds>(get_time() - clock_start).count();
            cout << "Fitness: " << nn.fitness << "\n" << endl;
        }
        cout << "-> Making new population...\n" << endl;
        keyboard.reload_page();
        // if (population.generation() == 2) break;
        population.make_population();
    }

    return 0;
}
