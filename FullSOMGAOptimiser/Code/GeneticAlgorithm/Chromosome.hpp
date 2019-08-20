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
        vector<float> genes;
        float fitnessValue;
		bool fitnessCalculated;

        SelfOrganisingMap * selfOrganisingMap;

        void intialiseGenes();
    public:
        Chromosome(GAConfigurations *);
        ~Chromosome();

        void setFitnessValue(float);
        float getFitnessValue();
        void setGene(int index, float value);
        vector<float> getGenes();
        float getGene(int index);
        float getGeneRandomValue(size_t index);
		float mutateGene(size_t index);

        void runAlgorithm(SOMConfigurations *);

		float computeRastrigin();

};

#endif