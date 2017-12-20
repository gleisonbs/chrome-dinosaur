// g++ -std=c++11 main.cpp key_output.cpp screen_reader.cpp dinossaur.cpp population.cpp -o dinossaur -I/usr/include/eigen3/ -lX11 -lXtst `pkg-config gtkmm-2.4 --cflags --libs`
#include <chrono>
#include <iostream>
#include <string>
#include <cstdlib>
#include <signal.h>
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

// DEFINES THE NEURAL NETWORK STRUCTURE THAT WILL COMPOSE THE POPULATION (SEE Population::Population(int, double))
constexpr int INPUT_LAYER = 3;
constexpr int HIDDEN_LAYER = 6;
constexpr int OUTPUT_LAYER = 2;

Population population;

void exitHandler(int dummy) {
    save_session(population, {INPUT_LAYER, HIDDEN_LAYER, OUTPUT_LAYER});
    cout << "\nExiting" << endl;
    exit(0);
}

std::chrono::steady_clock::time_point get_time() {
    return std::chrono::steady_clock::now();
}

void sleep_for(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void print_info(const Population &p, int pop_member, const NeuralNetwork &nn) {
    cout << "Generation: " << p.generation() << endl;
    cout << "Member: " << pop_member << endl;
    cout << "Best of this generation: " << p.best_current_generation() << endl;
    cout << "Best overall: " << p.best_overall() << endl;
    cout << nn << endl;
}

int main(int argc, char** argv) {
    std::srand(std::time(0));
    signal(SIGINT, exitHandler);

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

    population = Population(population_size, mutation_rate);
    for (int i = 1; i < argc; i++) {
        string s(argv[i]);
        if (s == "-c") {
            string session_name(argv[i+1]);
            population = load_session(session_name);
        }
    }

    sleep_for(2000); // give 2 secods for the user to click the screen
    ScreenReader screen;
    Keyboard keyboard;
    Obstacle obstacle;

    while (true) { // GENERATION LOOP
        int pop_member = 0;
        // save the session every twenty generations
        if (population.generation() % 20 == 0) save_session(population, {INPUT_LAYER, HIDDEN_LAYER, OUTPUT_LAYER});
        for (auto &nn : population) { // POPULATION MEMBER LOOP
            print_info(population, ++pop_member, nn);
            keyboard.start_new_game();
            std::chrono::steady_clock::time_point clock_speed;
            std::chrono::steady_clock::time_point clock_start;
            bool first_obstacle_found = false;
            clock_speed = get_time();
            while (true) { // ACTION LOOP
                obstacle = screen.next_obstacle();
                // ONLY START CALCULATING THE FITNESS AFTER ENCOUNTERING THE FIRST OBSTACLE
                if (not first_obstacle_found and obstacle.distance > 0) {
                    first_obstacle_found = true;
                    clock_start = get_time();
                }

                double dinosaur_speed = std::chrono::duration_cast<std::chrono::milliseconds>(get_time() - clock_speed).count();
                dinosaur_speed /= 100000;
                vector<double> r = nn.decision({ obstacle.distance, obstacle.size, dinosaur_speed });

                // cout << "Distance: " << obstacle.distance << "\n";
                // cout << "Size: " << obstacle.size << "\n";
                // cout << "Speed: " << dinosaur_speed << "\n";

                int jump_power = (r[0] * 200) + 50; // translate the range 0-1(double) to 50-250(integer) (hold jump key for x milliseconds)
                if (r[1] > .55)
                    keyboard.press("Up", jump_power);
                else if (r[1] < .45)
                    keyboard.press("Down", 25);

                if (screen.is_game_over())
                    break;
            }
            nn.fitness_ = std::chrono::duration_cast<std::chrono::milliseconds>(get_time() - clock_start).count();
            cout << "Fitness: " << nn.fitness_ << "\n" << endl;
        }
        cout << "-> Making new population...\n" << endl;
        keyboard.reload_page();
        // if (population.generation() == 2) break;
        population.make_population();
    }

    return 0;
}
