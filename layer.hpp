#ifndef LAYER_HPP
#define LAYER_HPP

#include <vector>

#include "neuron.hpp"

class Layer {
public:
    Layer();
    Layer(const Layer&);
    Layer(int, int);

    int size() { return this->neurons.size(); }

    std::vector<Neuron> neurons;

    std::vector<Neuron>::iterator begin() { return this->neurons.begin(); }
    std::vector<Neuron>::iterator end() { return this->neurons.end(); }
    std::vector<Neuron>::const_iterator begin() const { return this->neurons.begin(); }
    std::vector<Neuron>::const_iterator end() const { return this->neurons.end(); }
private:
};

#endif // LAYER_HPP
