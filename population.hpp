#ifndef POPULATION_HPP
#define POPULATION_HPP

// CLASS THAT IMPLEMENTS THE GENETIC ALGORITHM

#include <iostream>
#include <vector>

#include "neural_network.hpp"

class Population {
public:
    Population();
    Population(long, double=0.02);
    Population(const Population&);

    NeuralNetwork fittest_member();
    long fitness_sum();
    void make_population();

    long generation() const { return this->current_generation_; }
    void generation(long g) { this->current_generation_ = g; }

    long best_current_generation() const { return this->best_current_generation_; }
    void best_current_generation(long b) { this->best_current_generation_ = b; }

    long best_overall() const { return this->best_overall_; }
    void best_overall(long b) { this->best_overall_ = b;}

    size_t size() const { return this->size_; }
    double mutation_rate() const { return this->mutation_rate_; }

    void mutate(NeuralNetwork&);
    NeuralNetwork crossover(const NeuralNetwork&, const NeuralNetwork&);

    friend std::ostream& operator<<(std::ostream&, const Population&);

    std::vector<NeuralNetwork>::iterator begin() { return this->population.begin(); }
    std::vector<NeuralNetwork>::iterator end() { return this->population.end(); }
    std::vector<NeuralNetwork>::const_iterator begin() const { return this->population.begin(); }
    std::vector<NeuralNetwork>::const_iterator end() const { return this->population.end(); }

private:
    NeuralNetwork pick_parent();

    size_t size_;
    double mutation_rate_;
    long current_generation_;
    long best_overall_;
    long best_current_generation_;
    std::vector<NeuralNetwork> population;
};

#endif // POPULATION_HPP
