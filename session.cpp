#include <fstream>
#include <iostream>

#include "session.hpp"
#include "neural_network.hpp" // included so we can set the structure at load_session()

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;

void save_session(const Population &p, const std::vector<int>& s) {
    ofstream out("session.txt", std::ofstream::out);

    out << p.size() << " "
        << p.mutation_rate() << " "
        << p.generation() << '\n';

         for (auto &n : s)
            out << n << " ";
        out << '\n';

         for (auto &nn : p) {
             for (auto &l : nn.layers) {
                 for (auto &n : l.neurons) {
                     for (auto &w : n.weights) {
                         out << w << " ";
                     }
                     out << '\n';
                 }
             }
         }

    out << endl;
    out.close();
}

Population load_session() {
    ifstream in("session.txt", std::ios::in);

    int size;
    in >> size;
    double mutation_rate;
    in >> mutation_rate;
    int generation;
    in >> generation;

    int input_layer;
    in >> input_layer;
    int hidden_layer;
    in >> hidden_layer;
    int output_layer;
    in >> output_layer;

    NeuralNetwork::set_structure({input_layer, hidden_layer, output_layer});
    Population population(size, mutation_rate);
    population.generation(generation);

    for (auto &member : population)
        for (auto &layer : member)
            for (auto &neuron : layer)
                for (auto &weight : neuron)
                    in >> weight;

    in.close();
    return population;
}
