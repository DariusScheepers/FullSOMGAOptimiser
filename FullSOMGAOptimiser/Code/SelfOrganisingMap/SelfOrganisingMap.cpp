#include "SelfOrganisingMap.hpp"

SelfOrganisingMap::SelfOrganisingMap(SOMConfigurations * configurations, usint rows, usint columns, double learningRate, double learningDecay, double kernelWidth, double kernelDecay)
{
    SelfOrganisingMap::configurations = configurations;
    SelfOrganisingMap::rows = rows;
    SelfOrganisingMap::columns = columns;
    SelfOrganisingMap::learningRate = learningRate;
    SelfOrganisingMap::learningDecay = learningDecay;
    SelfOrganisingMap::kernelWidth = kernelWidth;
    SelfOrganisingMap::kernelDecay = kernelDecay;
}

SelfOrganisingMap::~SelfOrganisingMap()
{
	deleteNeuronMap();
}

void SelfOrganisingMap::deleteNeuronMap()
{
	for (size_t i = 0; i < neuronMap.size(); i++)
	{
		vector<Neuron *> neuronMapRow = neuronMap.at(i);
		for (size_t j = 0; j < neuronMapRow.size(); j++)
		{
			Neuron * deletingNeuron = neuronMapRow.at(j);
			delete deletingNeuron;
		}
		neuronMapRow.clear();
	}
	neuronMap.clear();
}

void SelfOrganisingMap::runSelfOrganisingMap() ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{	
	for (size_t i = 0; i < 30; i++)
	{
		deleteNeuronMap();
		createNeuronMap();
		prepareTrainingSet(i);
		trainSOM();
		testSetQEHistory.push_back(getTestRunAverageQuantizationError());
	}
}

void SelfOrganisingMap::prepareTrainingSet(int offset)
{
	trainingSet.clear();
	testSet.clear();
	inputVectors inputSet = configurations->getInput();
	int trainingSetStartIndex = 0 + offset;
	int trainingSetPortion = static_cast<int>((double)inputSet.size() - ((double)inputSet.size() / 30.0));
	int trainingSetEndIndex = (trainingSetPortion + offset) % inputSet.size();
	for (size_t i = 0; i < inputSet.size(); i++)
	{
		InputVector * nextInputVector = inputSet.at(i);
		if (trainingSetStartIndex < trainingSetEndIndex)
		{
			if (i < trainingSetStartIndex)
			{
				testSet.push_back(nextInputVector);
			}
			else if (i < trainingSetEndIndex)
			{
				trainingSet.push_back(nextInputVector);
			}
			else
			{
				testSet.push_back(nextInputVector);
			}
		}
		else
		{
			if (i < trainingSetEndIndex)
			{
				trainingSet.push_back(nextInputVector);
			}
			else if (i < trainingSetStartIndex)
			{
				testSet.push_back(nextInputVector);
			}
			else
			{
				trainingSet.push_back(nextInputVector);
			}
		}
	}
}

void SelfOrganisingMap::trainSOM()
{
	const double stoppingCriteriaThreshhold = configurations->getStoppingCriteriaThreshhold();
	const double slidingWindowSize = configurations->getSlidingWindowOffset();
	const int maxIterations = configurations->getMaxEpochs();
	vector<string> output;

	for (iteration = 0;
		iteration < trainingSet.size() || (iteration < maxIterations && calculateStandardDeviationOfQE(slidingWindowSize) >= stoppingCriteriaThreshhold);
		iteration++)
	{
		setNewLearningRateAndKernelWidth(iteration);
		InputVector * selectedVector = selectTrainingVector();
		Neuron * bestMatchingUnit = getBestMatchingUnit(selectedVector);
		updateEachNeuronWeights(selectedVector, bestMatchingUnit);

		double qe = calculateQuantizationError();
		addToQEHistory(qe, slidingWindowSize);
		string line = to_string(iteration) + "," + to_string(qe);
		// cout << line << endl;
		output.push_back(line);

		printQuantizationError(slidingWindowSize);
	}
	printEndNeuronMap();
	configurations->getWriter()->writeToFileWithName("test", output);
}

void SelfOrganisingMap::createNeuronMap()
{
    for (size_t i = 0; i < rows; i++)
    {
        vector<Neuron*> columnNeurons;
		for (size_t j = 0; j < columns; j++)
		{
			const size_t weightSize = configurations->getTrainingVectorAt(0)->getInputValues().size();
			vector<double> neuronWeights(weightSize);
			columnNeurons.push_back(new Neuron(neuronWeights, i, j));
		}
		neuronMap.push_back(columnNeurons);
    }

    performHypercubeWeightInitialization();
	// printInitialNeuronMap();
}

InputVector * SelfOrganisingMap::selectTrainingVector()
{
	int index = iteration % trainingSet.size();
	InputVector * chosenInputVector = trainingSet.at(index);
	return chosenInputVector;
}

Neuron * SelfOrganisingMap::getBestMatchingUnit(InputVector* selectedTrainingVector)
{
	Neuron * bmu;
	double closestDistance = numeric_limits<double>::max();
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < columns; j++)
		{
			Neuron * currentCandidate = neuronMap.at(i).at(j);
			const double currentDistance = configurations->calculations->euclidianDistance(
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
				const double currentWeight = currentNeuron->getWeightAt(k);
				const double selectedTrainingVectorWeight = selectedTrainingVector->getInputValueAt(k);
				WeightCalculations * weightCalculations = new WeightCalculations(selectedTrainingVectorWeight,
					bestMatchingUnit,
					currentNeuron,
					currentWeight,
					newLearningRate,
					newKernelWidth
				);
				const double newWeight = weightCalculations->returnNewNeuronWeight();
				//cout << "CurrentWeight: " << currentWeight << " New Weight: " << newWeight << endl;
				currentNeuron->setWeight(k, newWeight);

				delete weightCalculations;
			}
		}
	}
}

double SelfOrganisingMap::calculateExponenialDecay(double intialValue, int iteration, double decayConstant)
{
    const double exponent = (double)(-1 * iteration) / decayConstant;
    const double exponentialValue = exp(exponent);
    return intialValue * exponentialValue;
}

void SelfOrganisingMap::setNewLearningRateAndKernelWidth(int iteration)
{
    newLearningRate = calculateExponenialDecay(learningRate, iteration, learningDecay);
	// cout << "NewLearningRate: " << newLearningRate << endl;
    newKernelWidth = calculateExponenialDecay(kernelWidth, iteration, kernelDecay);
	// cout << "NewKernelWidth: " << newKernelWidth << endl;
}

void SelfOrganisingMap::performHypercubeWeightInitialization()
{
	const vector<double> bottomLeftTrainingVector = configurations->getCornerVectorAt(cornerVectors::bottomLeft)->getInputValues();
	const vector<double> topLeftTrainingVector = configurations->getCornerVectorAt(cornerVectors::topLeft)->getInputValues();
	const vector<double> bottomRightTrainingVector = configurations->getCornerVectorAt(cornerVectors::bottomRight)->getInputValues();
	const vector<double> topRightTrainingVector = configurations->getCornerVectorAt(cornerVectors::topRight)->getInputValues();

    neuronMap.at(0).at(0)->setAllWeights(bottomLeftTrainingVector);
    neuronMap.at(rows - 1).at(0)->setAllWeights(topLeftTrainingVector);
    neuronMap.at(0).at(columns - 1)->setAllWeights(bottomRightTrainingVector);
    neuronMap.at(rows - 1).at(columns - 1)->setAllWeights(topRightTrainingVector);

    for (size_t i = 2; i <= columns - 1; i++)
    {
		const int index = i - 1;
		const vector<double> adjustedWeights = adjustedWeightByHypercube(
            bottomRightTrainingVector,	
            bottomLeftTrainingVector,
            columns,
            i
        );
        neuronMap.at(0).at(index)->setAllWeights(adjustedWeights);
		const vector<double> adjustedWeights1 = adjustedWeightByHypercube(
            topRightTrainingVector,
            topLeftTrainingVector,
            columns,
            i
        );
        neuronMap.at(rows - 1).at(index)->setAllWeights(adjustedWeights1);
    }
    for (size_t i = 2; i <= rows - 1; i++)
    {
		const int rowIndex = i - 1;
		const vector<double> adjustedWeights = adjustedWeightByHypercube(
            topLeftTrainingVector,
            bottomLeftTrainingVector,
            rows,
            i
        );
        neuronMap.at(rowIndex).at(0)->setAllWeights(adjustedWeights);
		const vector<double> adjustedWeights1 = adjustedWeightByHypercube(
            topRightTrainingVector,
            bottomRightTrainingVector,
            rows,
            i
        );
        neuronMap.at(rowIndex).at(columns - 1)->setAllWeights(adjustedWeights1);
        for (size_t j = 2; j <= columns - 1; j++)
        {
			const int columnIndex = j - 1;
			const vector<double> adjustedWeights2 = adjustedWeightByHypercube(
                neuronMap.at(rowIndex).at(columns - 1)->getWeights(),
                neuronMap.at(rowIndex).at(0)->getWeights(),
                columns,
                j
            );
            neuronMap.at(rowIndex).at(columnIndex)->setAllWeights(adjustedWeights2);
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
			vector<double> weights = row.at(j)->getWeights();
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

void SelfOrganisingMap::printQuantizationError(int slidingWindowSize)
{
	const double quantizationError = quantizationErrorHistory.back();
	cout << "Quantization Error at Iteration: " << iteration << endl;
	cout << "\t " << quantizationError << endl;

	if (iteration % slidingWindowSize == 0)
	{
		cout << "Smooth Quantization Error: " << endl;
		cout << "\t" << calculateDecreaseInQE(slidingWindowSize) << endl;
	}
}

void SelfOrganisingMap::printEndNeuronMap()
{
	cout << "End Neuron Map" << endl;
	printNeuronMap();
	cout << endl << endl;
}

vector<double> SelfOrganisingMap::adjustedWeightByHypercube(vector<double> vector1, vector<double> vector2, int max, int index)
{
    const double scalar = ((double)index - 1) / ((double)max - 1);
    const vector<double> differenceInVector1And2 = configurations->calculations->differenceBetweenVectors(vector1, vector2);
    const vector<double> term1 = configurations->calculations->scalarTimesVector(scalar, differenceInVector1And2);
    const vector<double> result = configurations->calculations->sumOfVectors(term1, vector2);
	return result;
}

double SelfOrganisingMap::calculateQuantizationError()
{
    double sum = 0.0;
    const size_t trainingVectorsSizeForTrainingVectorsLeft = trainingSet.size();
	for (size_t i = 0; i < trainingVectorsSizeForTrainingVectorsLeft; i++)
    {
        InputVector * currentTrainingVector = trainingSet.at(i);
        Neuron * bmu = getBestMatchingUnit(currentTrainingVector);
        sum += configurations->calculations->euclidianDistance(currentTrainingVector->getInputValues(), bmu->getWeights());
    }
	const double result = sum / static_cast<double>(trainingVectorsSizeForTrainingVectorsLeft);
	return result;
}

void SelfOrganisingMap::addToQEHistory(double quantizationError, int slidingWindowSize)
{
	quantizationErrorHistory.push_back(quantizationError);
	if (quantizationErrorHistory.size() > slidingWindowSize)
	{
		rotate(quantizationErrorHistory.begin(), quantizationErrorHistory.begin() + 1, quantizationErrorHistory.end());
		quantizationErrorHistory.resize(slidingWindowSize);
	}
}

double SelfOrganisingMap::calculateDecreaseInQE(int slidingWindowSize)
{
	double sum = 0.0;
	for each (double qe in quantizationErrorHistory)
	{
		sum += qe;
	}
	const double result = sum / (double) slidingWindowSize;
	return result;

}

double SelfOrganisingMap::calculateStandardDeviationOfQE(int slidingWindowSize)
{
	if (iteration < slidingWindowSize)
	{
		return numeric_limits<double>::max();
	}
	double decreaseInQE = calculateDecreaseInQE(slidingWindowSize);
	double sum = 0.0;

	for each (double qe in quantizationErrorHistory)
	{
		const double differenceInQEAndDecreaseInQE = qe - decreaseInQE;
		const double square = differenceInQEAndDecreaseInQE * differenceInQEAndDecreaseInQE;
		sum += square;
	}
	const double average = sum / (double)(slidingWindowSize - 1);
	const double result = sqrt(average);
	return result;
}

double SelfOrganisingMap::getTestRunAverageQuantizationError()
{
	double sum = 0.0;
	for (InputVector * testCandidate: testSet)
	{
		Neuron * bmu = getBestMatchingUnit(testCandidate);
		sum += configurations->calculations->euclidianDistance(testCandidate->getInputValues(), bmu->getWeights());
	}

	const double result = sum / testSet.size();
	return result;
}

double SelfOrganisingMap::calculatePerformance()
{
	double sum = 0.0;
	for (double qe : testSetQEHistory)
	{
		sum += qe;
	}
	double result = sum / (double)testSetQEHistory.size();
	return result;
}