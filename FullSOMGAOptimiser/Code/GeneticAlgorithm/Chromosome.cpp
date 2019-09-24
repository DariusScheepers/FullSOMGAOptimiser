#include "Chromosome.hpp"

Chromosome::Chromosome(int index, GAConfigurations * configurations)
{
	Chromosome::index = index;
	Chromosome::configurations = configurations;
    intialiseGenes();    
	fitnessValue = numeric_limits<double>::max();
	fitnessCalculated = false;
}
Chromosome::~Chromosome()
{
	genes.clear();
	genes.shrink_to_fit();
	if (!configurations->runTest && fitnessCalculated)
	{
		delete selfOrganisingMap;
	}
}

void Chromosome::intialiseGenes()
{
    const unsigned int genesPopulationAmount = configurations->getGeneValueRanges().size();
    for (size_t i = 0; i < genesPopulationAmount; i++)
    {
        const double randomValue = getGeneRandomValue(i);
        genes.push_back(randomValue);
		// cout << "R at " << i << " : " << randomValue;
    }
}

double Chromosome::getGeneRandomValue(size_t index) {
	vector<vector<double>> genesValueRanges = configurations->getGeneValueRanges();
    double minValue = genesValueRanges.at(index)[0];
    double maxValue = genesValueRanges.at(index)[1];
	return configurations->calculations->getRandomDouble(minValue, maxValue);
}

void Chromosome::setFitnessValue(double fitnessValue)
{
    Chromosome::fitnessValue = fitnessValue;
}

double Chromosome::getFitnessValue()
{
    return fitnessValue;
}

void Chromosome::setGene(int index, double value)
{
    genes.at(index) = value;
}

vector<double> Chromosome::getGenes()
{
    return genes;
}

double Chromosome::getGene(int index)
{
    return genes.at(index);
}

int Chromosome::getIndex()
{
	return index;
}

double Chromosome::mutateGene(size_t index)
{
	const double currentGeneValue = genes.at(index);
	const double standardDevaition = configurations->calculations->percentageToDouble(configurations->getMutationStandardDeviation());
	if (standardDevaition == 0.0)
	{
		return currentGeneValue;
	}
	vector<vector<double>> genesValueRanges = configurations->getGeneValueRanges();
	const double maxGeneValue = genesValueRanges.at(index)[1];
	const double minGeneValue = genesValueRanges.at(index)[0];
	const double mean = 0.0;
	const double newStD = standardDevaition * (maxGeneValue - minGeneValue);
	const double randomOffset = configurations->calculations->getRandomNormalDistributionDouble(mean, newStD);
	double newGeneValue = currentGeneValue + randomOffset;
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

	if (configurations->runTest)
	{
		fitnessValue = computeSpherical();
		fitnessCalculated = true;
		return;
	}

	const unsigned short int rows = static_cast<unsigned short int>(genes.at(0));
	const unsigned short int columns = static_cast<unsigned short int>(genes.at(1));
	const double learningRate = genes.at(2);
	const double learningDecay = genes.at(3);
	
	const double kernelWidthPortion = configurations->calculations->percentageToDouble(genes.at(4));
	const double kernelWidth = kernelWidthPortion * somConfiguration->getMaxDistanceBetweenCornerVectors();
	
	const double kernelDecay = genes.at(5);

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
    fitnessValue = selfOrganisingMap->calculatePerformance();
	cout << "Chromosomes fitness: " << fitnessValue << endl;
	fitnessCalculated = true;
}

double Chromosome::computeSpherical()
{
	const int maxIterations = genes.size();
	double sum = 0.0;
	//cout << "+++++++++++++++++++++++++++++++++\n";
	for (size_t i = 0; i < maxIterations; i++)
	{
		const double geneX = genes.at(i);
		//cout << "Gene Value " << geneX << endl;
		double xSquare = pow(geneX, 2);

		sum += xSquare;
	}
	//cout << "+++++++++++++++++++++++++++++++++\n";
	return sum;
}

SelfOrganisingMap * Chromosome::returnSolution()
{
	return selfOrganisingMap;
}