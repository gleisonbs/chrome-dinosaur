#include <iostream>
#include <random>
#include <cmath>

#include "neural_network.hpp"

using std::cout;
using std::endl;
using std::ostream;
using std::string;
using std::vector;

vector<int> NeuralNetwork::structure = std::vector<int>();

NeuralNetwork::NeuralNetwork() {}
NeuralNetwork::NeuralNetwork(const NeuralNetwork &nn) {
    this->fitness_ = nn.fitness_;
    this->layers = nn.layers;
}

NeuralNetwork::NeuralNetwork(const vector<int> &structure) {
    for (size_t i = 1; i < structure.size(); i++)
        this->layers.push_back(Layer(structure[i], structure[i-1]));
}

void NeuralNetwork::set_structure(const std::vector<int> &s) {
    NeuralNetwork::structure = s;
}

double NeuralNetwork::sigmoid(double v) {
    return 1/(1+exp(-v));
    // ALTERNATIVE:
    //return v / (1 + abs(v))
}

vector<double> NeuralNetwork::decision(vector<double> inputs) {

    // TODO: PUT THIS IN A LOOP FOR (NUMBER_OF_LAYERS-1)
    inputs.push_back(1); // bias neuron
    vector<double> z;

    for (size_t n = 0; n < this->layers[0].neurons.size(); n++) {
        double sum = 0;
        for (size_t i = 0; i < this->layers[0].neurons[n].weights.size(); i++) {
            sum += inputs[i] * this->layers[0].neurons[n].weights[i];
        }
        z.push_back(this->sigmoid(sum));
    }

    z.push_back(1); // bias neuron
    vector<double> z1;
    for (size_t n = 0; n < this->layers[1].neurons.size(); n++) {
        double sum = 0;
        for (size_t i = 0; i < this->layers[1].neurons[n].weights.size(); i++) {
            sum += z[i] * this->layers[1].neurons[n].weights[i];
        }
        z1.push_back(this->sigmoid(sum));
    }

    return z1;
}

ostream& operator<<(ostream &out, const NeuralNetwork &nn) {
    string msg = "";
    for (size_t l = 0; l < nn.layers.size(); l++) {
        out << "Layer " << l << "\n";
        for (size_t n = 0; n < nn.layers[l].neurons.size(); n++) {
            out << "\tNeuron " << n << ": ";
            for (size_t w = 0; w < nn.layers[l].neurons[n].weights.size(); w++) {
                string weight = std::to_string(nn.layers[l].neurons[n].weights[w]);
                if (weight[0] != '-' ) weight = " " + weight;
                while (weight.size() < 10) weight += '0';
                out << " | " << weight;
            }
            out << '\n';
        }
        out << endl;
    }

    return out;
}
