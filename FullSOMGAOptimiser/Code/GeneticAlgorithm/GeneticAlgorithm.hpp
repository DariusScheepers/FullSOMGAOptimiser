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
        vector<double> fitnessHistory;
		unsigned int iteration;
		double standardDeviation;

        void initialiseChromosomes();
		void handleChromosomesInformation();
        void addFitnessValueToFitnessHistory();
        double calculateStandardDeviationForWindow();
		void setAllChromosomesFitness();
        void runExperimentAndCalculateFitnessConcurrently();
		void runExperimentAndCalculateFitnessLinear();
        void calculateFitness(Chromosome *);
        void generateOffSpring();
		int getPoolSize();
        vector<Chromosome *> getBestParentsByTournamentSelectionAlgorithm();
        int indexOfBestChromosomeByTournamentSelection(int poolSize);
        Chromosome * removeAndReturnChromosomeAt(int index);
        vector<Chromosome *> createOffspringByUniformCrossover(int, vector<Chromosome *>);
		vector<Chromosome *> performMutation(vector<Chromosome *> offspring);
        void sortChromosomesFromMostFittestToLowest();
        void removeWeakestChromosomes();
		void deleteChromosomes(vector<Chromosome *>);

		void printCurrentBestChromosome(int);
		Chromosome * findChromosomeByIndex(int);
		void printChromosomeGenesToFile(Chromosome *);
        void printFitnessHistoryToFile();

    public:
        GeneticAlgorithm(GAConfigurations * configurations);

        void runGeneticAlgorithm();
        Chromosome * returnBestChromsomes();
        ~GeneticAlgorithm();
};

#endif