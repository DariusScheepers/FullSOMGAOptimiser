#include "Chromosome.hpp"

Chromosome::Chromosome(GAConfigurations * configurations)
{
	Chromosome::configurations = configurations;
    intialiseGenes();    
	fitnessValue = numeric_limits<float>::max();
	fitnessCalculated = false;
}
Chromosome::~Chromosome()
{
	genes.clear();
	genes.shrink_to_fit();
	if (fitnessCalculated) {
		// delete selfOrganisingMap;
	}
}

void Chromosome::intialiseGenes()
{
    const unsigned int genesPopulationAmount = configurations->getGeneValueRanges().size();
    for (size_t i = 0; i < genesPopulationAmount; i++)
    {
        const float randomValue = getGeneRandomValue(i);
        genes.push_back(randomValue);
		// cout << "R at " << i << " : " << randomValue;
    }
}

float Chromosome::getGeneRandomValue(size_t index) {
	vector<vector<float>> genesValueRanges = configurations->getGeneValueRanges();
    float minValue = genesValueRanges.at(index)[0];
    float maxValue = genesValueRanges.at(index)[1];
	return configurations->calculations->getRandomFloat(minValue, maxValue);
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

float Chromosome::mutateGene(size_t index)
{
	vector<vector<float>> genesValueRanges = configurations->getGeneValueRanges();
	const float maxGeneValue = genesValueRanges.at(index)[1];
	const float minxGeneValue = genesValueRanges.at(index)[0];
	const float mean = 0.0;
	const float standardDevaition = configurations->getMutationStandardDeviation();
	const float randomOffset = configurations->calculations->getRandomNormalDistributionFloat(mean, standardDevaition);
	const float currentGeneValue = genes.at(index);
	float newGeneValue = currentGeneValue + randomOffset;
	if (newGeneValue > maxGeneValue)
	{
		newGeneValue = maxGeneValue;
	}
	else if (newGeneValue < minxGeneValue)
	{
		newGeneValue = minxGeneValue;
	}
	return newGeneValue;


	//vector<vector<float>> genesValueRanges = configurations->getGeneValueRanges();
	//const float maxGeneValue = genesValueRanges.at(index)[1];
	//const float minxGeneValue = genesValueRanges.at(index)[0];
	//const float geneValueRangeAtIndex = maxGeneValue - minxGeneValue;
	//const float mutationOffsetAsDecimal = configurations->calculations->percentageToFloat(configurations->getMutationStandardDeviation());
	//const float geneValueOffset = geneValueRangeAtIndex * mutationOffsetAsDecimal;
	//const float currentGeneValue = genes.at(index);
	//const float minValue = currentGeneValue - geneValueOffset;
	//const float maxValue = currentGeneValue + geneValueOffset;
	//float newGeneValue = configurations->calculations->getRandomFloat(minValue, maxValue);
	//if (newGeneValue > maxGeneValue)
	//{
	//	newGeneValue = maxGeneValue;
	//}
	//else if (newGeneValue < minxGeneValue)
	//{
	//	newGeneValue = minxGeneValue;
	//}
	//
	//return newGeneValue;
}

void Chromosome::runAlgorithm(SOMConfigurations * somConfiguration)
{
	if (fitnessCalculated)
	{
		return;
	}

	fitnessValue = configurations->calculations->getRandomFloat(0, 100);
	fitnessCalculated = true;
	return;

	const unsigned short int rows = static_cast<unsigned short int>(genes.at(0));
	const unsigned short int columns = static_cast<unsigned short int>(genes.at(1));
	const float learningRate = genes.at(2);
	const float learningDecay = genes.at(3);
	const float kernelWidth = genes.at(4);
	const float kernelDecay = genes.at(5);

	selfOrganisingMap = new SelfOrganisingMap(
		somConfiguration,
		rows,
		columns,
		learningRate,
        learningDecay,
        kernelWidth,
        kernelDecay
    );
    selfOrganisingMap->runSelfOrganisingMap();
    fitnessValue = selfOrganisingMap->calculateQuantizationError();
	fitnessCalculated = true;
}