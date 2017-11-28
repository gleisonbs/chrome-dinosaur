#include "layer.hpp"

Layer::Layer() {}
Layer::Layer(const Layer &layer) {
    this->neurons = layer.neurons;
}
Layer::Layer(int number_of_neurons, int number_of_weights) {
    for (int i = 0; i < number_of_neurons; i++)
        neurons.push_back(Neuron(number_of_weights+1)); // +1 for the bias neuron
}
