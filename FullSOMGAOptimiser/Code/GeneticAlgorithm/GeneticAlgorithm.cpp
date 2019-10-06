#include "GeneticAlgorithm.hpp"

GeneticAlgorithm::GeneticAlgorithm(GAConfigurations * configurations)
{
    GeneticAlgorithm::configurations = configurations;
}

GeneticAlgorithm::~GeneticAlgorithm()
{
	for (size_t i = 0; i < chromosomes.size(); i++)
	{
		Chromosome * deletingChromosome = chromosomes.at(i);
		delete deletingChromosome;
	}
	chromosomes.clear();
	chromosomes.resize(0);
	delete configurations;
}

void GeneticAlgorithm::runGeneticAlgorithm()
{
    const unsigned int maxIterations = configurations->getMaxIterations();
    const double stdThreshhold = configurations->getStandardDeviationThreshold();
	cout << "Running GA..." << endl;

	standardDeviation = numeric_limits<double>::max();
	iteration = 0;

    initialiseChromosomes();
	setAllChromosomesFitness();
	handleChromosomesInformation();
    for	(iteration = 1;
		stdThreshhold < standardDeviation
			&& iteration < maxIterations;
		iteration++)
    {
		generateOffSpring();
		setAllChromosomesFitness();
		standardDeviation = calculateStandardDeviationForWindow();
		handleChromosomesInformation();
    }
	sortChromosomesFromMostFittestToLowest();
	printCurrentBestChromosome(maxIterations);
	printChromosomeGenesToFile(returnBestChromsomes());
}

void GeneticAlgorithm::initialiseChromosomes()
{
    const unsigned int size = configurations->getChromosomePopulationSize();
    for (size_t index = 0; index < size; index++)
    {
        Chromosome * newChromosome = new Chromosome(index, configurations);
        chromosomes.push_back(newChromosome);
    }
}

void GeneticAlgorithm::handleChromosomesInformation()
{
	sortChromosomesFromMostFittestToLowest();
	printCurrentBestChromosome(iteration);
	addFitnessValueToFitnessHistory();
}

void GeneticAlgorithm::addFitnessValueToFitnessHistory()
{
	const double bestFitness = returnBestChromsomes()->getFitnessValue();
	fitnessHistory.push_back(bestFitness);
}

double GeneticAlgorithm::calculateStandardDeviationForWindow()
{
	vector<double> windowedFitnessHistroy;
	const int window = 5;
	if (fitnessHistory.size() < 5)
	{
		return numeric_limits<double>::max();
	}
	for (size_t i = fitnessHistory.size() - window; i < fitnessHistory.size(); i++)
	{
		const double currentFitness = fitnessHistory.at(i);
		windowedFitnessHistroy.push_back(currentFitness);
	}
	const double std = configurations->calculations->calculateStandardDeviation(windowedFitnessHistroy);
	cout << "Standard deviation value:\t" << to_string(std) << endl;
	return std;
}

void GeneticAlgorithm::setAllChromosomesFitness()
{
	//runExperimentAndCalculateFitnessConcurrently();
	runExperimentAndCalculateFitnessLinear();
}

void GeneticAlgorithm::runExperimentAndCalculateFitnessLinear()
{
	for (Chromosome * chromosome : chromosomes)
	{
		calculateFitness(chromosome);
	}
}

void GeneticAlgorithm::runExperimentAndCalculateFitnessConcurrently()
{
    int amountOfThreads = chromosomes.size();
    vector<thread> experimentThreads;
    for (size_t i = 0; i < amountOfThreads; i++)
    {
        Chromosome * chromosome = chromosomes.at(i);
        thread experimentProgram(&GeneticAlgorithm::calculateFitness, this, chromosome);
		experimentThreads.push_back(move(experimentProgram));
    }

    for (size_t i = 0; i < amountOfThreads; i++)
    {
        thread * returningThread = &experimentThreads.at(i);    
        returningThread->join();
    }    
}

void GeneticAlgorithm::calculateFitness(Chromosome * chromosome)
{
	chromosome->runAlgorithm(configurations->getTargetExperimentConfig());
}

void GeneticAlgorithm::generateOffSpring()
{
	const int parentPairsAmount = (double)chromosomes.size() / (double)2;
	vector<Chromosome *> offspring;
	for (size_t i = 0; i < parentPairsAmount * 2; i += 2)
	{
		vector<Chromosome *> parents = getBestParentsByTournamentSelectionAlgorithm();
		vector<Chromosome *> addedOffspring = createOffspringByUniformCrossover(i, parents);
		addedOffspring = performMutation(addedOffspring);
		for (Chromosome * addedChild : addedOffspring)
		{
			offspring.push_back(addedChild);
		}
	}
	for (size_t i = 0; i < chromosomes.size(); i++)
	{
		Chromosome * parent = chromosomes.at(i);
		delete parent;
	}
	chromosomes.clear();
	for (Chromosome * child : offspring)
	{
		chromosomes.push_back(child);
	}
}

vector<Chromosome *> GeneticAlgorithm::getBestParentsByTournamentSelectionAlgorithm()
{
    const int poolSize = getPoolSize();
    int parent1Index = indexOfBestChromosomeByTournamentSelection(poolSize);
    Chromosome * parent1 = removeAndReturnChromosomeAt(parent1Index);
    int parent2Index = indexOfBestChromosomeByTournamentSelection(poolSize);
    Chromosome * parent2 = chromosomes.at(parent2Index);
	chromosomes.push_back(parent1);

    vector<Chromosome *> parentPair;
    parentPair.push_back(parent1);
    parentPair.push_back(parent2);
    return parentPair;
}

int GeneticAlgorithm::getPoolSize()
{
	const double decimalSelectionCutOffSize = configurations->calculations->percentageToDouble(configurations->getSelectionCutOffPercentage());
	const int result = ceil(decimalSelectionCutOffSize * (double)chromosomes.size());
	return result;
}

int GeneticAlgorithm::indexOfBestChromosomeByTournamentSelection(int poolSize)
{
	vector<Chromosome *> chromosomesInTournament;
	for (size_t i = 0; i < poolSize; i++)
	{
		int currentChromosomePopulationSize = chromosomes.size() - 1;
		const int randomIndex = configurations->calculations->getRandomInt(0, currentChromosomePopulationSize);
		Chromosome * pickedChromosome = removeAndReturnChromosomeAt(randomIndex);
		chromosomesInTournament.push_back(pickedChromosome);
	}

	int bestChromosomeIndex;
	double bestFitness = numeric_limits<double>::max();
    for (size_t i = 0; i < poolSize; i++)
    {
        Chromosome * pickedChromosome = chromosomesInTournament.at(i);
        if (pickedChromosome->getFitnessValue() < bestFitness)
        {
            bestChromosomeIndex = pickedChromosome->getIndex();
            bestFitness = pickedChromosome->getFitnessValue();
        }
		chromosomes.push_back(pickedChromosome);
    }
	chromosomesInTournament.clear();
	for (size_t i = 0; i < chromosomes.size(); i++)
	{
		Chromosome * chr = chromosomes.at(i);
		if (chr->getIndex() == bestChromosomeIndex)
		{
			return i;
		}
	}
}

Chromosome * GeneticAlgorithm::removeAndReturnChromosomeAt(int index)
{
    Chromosome * selectedChromosome = chromosomes.at(index);
    chromosomes.at(index) = chromosomes.back();
    chromosomes.pop_back();
    return selectedChromosome;
}

vector<Chromosome *> GeneticAlgorithm::createOffspringByUniformCrossover(int index, vector<Chromosome *> parents)
{
    Chromosome * child1 = new Chromosome(index, configurations);
    Chromosome * child2 = new Chromosome(index + 1, configurations);
    
    const unsigned int childGenesAmount = configurations->getGeneValueRanges().size();
	const int crossOverSplit = configurations->getCrossOverSplit();
    for (size_t i = 0; i < childGenesAmount; i++)
    {
		int randomNumber = configurations->calculations->getRandomInt(0, 100);
        double newChildGene1;
        double newChildGene2;
        if (randomNumber > crossOverSplit)
        {
            newChildGene1 = parents.at(0)->getGene(i);
            newChildGene2 = parents.at(1)->getGene(i);
        }
        else
        {
            newChildGene2 = parents.at(0)->getGene(i);
            newChildGene1 = parents.at(1)->getGene(i);
        }
        child1->setGene(i, newChildGene1);
        child2->setGene(i, newChildGene2);
    }

    vector<Chromosome *> newChildren;
    newChildren.push_back(child1);
    newChildren.push_back(child2);
    return newChildren;
}

void GeneticAlgorithm::deleteChromosomes(vector<Chromosome *> deletingChromosomes)
{
	for (Chromosome * deletingChromosome : deletingChromosomes)
	{
		delete deletingChromosome;
	}
}

vector<Chromosome *> GeneticAlgorithm::performMutation(vector<Chromosome *> offspring)
{
	vector<Chromosome *> mutatedChildren;
	mutatedChildren.push_back(new Chromosome(offspring.at(0)->getIndex(), configurations));
	mutatedChildren.push_back(new Chromosome(offspring.at(1)->getIndex(), configurations));

    const unsigned short mutationProbability = configurations->getMutationProbability();
    for (size_t i = 0; i < offspring.size(); i++)
    {
		Chromosome * child = offspring.at(i);
        int index = 0;
        for (double gene : child->getGenes())
        {
			int randomNumber = configurations->calculations->getRandomInt(0, 100);
			double newGene = child->getGene(index);
            if (randomNumber <= mutationProbability)
            {
                newGene = child->mutateGene(index);
			}  
			mutatedChildren.at(i)->setGene(index++, newGene);
        }
		delete child;
    }
	return mutatedChildren;
}

void GeneticAlgorithm::sortChromosomesFromMostFittestToLowest()
{
	struct less_than_key
	{
		inline bool operator() (Chromosome* chr1, Chromosome* chr2)
		{
			return (chr1->getFitnessValue() < chr2->getFitnessValue());
		}
	};
    sort(chromosomes.begin(), chromosomes.end(), less_than_key());
}


void GeneticAlgorithm::removeWeakestChromosomes()
{
    const size_t requiredPopulationSize = configurations->getChromosomePopulationSize();
	const size_t currentChromosomesSize = chromosomes.size();
	if (requiredPopulationSize == currentChromosomesSize)
	{
		return;
	}
	// chromosomes.erase(chromosomes.begin() + requiredPopulationSize, chromosomes.begin() + currentChromosomesSize - 1);
	for (size_t i = requiredPopulationSize; i < chromosomes.size(); i++)
	{
		Chromosome * deletingChromosome = chromosomes.at(i);
		delete deletingChromosome;
	}
    chromosomes.resize(requiredPopulationSize);
    chromosomes.shrink_to_fit();
}

Chromosome * GeneticAlgorithm::returnBestChromsomes()
{
    return chromosomes[0];
}

Chromosome * GeneticAlgorithm::findChromosomeByIndex(int index)
{
	for (Chromosome * chr : chromosomes)
	{
		if (chr->getIndex() == index)
		{
			return chr;
		}
	}
}

void GeneticAlgorithm::printCurrentBestChromosome(int iteration)
{
	Chromosome * bestChromosome = returnBestChromsomes();
	cout << "========================\n";
	cout << "Best Chromosome at iteration " << iteration + 1 << "/" << configurations->getMaxIterations() 
		<< " With STD: " << to_string(standardDeviation) << " > " << to_string(configurations->getStandardDeviationThreshold()) << endl;
	cout << "\tMemory Loc " << bestChromosome << endl;
	cout << "\tIndex " << bestChromosome->getIndex() << endl;
	cout << "\tFitness " << bestChromosome->getFitnessValue() << endl;
	cout << "\tGenes Values" << endl;
	for (size_t i = 0; i < bestChromosome->getGenes().size(); i++)
	{
		const double geneValue = bestChromosome->getGene(i);
		cout << "\t\t" << geneValue << endl;
	}
	cout << "========================\n";
	
	string fileName = configurations->calculations->getTimeString()
		+ "_Best_Solution_Parameters_Iteration_"
		+ to_string(iteration);
	string output = bestChromosome->returnSolution()->parametersToString();
	output = output + "\nFitness:\t" + to_string(bestChromosome->getFitnessValue());
	configurations->getWriter()->writeToFileWithNameUsingOneLine(fileName, output);
}

void GeneticAlgorithm::printChromosomeGenesToFile(Chromosome * chromosome)
{
	vector<string> output;
	for (double gene : chromosome->getGenes())
	{
		string line = to_string(gene);
		output.push_back(line);
	}

	string fileName = configurations->getTargetExperimentConfig()->getDataSetName()
		+ "_" + configurations->calculations->getTimeString()
		+ "_FitnessValue_"
		+ to_string(chromosome->getFitnessValue())
		+ "_GenesAmount_"
		+ to_string(chromosome->getGenes().size());
	configurations->getWriter()->writeToFileWithName(fileName, output);
}

void GeneticAlgorithm::printFitnessHistoryToFile()
{
	vector<string> output;
	for (double fitness : fitnessHistory)
	{
		string line = to_string(fitness);
		output.push_back(line);
	}

	string fileName = configurations->getTargetExperimentConfig()->getDataSetName()
		+ "_" + configurations->calculations->getTimeString()
		+ "_FitnessHistory_";
	configurations->getWriter()->writeToFileWithName(fileName, output);
}