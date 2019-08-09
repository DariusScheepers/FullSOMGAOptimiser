#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include "GAConfigurations.hpp"
#include "Chromosome.hpp"
#include <thread> 
#include <limits>
#include <random>

using namespace std;

class GeneticAlgorithm
{
    private:
        GAConfigurations configurations;
        vector<Chromosome *> chromosomes;

        void initialiseChromosomes();
        void runSOMAndCalculateFitness();
        void calculateFitness(Chromosome *);
        void generateOffSpring();
        vector<Chromosome *> getBestParentsByTournamentSelectionAlgorithm();
        int indexOfBestChromosomeByTournamentSelection(int poolSize);
        Chromosome * removeAndReturnChromosomeAt(int index);
        vector<Chromosome *> createOffspringByUniformCrossover(vector<Chromosome *>);
        void performMutation(vector<Chromosome *> offspring);
        void sortChromosomesFromMostFittestToLowest();
        void removeWeakestChromosomes();
    public:
        GeneticAlgorithm(GAConfigurations configurations);

        void runGeneticAlgorithm();
        Chromosome * returnBestChromsomes();
        ~GeneticAlgorithm();
};

#endif