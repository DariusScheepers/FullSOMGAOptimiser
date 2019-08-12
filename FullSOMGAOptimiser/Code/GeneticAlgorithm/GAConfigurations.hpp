#ifndef GA_CONFIGURATIONS_H
#define GA_CONFIGURATIONS_H

#include <vector>
#include <limits>
#include "../SelfOrganisingMap/SOMConfigurations.hpp"
#define usint unsigned short int

using namespace std;

class GAConfigurations
{
    private:
        unsigned int chromosomePopulationSize;
        unsigned int iterations;
        unsigned int genesAmount;
        vector<vector<float>> geneValueRanges;
        vector<vector<bool>> geneValueRangesInclusiveOrExclusive;
        usint crossoverProbability;
        usint mutationProbability;
        usint selectionCutOffPercentage;
		SOMConfigurations * targetExperimentConfigurations;
        
        void setExactRangesOnGenes();
    public:
		GAConfigurations(
			unsigned int chromosomePopulationSize,
			unsigned int iterations,
			unsigned int genesAmount,
			vector<vector<float>>geneValueRanges,
			vector<vector<bool>>geneValueRangesInclusiveOrExclusive,
			usint crossoverProbability,
			usint mutationProbability,
			usint selectionCutOffPercentage,
			SOMConfigurations * targetExperimentConfigurations,
			CalculationHelper * calculations
        );

		CalculationHelper * calculations;

        void setChromosomePopulationSize(unsigned int chromosomePopulationSize);
        void setIterations(unsigned int iterations);
        void setGenesAmount(unsigned int genesAmount);
        void setGeneValueRanges(vector<vector<float>> geneValueRanges);
        void setCrossoverProbability(usint crossoverProbability);
        void setMutationProbability(usint mutationProbability);
        void setSelectionCutOffPercentage(usint selectionCutOffPercentage);

        unsigned int getChromosomePopulationSize();
        unsigned int getIterations();
        unsigned int getGenesAmount();
        vector<vector<float>> getGeneValueRanges();
        usint getCrossoverProbability();
        usint getMutationProbability();
        usint getSelectionCutOffPercentage();

		void setTargetExperimentConfig(SOMConfigurations *);
		SOMConfigurations * getTargetExperimentConfig();

        ~GAConfigurations();
};

#endif