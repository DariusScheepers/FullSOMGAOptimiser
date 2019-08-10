#include "GeneticAlgorithm.hpp"

GeneticAlgorithm::GeneticAlgorithm(GAConfigurations * configurations)
{
    GeneticAlgorithm::configurations = configurations;
}

GeneticAlgorithm::~GeneticAlgorithm()
{
    chromosomes.resize(0);
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
    }
	cout << "Best Chromosome Fitness: " << chromosomes.at(0)->getFitnessValue() << endl;
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
	// runExperimentAndCalculateFitnessLinear();
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
    int amountOfThreads = configurations->getChromosomePopulationSize();
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
    vector<Chromosome *> parents = getBestParentsByTournamentSelectionAlgorithm();
    vector<Chromosome *> offspring = createOffspringByUniformCrossover(parents);
	offspring = performMutation(offspring);
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
    Chromosome * parent2 = chromosomes.at(parent2Index);
    chromosomes.push_back(parent1);

    vector<Chromosome *> parentPair;
    parentPair.push_back(parent1);
    parentPair.push_back(parent2);
    return parentPair;
}

int GeneticAlgorithm::getPoolSize()
{
	CalculationHelper calculations;
	const float decimalSelectionCutOffSize = calculations.percentageToFloat(configurations->getSelectionCutOffPercentage());
	const int result = static_cast<int> (decimalSelectionCutOffSize * (float)chromosomes.size());
	return result;
}

int GeneticAlgorithm::indexOfBestChromosomeByTournamentSelection(int poolSize)
{
    default_random_engine generator;
	int populationSize = chromosomes.size() - 1;
    uniform_int_distribution<int> distribution(0, populationSize);
    int bestChromosomeIndex;
    float bestFitness = numeric_limits<float>::max();
    for (size_t i = 0; i < poolSize; i++)
    {
        const int randomIndex = distribution(generator);

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
    default_random_engine generator;
    uniform_int_distribution<int> distribution(0, 100);

    Chromosome * child1 = new Chromosome(configurations);
    Chromosome * child2 = new Chromosome(configurations);
    
    const unsigned int childGenesAmount = configurations->getGenesAmount();
    for (size_t i = 0; i < childGenesAmount; i++)
    {
        int randomNumber = distribution(generator);
        float newChildGene1;
        float newChildGene2;
        if (randomNumber > 50)
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

vector<Chromosome *> GeneticAlgorithm::performMutation(vector<Chromosome *> offspring)
{
    default_random_engine generator;
    uniform_int_distribution<int> distribution(0, 100);

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
            int randomNumber = distribution(generator);
			float newGene = child->getGene(index);
            if (randomNumber <= mutationProbability)
            {
                newGene = child->getGeneRandomValue(index);
			}  
			mutatedChildren.at(i)->setGene(index++, newGene);
        }
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
	chromosomes.erase(chromosomes.begin() + requiredPopulationSize, chromosomes.begin() + currentChromosomesSize - 1);
    chromosomes.resize(requiredPopulationSize);
    chromosomes.shrink_to_fit();
}

Chromosome * GeneticAlgorithm::returnBestChromsomes()
{
    return chromosomes[0];
}