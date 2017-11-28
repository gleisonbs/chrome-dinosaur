#include <algorithm>
#include <random>

#include "population.hpp"

using std::accumulate;
using std::max_element;
using std::random_device;
using std::uniform_int_distribution;
using std::uniform_real_distribution;
using std::vector;

Population::Population(int size) {
    this->size = size;
    for (int i = 0; i < size; i++) {
        this->population.push_back(NeuralNetwork(NeuralNetwork::structure));
    }
}

NeuralNetwork Population::fittest_member() {
    return *max_element(this->population.begin(), this->population.end(),
                        [](NeuralNetwork &d1, NeuralNetwork &d2) { return d1.fitness < d2.fitness; });
}

long Population::fitness_sum() {
    return accumulate(this->population.begin(), this->population.end(), 0, [](int sum, NeuralNetwork &d){ return sum + d.fitness; });
}

NeuralNetwork Population::pick_parent() {
    random_device rd;
    uniform_int_distribution<int> fitness_distribution(0, this->fitness_sum());
    long random_pick = fitness_distribution(rd);
    for (auto &nn : this->population)
        if (random_pick <= nn.fitness)
            return nn;
        else
            random_pick -= nn.fitness;

    return NeuralNetwork();
}

void Population::mutate(NeuralNetwork &nn) {
    random_device rd;
    uniform_real_distribution<double> random_chance(0, 1);
    uniform_real_distribution<double> random_gene(-1, 1);

    for (auto &layer : nn) {
        for (auto &neuron : layer)
            for (auto &weight : neuron)
                if (random_chance(rd) < NeuralNetwork::mutation_rate)
                    weight = random_gene(rd);
    }
}

NeuralNetwork Population::crossover(const NeuralNetwork &parent1, const NeuralNetwork &parent2) {
    NeuralNetwork nn(parent1);

    int number_of_genes = 0;
    for (auto &layer : parent1)
        for (auto &neuron : layer)
            number_of_genes += neuron.weights.size();

    random_device rd;
    uniform_int_distribution<int> random_gene(0, number_of_genes);

    int crossover_gene = random_gene(rd);
    for (size_t l = 0; l < parent2.layers.size(); l++)
        for (size_t n = 0; n < parent2.layers[l].neurons.size(); n++)
            for (size_t w = 0; w < parent2.layers[l].neurons[n].weights.size(); w++)
                if (crossover_gene-- <= 0)
                    nn.layers[l].neurons[n].weights[w] = parent2.layers[l].neurons[n].weights[w];

    return nn;
}

void Population::make_population() {
    vector<NeuralNetwork> descendents;

    NeuralNetwork fittest_nn = this->fittest_member(); // ELITISM
    descendents.push_back(fittest_nn); // ELITISM

    while (descendents.size() < this->size) {
        NeuralNetwork child = this->crossover(this->pick_parent(), this->pick_parent());
        this->mutate(child);
        descendents.push_back(child);
    }

    this->population.clear();
    this->population.shrink_to_fit();
    this->population = descendents;
}
