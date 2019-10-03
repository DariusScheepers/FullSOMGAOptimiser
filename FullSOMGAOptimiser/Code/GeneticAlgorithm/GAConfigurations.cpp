#include "GAConfigurations.hpp"

GAConfigurations::GAConfigurations(
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
	Writer * writer
)
{
    GAConfigurations::chromosomePopulationSize = chromosomePopulationSize;
    GAConfigurations::maxIterations = maxIterations;
    GAConfigurations::standardDeviationThreshold = standardDeviationThreshold;
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
	GAConfigurations::writer = writer;

	if (!runTest)
	{
		targetExperimentConfigurations->runDataPreperations();
	}

    setExactRangesOnGenes();
	runExperimentSpecificPreparations();
}

GAConfigurations::~GAConfigurations()
{
	delete targetExperimentConfigurations;
}

void GAConfigurations::runExperimentSpecificPreparations()
{
	perpareDimensionsAsSqrtOfTrainingSetSize();
}

void GAConfigurations::perpareDimensionsAsSqrtOfTrainingSetSize()
{
	const int inputSetSize = targetExperimentConfigurations->getInput().size();
	const double maxTrainingSetSize = round((double)inputSetSize - ((double)inputSetSize / targetExperimentConfigurations->getCrossValidationNumber()));
	const double addedToMaxForRounding = 0.5 - 0.0000000001; // numeric_limits<double>::min(); // 0.0000000001
	const double addedToMinForRounding = 0.5;

	const double fullMax = maxTrainingSetSize + addedToMaxForRounding;
	const double fullMin = geneValueRanges.at(0).at(0) - addedToMinForRounding;
	vector<double> dimension;
	dimension.push_back(fullMin);
	dimension.push_back(fullMax);

	geneValueRanges.at(0) = dimension;
	geneValueRanges.at(1) = dimension;
}

unsigned int GAConfigurations::getChromosomePopulationSize()
{
    return chromosomePopulationSize;
}

unsigned int GAConfigurations::getMaxIterations()
{
    return maxIterations;
}

double GAConfigurations::getStandardDeviationThreshold()
{
	return standardDeviationThreshold;
}

vector<vector<double>> GAConfigurations::getGeneValueRanges()
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
			const double currentGeneValue = geneValueRanges.at(i)[0];
            geneValueRanges.at(i)[0] = currentGeneValue + numeric_limits<double>::min();
        }
		const bool closingInclusive = geneValueRangesInclusiveOrExclusive.at(i)[1];
		if (!closingInclusive) {
			const double currentGeneValue = geneValueRanges.at(i)[1];
            geneValueRanges.at(i)[1] = currentGeneValue - numeric_limits<double>::min();
        }
    }
}

Writer * GAConfigurations::getWriter()
{
	return writer;
}

