#ifndef GA_CONFIGURATIONS_H
#define GA_CONFIGURATIONS_H

#include <vector>
#include <limits>
#include "../SelfOrganisingMap/SOMConfigurations.hpp"
#include "../Writers/Writer.h"
#define usint unsigned short int

using namespace std;

class GAConfigurations
{
    private:
        unsigned int chromosomePopulationSize;
        unsigned int maxIterations;
		double standardDeviationThreshold;
        unsigned int genesAmount;
        vector<vector<double>> geneValueRanges;
        vector<vector<bool>> geneValueRangesInclusiveOrExclusive;
        usint crossoverProbability;
        usint mutationProbability;
        usint selectionCutOffPercentage;
		usint crossOverSplit;
		usint mutationStandardDeviation;
		SOMConfigurations * targetExperimentConfigurations;
		Writer * writer;
        
		void runExperimentSpecificPreparations();
		void perpareDimensionsAsSqrtOfTrainingSetSize();
        void setExactRangesOnGenes();
    public:
		GAConfigurations(
			unsigned int chromosomePopulationSize,
			unsigned int maxIterations,
			double standardDeviationThreshold,
			vector<vector<double>> geneValueRanges,
			vector<vector<bool>> geneValueRangesInclusiveOrExclusive,
			usint mutationProbability,
			usint selectionCutOffPercentage,
			usint crossOverSplit,
			usint mutationStandardDeviation,
			SOMConfigurations * targetExperimentConfigurations,
			CalculationHelper * calculations,
			Writer *
        );
		~GAConfigurations();

		CalculationHelper * calculations;

        unsigned int getChromosomePopulationSize();
        unsigned int getMaxIterations();
        double getStandardDeviationThreshold();
        vector<vector<double>> getGeneValueRanges();
        usint getMutationProbability();
        usint getSelectionCutOffPercentage();
		usint getCrossOverSplit();
		usint getMutationStandardDeviation();

		SOMConfigurations * getTargetExperimentConfig();
		Writer * getWriter();

		bool runTest = false;
};

#endif