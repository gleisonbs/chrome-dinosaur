#include <random>

#include "neuron.hpp"

using std::random_device;
using std::uniform_real_distribution;

Neuron::Neuron() {}
Neuron::Neuron(const Neuron &neuron) {
    this->weights = neuron.weights;
}

Neuron::Neuron(int number_of_weights) {
    random_device rd;
    uniform_real_distribution<double> random_weight(-1, 1);

    for (int i = 0; i < number_of_weights; i++) {
        this->weights.push_back(random_weight(rd));
    }
}
