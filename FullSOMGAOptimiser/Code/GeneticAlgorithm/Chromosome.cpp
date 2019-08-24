#include "Chromosome.hpp"

Chromosome::Chromosome(int index, GAConfigurations * configurations)
{
	Chromosome::index = index;
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

int Chromosome::getIndex()
{
	return index;
}

float Chromosome::mutateGene(size_t index)
{
	const float currentGeneValue = genes.at(index);
	const float standardDevaition = configurations->calculations->percentageToFloat(configurations->getMutationStandardDeviation());
	if (standardDevaition == 0.0)
	{
		return currentGeneValue;
	}
	vector<vector<float>> genesValueRanges = configurations->getGeneValueRanges();
	const float maxGeneValue = genesValueRanges.at(index)[1];
	const float minGeneValue = genesValueRanges.at(index)[0];
	const float mean = 0.0;
	const float newStD = standardDevaition * (maxGeneValue - minGeneValue);
	const float randomOffset = configurations->calculations->getRandomNormalDistributionFloat(mean, newStD);
	float newGeneValue = currentGeneValue + randomOffset;
	if (newGeneValue > maxGeneValue)
	{
		newGeneValue = maxGeneValue;
	}
	else if (newGeneValue < minGeneValue)
	{
		newGeneValue = minGeneValue;
	}
	return newGeneValue;
	//return newGeneValue;
}

void Chromosome::runAlgorithm(SOMConfigurations * somConfiguration)
{
	if (fitnessCalculated)
	{
		return;
	}

	fitnessValue = computeSpherical();
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

float Chromosome::computeSpherical()
{
	const float M_PI = 3.14159;
	const int maxIterations = genes.size();
	float sum = 0.0;
	//cout << "+++++++++++++++++++++++++++++++++\n";
	for (size_t i = 0; i < maxIterations; i++)
	{
		const float geneX = genes.at(i);
		//cout << "Gene Value " << geneX << endl;
		float xSquare = pow(geneX, 2);

		sum += xSquare;
	}
	//cout << "+++++++++++++++++++++++++++++++++\n";
	return sum;
}