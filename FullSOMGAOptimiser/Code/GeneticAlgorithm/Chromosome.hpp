#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <vector>
#include "GAConfigurations.hpp"
#include "../SelfOrganisingMap/SelfOrganisingMap.hpp"
#include "../SelfOrganisingMap/SOMConfigurations.hpp"
#include <random>
#include <ctime>

#define _USE_MATH_DEFINES

#include <cmath>

using namespace std;

class Chromosome
{
    private:
        GAConfigurations * configurations;
		int index;
        vector<double> genes;
        double fitnessValue;
		bool fitnessCalculated;

        SelfOrganisingMap * selfOrganisingMap;

        void intialiseGenes();
    public:
        Chromosome(int, GAConfigurations *);
        ~Chromosome();

        void setFitnessValue(double);
        double getFitnessValue();
        void setGene(int index, double value);
        vector<double> getGenes();
        double getGene(int index);
        double getGeneRandomValue(size_t index);
		double mutateGene(size_t index);
		int getIndex();

        void runAlgorithm(SOMConfigurations *);

		double computeSpherical();

};

#endif