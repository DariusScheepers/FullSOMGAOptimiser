#include "GAConfigurations.hpp"

GAConfigurations::GAConfigurations(
    unsigned int chromosomePopulationSize,
    unsigned int iterations,
	vector<vector<float>>geneValueRanges,
    vector<vector<bool>>geneValueRangesInclusiveOrExclusive,
    usint mutationProbability,
	usint selectionCutOffPercentage,
	usint crossOverSplit,
	usint mutationStandardDeviation,
	SOMConfigurations * targetExperimentConfigurations,
	CalculationHelper * calculations
)
{
    GAConfigurations::chromosomePopulationSize = chromosomePopulationSize;
    GAConfigurations::iterations = iterations;
    GAConfigurations::genesAmount = genesAmount;
    GAConfigurations::geneValueRanges = geneValueRanges;
    GAConfigurations::geneValueRangesInclusiveOrExclusive = geneValueRangesInclusiveOrExclusive;
    GAConfigurations::crossoverProbability = crossoverProbability;
    GAConfigurations::mutationProbability = mutationProbability;
	GAConfigurations::selectionCutOffPercentage = selectionCutOffPercentage;
	GAConfigurations::crossOverSplit = crossOverSplit;
	GAConfigurations::mutationStandardDeviation = mutationStandardDeviation;
	GAConfigurations::targetExperimentConfigurations = targetExperimentConfigurations;
	GAConfigurations::calculations = calculations;

	if (!runTest)
	{
		targetExperimentConfigurations->runDataPreperations();
	}

    setExactRangesOnGenes();
}

GAConfigurations::~GAConfigurations()
{
	delete targetExperimentConfigurations;
}

unsigned int GAConfigurations::getChromosomePopulationSize()
{
    return chromosomePopulationSize;
}

unsigned int GAConfigurations::getIterations()
{
    return iterations;
}

vector<vector<float>> GAConfigurations::getGeneValueRanges()
{
    return geneValueRanges;
}

usint GAConfigurations::getMutationProbability()
{
    return mutationProbability;
}

usint GAConfigurations::getSelectionCutOffPercentage()
{
    return selectionCutOffPercentage;
}
usint GAConfigurations::getCrossOverSplit()
{
	return crossOverSplit;
}
usint GAConfigurations::getMutationStandardDeviation()
{
	return mutationStandardDeviation;
}

SOMConfigurations * GAConfigurations::getTargetExperimentConfig()
{
	return targetExperimentConfigurations;
}

void GAConfigurations::setExactRangesOnGenes()
{
    for (size_t i = 0; i < geneValueRanges.size(); i++)
    {
		const bool openingInclusive = geneValueRangesInclusiveOrExclusive.at(i)[0];
        if (!openingInclusive) {
			const float currentGeneValue = geneValueRanges.at(i)[0];
            geneValueRanges.at(i)[0] = currentGeneValue + numeric_limits<float>::min();
        }
		const bool closingInclusive = geneValueRangesInclusiveOrExclusive.at(i)[1];
		if (!closingInclusive) {
			const float currentGeneValue = geneValueRanges.at(i)[1];
            geneValueRanges.at(i)[1] = currentGeneValue - numeric_limits<float>::min();
        }
    }
}
