#include "GeneticAlgorithm.hpp";

GeneticAlgorithm::GeneticAlgorithm(GAConfigurations configurations)
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
    const auto maxIterations = configurations.getIterations(); 
    for (size_t i = 0; i < maxIterations; i++)
    {
        for (Chromosome * chromosome : chromosomes)
        {
            calculateFitness(chromosome);
        }
        generateOffSpring();
        sortChromosomesFromMostFittestToLowest();
        removeWeakestChromosomes();
    }
}

void GeneticAlgorithm::initialiseChromosomes()
{
    const auto size = configurations.getChromosomePopulationSize();
    for (size_t i = 0; i < size; i++)
    {
        Chromosome * newChromosome = new Chromosome();
        chromosomes.push_back(newChromosome);
    }
}

void GeneticAlgorithm::runSOMAndCalculateFitness()
{
    int amountOfThreads = configurations.getChromosomePopulationSize();
    // thread somThreads;
    vector<thread> somThreads;
    for (size_t i = 0; i < amountOfThreads; i++)
    {
        Chromosome * chromosome = chromosomes.at(i);
        thread somProgram(calculateFitness, chromosome);
        somThreads.push_back(move(somProgram));
    }

    for (size_t i = 0; i < amountOfThreads; i++)
    {
        thread * returningThread = &somThreads.at(i);
        if (returningThread->joinable())
        {
            returningThread->join();
        }
    }    
}

void GeneticAlgorithm::calculateFitness(Chromosome * chromosome)
{
    const float newFitnessValue = 0.0; // TODO

    // Run SOM program
    chromosome->setFitnessValue(newFitnessValue);
}

void GeneticAlgorithm::generateOffSpring()
{
    vector<Chromosome *> parents = getBestParentsByTournamentSelectionAlgorithm();
    vector<Chromosome *> offspring = createOffspringByUniformCrossover(parents);
    performMutation(offspring);
}

vector<Chromosome *> GeneticAlgorithm::getBestParentsByTournamentSelectionAlgorithm()
{
    const auto selectionCutOffSize = static_cast<int>(chromosomes.size() * (configurations.getSelectionCutOffPercentage() / 100));
    int parent1Index = indexOfBestChromosomeByTournamentSelection(selectionCutOffSize);
    Chromosome * parent1 = removeAndReturnChromosomeAt(parent1Index);
    int parent2Index = indexOfBestChromosomeByTournamentSelection(selectionCutOffSize);
    Chromosome * parent2 = chromosomes.at(parent2Index);
    chromosomes.push_back(parent1);

    vector<Chromosome *> parentPair;
    parentPair.push_back(parent1);
    parentPair.push_back(parent2);
    return parentPair;
}

int GeneticAlgorithm::indexOfBestChromosomeByTournamentSelection(int poolSize)
{
    default_random_engine generator;
    uniform_int_distribution<int> distribution(0, chromosomes.size() - 1);
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

    Chromosome * child1 = new Chromosome();
    Chromosome * child2 = new Chromosome();
    
    const auto childGenesAmount = configurations.getGenesAmount(); 
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
            newChildGene2 = parents.at(1)->getGene(i);
            newChildGene1 = parents.at(0)->getGene(i);
        }
        child1->setGene(i, newChildGene1);
        child2->setGene(i, newChildGene2);
    }

    vector<Chromosome *> newChildren;
    newChildren.push_back(child1);
    newChildren.push_back(child2);
    return newChildren;
}

void GeneticAlgorithm::performMutation(vector<Chromosome *> offspring)
{
    default_random_engine generator;
    uniform_int_distribution<int> distribution(0, 100);

    const auto mutationPercentage = configurations.getMutationPercentage();
    const auto mutationProbability = configurations.getMutationProbability();
    for (Chromosome * child : offspring)
    {
        int index = 0;
        for (float gene : child->getGenes())
        {
            int randomNumber = distribution(generator);
            if (randomNumber < mutationProbability)
            {
                float newGene = child->getGeneRandomValue(index);
                child->setGene(index++, newGene);
            }
        }        
    }
}

void GeneticAlgorithm::sortChromosomesFromMostFittestToLowest()
{
    sort(chromosomes.begin(), chromosomes.end());
}

void GeneticAlgorithm::removeWeakestChromosomes()
{
    const auto requiredPopulationSize = configurations.getChromosomePopulationSize();
    chromosomes.resize(requiredPopulationSize);
    chromosomes.shrink_to_fit();    
}

Chromosome * GeneticAlgorithm::returnBestChromsomes()
{
    return chromosomes[0];
}