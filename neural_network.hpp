#ifndef NEURAL_NETWORK_HPP
#define NEURAL_NETWORK_HPP

#include <iostream>
#include <vector>

#include "layer.hpp"

class NeuralNetwork {
public:
    NeuralNetwork();
    NeuralNetwork(const NeuralNetwork&);
    NeuralNetwork(const std::vector<int>&);

    std::vector<double> decision(std::vector<double>);
    int size() { return this->layers.size(); }

    static void set_structure(const std::vector<int>&);

    long fitness_ = 0;
    std::vector<Layer> layers;

    static std::vector<int> structure;

    friend std::ostream& operator<<(std::ostream&, const NeuralNetwork&);

    std::vector<Layer>::iterator begin() { return this->layers.begin(); }
    std::vector<Layer>::iterator end() { return this->layers.end(); }
    std::vector<Layer>::const_iterator begin() const { return this->layers.begin(); }
    std::vector<Layer>::const_iterator end() const { return this->layers.end(); }

private:
    double sigmoid(double);
};

#endif // NEURAL_NETWORK_HPP
