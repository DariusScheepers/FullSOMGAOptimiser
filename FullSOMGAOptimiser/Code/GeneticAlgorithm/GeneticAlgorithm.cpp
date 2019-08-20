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
    initialiseChromosomes();
    const unsigned int maxIterations = configurations->getIterations(); 
    for (size_t i = 0; i < maxIterations; i++)
    {
		setAllChromosomesFitness();
        sortChromosomesFromMostFittestToLowest();
        removeWeakestChromosomes();
        generateOffSpring();
		printCurrentBestChromosome(i);
    }
}

void GeneticAlgorithm::initialiseChromosomes()
{
    const unsigned int size = configurations->getChromosomePopulationSize();
    for (size_t i = 0; i < size; i++)
    {
        Chromosome * newChromosome = new Chromosome(configurations);
        chromosomes.push_back(newChromosome);
    }
}

void GeneticAlgorithm::setAllChromosomesFitness()
{
	runExperimentAndCalculateFitnessConcurrently();
	//runExperimentAndCalculateFitnessLinear();
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
	const int parentPairsAmount = (float)chromosomes.size() / (float)2;
	vector<Chromosome *> offspring;
	for (size_t i = 0; i < parentPairsAmount; i++)
	{
		vector<Chromosome *> parents = getBestParentsByTournamentSelectionAlgorithm();
		vector<Chromosome *> addedOffspring = createOffspringByUniformCrossover(parents);
		addedOffspring = performMutation(addedOffspring);
		for each (Chromosome * addedChild in addedOffspring)
		{
			offspring.push_back(addedChild);
		}
	}
	for each (Chromosome * child in offspring)
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
    Chromosome * parent2 = removeAndReturnChromosomeAt(parent2Index);

    vector<Chromosome *> parentPair;
    parentPair.push_back(parent1);
    parentPair.push_back(parent2);
    return parentPair;
}

int GeneticAlgorithm::getPoolSize()
{
	const float decimalSelectionCutOffSize = configurations->calculations->percentageToFloat(configurations->getSelectionCutOffPercentage());
	const int result = ceil(decimalSelectionCutOffSize * (float)chromosomes.size());
	return result;
}

int GeneticAlgorithm::indexOfBestChromosomeByTournamentSelection(int poolSize)
{
	int populationSize = chromosomes.size() - 1;
    int bestChromosomeIndex;
    float bestFitness = numeric_limits<float>::max();
    for (size_t i = 0; i < poolSize; i++)
    {
        const int randomIndex = configurations->calculations->getRandomInt(0, populationSize);

        Chromosome * pickedChromosome = chromosomes.at(randomIndex);
        if (pickedChromosome->getFitnessValue() < bestFitness)
        {
            bestChromosomeIndex = randomIndex;
            bestFitness = pickedChromosome->getFitnessValue();
        }
    }
    return bestChromosomeIndex;
}

Chromosome * GeneticAlgorithm::removeAndReturnChromosomeAt(int index)
{
    Chromosome * selectedChromosome = chromosomes.at(index);
    chromosomes.at(index) = chromosomes.back();
    chromosomes.pop_back();
    return selectedChromosome;
}

vector<Chromosome *> GeneticAlgorithm::createOffspringByUniformCrossover(vector<Chromosome *> parents)
{
    Chromosome * child1 = new Chromosome(configurations);
    Chromosome * child2 = new Chromosome(configurations);
    
    const unsigned int childGenesAmount = configurations->getGenesAmount();
	const usint crossOverSplit = configurations->getCrossOverSplit();
    for (size_t i = 0; i < childGenesAmount; i++)
    {
		int randomNumber = configurations->calculations->getRandomInt(0, 100);
        float newChildGene1;
        float newChildGene2;
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

	deleteChromosomes(parents);
    vector<Chromosome *> newChildren;
    newChildren.push_back(child1);
    newChildren.push_back(child2);
    return newChildren;
}

void GeneticAlgorithm::deleteChromosomes(vector<Chromosome *> deletingChromosomes)
{
	for each (Chromosome * deletingChromosome in deletingChromosomes)
	{
		delete deletingChromosome;
	}
}

vector<Chromosome *> GeneticAlgorithm::performMutation(vector<Chromosome *> offspring)
{
	vector<Chromosome *> mutatedChildren;
	mutatedChildren.push_back(new Chromosome(configurations));
	mutatedChildren.push_back(new Chromosome(configurations));

    const unsigned short mutationProbability = configurations->getMutationProbability();
    for (size_t i = 0; i < offspring.size(); i++)
    {
		Chromosome * child = offspring.at(i);
        int index = 0;
        for (float gene : child->getGenes())
        {
			int randomNumber = configurations->calculations->getRandomInt(0, 100);
			float newGene = child->getGene(index);
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

void GeneticAlgorithm::printCurrentBestChromosome(int iteration)
{
	Chromosome * bestChromosome = chromosomes.at(0);
	cout << "========================\n";
	cout << "Best Chromosome at iteration " << iteration + 1 << "/" << configurations->getIterations() << endl;
	cout << "\t Fitness " << bestChromosome->getFitnessValue() << endl;
	cout << "\t Genes Values" << endl;
	for (size_t i = 0; i < bestChromosome->getGenes().size(); i++)
	{
		const float geneValue = bestChromosome->getGene(i);
		cout << "\t\t" << geneValue << endl;
	}
	cout << "========================\n";
}