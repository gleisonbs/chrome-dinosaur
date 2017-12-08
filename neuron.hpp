#ifndef NEURON_HPP
#define NEURON_HPP

#include <vector>

class Neuron {
public:
    Neuron();
    Neuron(const Neuron&);
    Neuron(int);

    int size() { return this->weights.size(); }

    std::vector<double> weights;

    std::vector<double>::iterator begin() { return this->weights.begin(); }
    std::vector<double>::iterator end() { return this->weights.end(); }
    std::vector<double>::const_iterator begin() const { return this->weights.begin(); }
    std::vector<double>::const_iterator end() const { return this->weights.end(); }
};

#endif // NEURON_HPP
