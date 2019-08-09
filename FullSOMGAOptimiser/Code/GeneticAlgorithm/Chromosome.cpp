#include "Chromosome.hpp"

enum class Chromosome::SelfOrganisingMapParameters {
    rows = 0,
    columns = 1,
    learningRate = 2,
    learningDecay = 3,
    kernelWidth = 4,
    kernelDecay = 5
};

Chromosome::Chromosome()
{
    intialiseGenes();    
}

void Chromosome::intialiseGenes()
{
    const auto genesPopulationAmount = configurations.getGenesAmount();
    for (size_t i = 0; i < genesPopulationAmount; i++)
    {
        const auto randomValue = getGeneRandomValue(i);
        genes.push_back(randomValue);
    }
}

float Chromosome::getGeneRandomValue(size_t index) {
    const default_random_engine generator;
    const auto genesValueRanges = configurations.getGeneValueRanges();
    const auto minValue = genesValueRanges.at(index)[0];
    const auto maxValue = genesValueRanges.at(index)[1];
    uniform_int_distribution<float> distribution(minValue, maxValue);

    return distribution(generator);
}

void Chromosome::setFitnessValue(float fitnessValue)
{
    Chromosome::fitnessValue = fitnessValue;
}

float Chromosome::getFitnessValue()
{
    return fitnessValue;
}

void Chromosome::setGene(int index, float value)
{
    genes.at(index) = value;
}

vector<float> Chromosome::getGenes()
{
    return genes;
}

float Chromosome::getGene(int index)
{
    return genes.at(index);
}

bool Chromosome::operator>(Chromosome* comparingChromosome) {
    return fitnessValue > comparingChromosome->fitnessValue;
}

void Chromosome::runAlgorithm()
{
    selfOrganisingMap = new SelfOrganisingMap(
        somConfig,
        static_cast<int>(genes.at(SelfOrganisingMapParameters::rows)),
        static_cast<int>(genes.at(SelfOrganisingMapParameters::columns)),
        genes.at(SelfOrganisingMapParameters::learningRate),
        genes.at(SelfOrganisingMapParameters::learningDecay),
        genes.at(SelfOrganisingMapParameters::kernelWidth),
        genes.at(SelfOrganisingMapParameters::kernelDecay),
    );
    selfOrganisingMap->runSelfOrganisingMap();
    fitnessValue = selfOrganisingMap->calculateQuantizationError();
}