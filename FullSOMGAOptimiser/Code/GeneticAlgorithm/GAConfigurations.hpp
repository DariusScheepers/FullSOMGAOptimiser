#ifndef GA_CONFIGURATIONS_H
#define GA_CONFIGURATIONS_H

#include <vector>
#include <limits>
#define usint unsigned short int

using namespace std;

class GAConfigurations
{
    private:
        unsigned int chromosomePopulationSize;
        unsigned int iterations;
        unsigned int genesAmount;
        vector<usint[2]> geneValueRanges;
        vector<bool[2]> geneValueRangesInclusiveOrExclusive;
        usint crossoverProbability;
        usint mutationProbability;
        usint mutationPercentage;
        usint selectionCutOffPercentage;
        
        void setExactRangesOnGenes();
    public:
        GAConfigurations(
            unsigned int chromosomePopulationSize,
            unsigned int iterations,
            unsigned int genesAmount,
            vector<usint[2]>geneValueRanges,
            vector<bool[2]>geneValueRangesInclusiveOrExclusive,
            usint crossoverProbability,
            usint mutationProbability,
            usint mutationPercentage
        );

        void setChromosomePopulationSize(unsigned int chromosomePopulationSize);
        void setIterations(unsigned int iterations);
        void setGenesAmount(unsigned int genesAmount);
        void setGeneValueRanges(vector<usint[2]> geneValueRanges);
        void setCrossoverProbability(usint crossoverProbability);
        void setMutationProbability(usint mutationProbability);
        void setMutationPercentage(usint mutationPercentage);
        void setSelectionCutOffPercentage(usint selectionCutOffPercentage);

        unsigned int getChromosomePopulationSize();
        unsigned int getIterations();
        unsigned int getGenesAmount();
        vector<usint[2]> getGeneValueRanges();
        usint getCrossoverProbability();
        usint getMutationProbability();
        usint getMutationPercentage();
        usint getSelectionCutOffPercentage();

        ~GAConfigurations();
};

#endif