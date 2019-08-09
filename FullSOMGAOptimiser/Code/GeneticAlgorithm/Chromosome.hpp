#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <vector>
#include "GAConfigurations.hpp"
#include "../SelfOrganisingMap/SelfOrganisingMap.hpp"
#include "../SelfOrganisingMap/SOMConfigurations.hpp"
#include <random>

using namespace std;

class Chromosome
{
    private:
        GAConfigurations configurations;
        vector<float> genes;
        float fitnessValue;
        enum class SelfOrganisingMapParameters;

        SOMConfigurations * somConfig;
        SelfOrganisingMap * selfOrganisingMap;

        void intialiseGenes();
    public:
        Chromosome();
        ~Chromosome();

        void setFitnessValue(float);
        float getFitnessValue();
        void setGene(int index, float value);
        vector<float> getGenes();
        float getGene(int index);
        float getGeneRandomValue(size_t index);

        bool operator>(Chromosome*);
        void runAlgorithm();

};

#endif