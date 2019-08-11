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
        GAConfigurations * configurations;
        vector<Chromosome *> chromosomes;

		vector<Chromosome *> parentsSelectedForReproducingAtAnIteration;

        void initialiseChromosomes();
		void setAllChromosomesFitness();
        void runExperimentAndCalculateFitnessConcurrently();
		void runExperimentAndCalculateFitnessLinear();
        void calculateFitness(Chromosome *);
        void generateOffSpring();
		int getPoolSize();
        vector<Chromosome *> getBestParentsByTournamentSelectionAlgorithm();
        int indexOfBestChromosomeByTournamentSelection(int poolSize);
        Chromosome * removeAndReturnChromosomeAt(int index);
        vector<Chromosome *> createOffspringByUniformCrossover(vector<Chromosome *>);
		vector<Chromosome *> performMutation(vector<Chromosome *> offspring);
		void placeSelectedReproducingParentsBack();
        void sortChromosomesFromMostFittestToLowest();
        void removeWeakestChromosomes();

		void printCurrentBestChromosome(int);

    public:
        GeneticAlgorithm(GAConfigurations * configurations);

        void runGeneticAlgorithm();
        Chromosome * returnBestChromsomes();
        ~GeneticAlgorithm();
};

#endif