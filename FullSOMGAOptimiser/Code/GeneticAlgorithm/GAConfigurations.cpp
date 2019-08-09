#include "GAConfigurations.hpp"

GAConfigurations::GAConfigurations(
    unsigned int chromosomePopulationSize,
    unsigned int iterations,
    unsigned int genesAmount,
    vector<usint[2]>geneValueRanges,
    vector<bool[2]>geneValueRangesInclusiveOrExclusive,
    usint crossoverProbability,
    usint mutationProbability,
    usint mutationPercentage
)
{
    GAConfigurations::chromosomePopulationSize = chromosomePopulationSize;
    GAConfigurations::iterations = iterations;
    GAConfigurations::genesAmount = genesAmount;
    GAConfigurations::geneValueRanges = geneValueRanges;
    GAConfigurations::geneValueRangesInclusiveOrExclusive = geneValueRangesInclusiveOrExclusive;
    GAConfigurations::crossoverProbability = crossoverProbability;
    GAConfigurations::mutationProbability = mutationProbability;
    GAConfigurations::mutationPercentage = mutationPercentage;

    setExactRangesOnGenes();
}

GAConfigurations::~GAConfigurations()
{

}

void GAConfigurations::setChromosomePopulationSize(unsigned int chromosomePopulationSize)
{
    GAConfigurations::chromosomePopulationSize = chromosomePopulationSize;
}

void GAConfigurations::setIterations(unsigned int iterations)
{
    GAConfigurations::iterations = iterations;
}

void GAConfigurations::setGenesAmount(unsigned int genesAmount)
{
    GAConfigurations::genesAmount = genesAmount;
}

void GAConfigurations::setGeneValueRanges(vector<usint[2]> geneValueRanges)
{
    GAConfigurations::geneValueRanges = geneValueRanges;
}

void GAConfigurations::setCrossoverProbability(usint crossoverProbability)
{
    GAConfigurations::crossoverProbability = crossoverProbability;
}

void GAConfigurations::setMutationProbability(usint mutationProbability)
{
    GAConfigurations::mutationProbability = mutationProbability;
}

void GAConfigurations::setMutationPercentage(usint mutationPercentage)
{
    GAConfigurations::mutationPercentage = mutationPercentage;
}

void GAConfigurations::setSelectionCutOffPercentage(usint selectionCutOffPercentage)
{
    GAConfigurations::selectionCutOffPercentage = selectionCutOffPercentage;
}

unsigned int GAConfigurations::getChromosomePopulationSize()
{
    return chromosomePopulationSize;
}

unsigned int GAConfigurations::getIterations()
{
    return iterations;
}

unsigned int GAConfigurations::getGenesAmount()
{
    return genesAmount;
}

vector<usint[2]> GAConfigurations::getGeneValueRanges()
{
    return geneValueRanges;
}

usint GAConfigurations::getCrossoverProbability()
{
    return crossoverProbability;
}

usint GAConfigurations::getMutationProbability()
{
    return mutationProbability;
}

usint GAConfigurations::getMutationPercentage()
{
    return mutationPercentage;
}

usint GAConfigurations::getSelectionCutOffPercentage()
{
    return selectionCutOffPercentage;
}

void GAConfigurations::setExactRangesOnGenes()
{
    for (size_t i = 0; i < geneValueRanges.size(); i++)
    {
        if (geneValueRangesInclusiveOrExclusive.at(i)[0]) {
            geneValueRanges.at(i)[0] += numeric_limits<float>::min();
        }
        if (geneValueRangesInclusiveOrExclusive.at(i)[1]) {
            geneValueRanges.at(i)[1] -= numeric_limits<float>::min();
        }
    }
}
