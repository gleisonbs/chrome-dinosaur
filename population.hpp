#ifndef POPULATION_HPP
#define POPULATION_HPP

// CLASS THAT IMPLEMENTS THE GENETIC ALGORITHM

#include <vector>

#include "neural_network.hpp"

class Population {
public:
    Population(int);

    NeuralNetwork fittest_member();
    long fitness_sum();
    void make_population();

    void mutate(NeuralNetwork&);
    NeuralNetwork crossover(const NeuralNetwork&, const NeuralNetwork&);

    std::vector<NeuralNetwork>::iterator begin() { return this->population.begin(); }
    std::vector<NeuralNetwork>::iterator end() { return this->population.end(); }
    std::vector<NeuralNetwork>::const_iterator begin() const { return this->population.begin(); }
    std::vector<NeuralNetwork>::const_iterator end() const { return this->population.end(); }

private:
    NeuralNetwork pick_parent();

    size_t size;
    std::vector<NeuralNetwork> population;
};

#endif // POPULATION_HPP
