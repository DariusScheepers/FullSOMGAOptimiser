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
        vector<float> genes;
        float fitnessValue;
		bool fitnessCalculated;

        SelfOrganisingMap * selfOrganisingMap;

        void intialiseGenes();
    public:
        Chromosome(int, GAConfigurations *);
        ~Chromosome();

        void setFitnessValue(float);
        float getFitnessValue();
        void setGene(int index, float value);
        vector<float> getGenes();
        float getGene(int index);
        float getGeneRandomValue(size_t index);
		float mutateGene(size_t index);
		int getIndex();

        void runAlgorithm(SOMConfigurations *);

		float computeSpherical();

};

#endif