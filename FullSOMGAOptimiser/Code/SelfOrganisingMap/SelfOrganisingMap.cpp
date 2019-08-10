#include "SelfOrganisingMap.hpp"

SelfOrganisingMap::SelfOrganisingMap(SOMConfigurations * configurations, usint rows, usint columns, float learningRate, float learningDecay, float kernelWidth, float kernelDecay, int trainingIterations)
{
    SelfOrganisingMap::configurations = configurations;
    SelfOrganisingMap::rows = rows;
    SelfOrganisingMap::columns = columns;
    SelfOrganisingMap::learningRate = learningRate;
    SelfOrganisingMap::learningDecay = learningDecay;
    SelfOrganisingMap::kernelWidth = kernelWidth;
    SelfOrganisingMap::kernelDecay = kernelDecay;
	SelfOrganisingMap::trainingIterations = trainingIterations;
}

SelfOrganisingMap::~SelfOrganisingMap()
{
	for (size_t i = 0; i < neuronMap.size(); i++)
	{
		neuronMap.at(i).clear();
		neuronMap.at(i).shrink_to_fit();
	}
	neuronMap.clear();
	neuronMap.shrink_to_fit();
}

void SelfOrganisingMap::runSelfOrganisingMap()
{
	createNeuronMap();
	// printInitialNeuronMap();
    for (iteration = 0; iteration < trainingIterations; iteration++)
    {
        setNewLearningRateAndKernelWidth(iteration);
        InputVector * selectedVector = selectTrainingVector();
        Neuron * bestMatchingUnit = getBestMatchingUnit(selectedVector);
        updateEachNeuronWeights(selectedVector, bestMatchingUnit);
		printQuantizationError();
    }
	// printEndNeuronMap();
}

void SelfOrganisingMap::createNeuronMap()
{
	neuronMatrix newNeuronMap;
    for (size_t i = 0; i < rows; i++)
    {
        vector<Neuron*> columnNeurons;
		for (size_t j = 0; j < columns; j++)
		{
			const size_t weightSize = configurations->getTrainingVectorAt(0)->getInputValues().size();
			vector<float> neuronWeights(weightSize);
			columnNeurons.push_back(new Neuron(neuronWeights, i, j));
		}
		newNeuronMap.push_back(columnNeurons);
    }
	neuronMap = newNeuronMap;

    performHypercubeWeightInitialization();
}

InputVector * SelfOrganisingMap::selectTrainingVector()
{
    default_random_engine generator;
	int trainingSetSize = configurations->getTrainingSet().size() - 1;
    uniform_int_distribution<int> distribution(0, trainingSetSize);
    const int randomIndex = distribution(generator);
    return configurations->getTrainingVectorAt(randomIndex);
}

Neuron * SelfOrganisingMap::getBestMatchingUnit(InputVector* selectedTrainingVector)
{
	CalculationHelper calculations;
	Neuron * bmu;
	float closestDistance = numeric_limits<float>::max();
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < columns; j++)
		{
			Neuron * currentCandidate = neuronMap.at(i).at(j);
			const float currentDistance = calculations.euclidianDistance(
				currentCandidate->getWeights(),
				selectedTrainingVector->getInputValues()
			);
			if (currentDistance < closestDistance)
			{
				closestDistance = currentDistance;
				bmu = currentCandidate;
			}
		}
	}

	return bmu;
}

void SelfOrganisingMap::updateEachNeuronWeights(InputVector * selectedTrainingVector, Neuron * bestMatchingUnit)
{
	for (size_t i = 0; i < rows; i++)
	{
		vector<Neuron *> currentNeuronRow = neuronMap.at(i);
		for (size_t j = 0; j < columns; j++)
		{
			Neuron * currentNeuron = currentNeuronRow.at(j);
			const int neuronWeightsSize = currentNeuron->getWeights().size();
			for (size_t k = 0; k < neuronWeightsSize; k++)
			{
				const float currentWeight = currentNeuron->getWeightAt(k);
				const float selectedTrainingVectorWeight = selectedTrainingVector->getInputValueAt(k);
				WeightCalculations * weightCalculations = new WeightCalculations(selectedTrainingVectorWeight, bestMatchingUnit, currentNeuron, currentWeight, newLearningRate, newKernelWidth, iteration);
				const float newWeight = weightCalculations->returnNewNeuronWeight();
				currentNeuron->setWeight(k, newWeight);

				delete weightCalculations;
			}
		}
	}
}

float SelfOrganisingMap::calculateExponenialDecay(float intialValue, int iteration, float decayConstant)
{
    const float exponent = (float)(-1 * iteration) / decayConstant;
    const float exponentialValue = exp(exponent);
    return intialValue * exponentialValue;
}

void SelfOrganisingMap::setNewLearningRateAndKernelWidth(int iteration)
{
    newLearningRate = calculateExponenialDecay(learningRate, iteration, learningDecay);
    newKernelWidth = calculateExponenialDecay(kernelWidth, iteration, kernelDecay);
}

void SelfOrganisingMap::performHypercubeWeightInitialization()
{
	const vector<float> bottomLeftTrainingVector = configurations->getCornerVectorAt(cornerVectors::bottomLeft)->getInputValues();
	const vector<float> topLeftTrainingVector = configurations->getCornerVectorAt(cornerVectors::topLeft)->getInputValues();
	const vector<float> bottomRightTrainingVector = configurations->getCornerVectorAt(cornerVectors::bottomRight)->getInputValues();
	const vector<float> topRightTrainingVector = configurations->getCornerVectorAt(cornerVectors::topRight)->getInputValues();

    neuronMap.at(0).at(0)->setAllWeights(bottomLeftTrainingVector);
    neuronMap.at(rows - 1).at(0)->setAllWeights(topLeftTrainingVector);
    neuronMap.at(0).at(columns - 1)->setAllWeights(bottomRightTrainingVector);
    neuronMap.at(rows - 1).at(columns - 1)->setAllWeights(topRightTrainingVector);

    for (size_t i = 2; i <= columns - 1; i++)
    {
		const int index = i - 1;
        neuronMap.at(0).at(index)->setAllWeights(adjustedWeightByHypercube(
            bottomRightTrainingVector,	
            bottomLeftTrainingVector,
            columns,
            i
        ));
        neuronMap.at(rows - 1).at(index)->setAllWeights(adjustedWeightByHypercube(
            topRightTrainingVector,
            topLeftTrainingVector,
            columns,
            i
        ));
    }
    for (size_t i = 2; i <= rows - 1; i++)
    {
		const int rowIndex = i - 1;
        neuronMap.at(rowIndex).at(0)->setAllWeights(adjustedWeightByHypercube(
            topLeftTrainingVector,
            bottomLeftTrainingVector,
            rows,
            i
        ));
        neuronMap.at(rowIndex).at(columns - 1)->setAllWeights(adjustedWeightByHypercube(
            topRightTrainingVector,
            bottomRightTrainingVector,
            rows,
            i
        ));
        for (size_t j = 2; j <= columns - 1; j++)
        {
			const int columnIndex = j - 1;
            neuronMap.at(rowIndex).at(columnIndex)->setAllWeights(adjustedWeightByHypercube(
                neuronMap.at(rowIndex).at(columns - 1)->getWeights(),
                neuronMap.at(rowIndex).at(0)->getWeights(),
                columns,
                j
            ));
        }
    }
}

void SelfOrganisingMap::printNeuronMap()
{
	for (size_t i = 0; i < neuronMap.size(); i++)
	{
		cout << i << ". ";
		vector<Neuron *> row = neuronMap.at(i);
		for (size_t j = 0; j < row.size(); j++)
		{
			vector<float> weights = row.at(j)->getWeights();
			cout << "(";
			for (size_t k = 0; k < weights.size() - 1; k++)
			{
				cout << weights.at(k) << ", ";
			}
			cout << weights.back() << ")  ";
		}
		cout << endl << endl;
	}
}

void SelfOrganisingMap::printInitialNeuronMap() {
	cout << "Initial Neuron Map" << endl;
	printNeuronMap();
	cout << endl << endl;
}

void SelfOrganisingMap::printQuantizationError()
{
	cout << "Quantization Error at Iteration: " << iteration << endl;
	cout << "\t " << calculateQuantizationError() << endl;
}

void SelfOrganisingMap::printEndNeuronMap()
{
	cout << "End Neuron Map" << endl;
	printNeuronMap();
	cout << endl << endl;
}

vector<float> SelfOrganisingMap::adjustedWeightByHypercube(vector<float> vector1, vector<float> vector2, int max, int index)
{
    CalculationHelper calculations;
    const float scalar = ((float)index - 1) / ((float)max - 1);
    const vector<float> differenceInVector1And2 = calculations.differenceBetweenVectors(vector1, vector2);
    const vector<float> term1 = calculations.scalarTimesVector(scalar, differenceInVector1And2);
    const vector<float> result = calculations.sumOfVectors(term1, vector2);
	return result;
}

float SelfOrganisingMap::calculateQuantizationError()
{
    CalculationHelper calculations;
    float sum = 0;
    const auto trainingVectorsSize = configurations->getTrainingSet().size();
    for (size_t i = 0; i < trainingVectorsSize; i++)
    {
        InputVector * currentTrainingVectors = configurations->getTrainingSet().at(i);
        Neuron * bmu = getBestMatchingUnit(currentTrainingVectors);
        sum += calculations.euclidianDistance(currentTrainingVectors->getInputValues(), bmu->getWeights());
    }
    return sum / trainingVectorsSize;
}