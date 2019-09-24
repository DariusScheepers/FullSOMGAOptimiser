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
	for (trainingWindowIteration = 0; trainingWindowIteration < 30; trainingWindowIteration++)
	{
		cout << "Training in window " << trainingWindowIteration << endl;
		deleteNeuronMap();
		createNeuronMap();
		prepareTrainingSet(trainingWindowIteration);
		trainSOM();
		testSetQEHistory.push_back(getTestRunAverageQuantizationError());
	}
}

void SelfOrganisingMap::prepareTrainingSet(int offset)
{
	trainingSet.clear();
	testSet.clear();
	InputVectors inputSet = configurations->getInput();
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
		iteration < maxIterations 
			&& stoppingCriteriaThreshhold < calculateStandardDeviationOfQE(slidingWindowSize);
		iteration++)
	{
		setNewLearningRateAndKernelWidth();
		InputVector * selectedVector = selectTrainingVector();
		Neuron * bestMatchingUnit = getBestMatchingUnit(selectedVector);
		updateEachNeuronWeights(selectedVector, bestMatchingUnit);

		const double qe = calculateQuantizationError();
		handleCurrentQEInfo(qe, output);
		printQuantizationError(slidingWindowSize);
	}

	string outputFileName = "SOMTrainings/"
		+ configurations->calculations->getTimeString()
		+ "_SOMTraining_"
		+ to_string(trainingWindowIteration);
	configurations->getWriter()->writeToFileWithName(outputFileName, output);
	// printEndNeuronMap();
	addToTrainingSetsQEHistory();
}

void SelfOrganisingMap::handleCurrentQEInfo(double qe, vector<string>& output)
{
	quantizationErrorHistory.push_back(qe);
	string line = to_string(iteration) + ",\t" + to_string(qe);
	if (configurations->fullOutput) {
		cout << line << endl;
	}
	output.push_back(line);
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
	if (configurations->fullOutput)
	{
		printInitialNeuronMap();
	}
}

void SelfOrganisingMap::addToTrainingSetsQEHistory()
{
	trainingSetsQEHistory.push_back(quantizationErrorHistory);
	quantizationErrorHistory.clear();
}

InputVector * SelfOrganisingMap::selectTrainingVector()
{
	int traingSetSize = trainingSet.size();
	if (iteration >= traingSetSize && traingSetSize % iteration == 0)
	{
		shuffleTrainingSet();
	}
	int index = iteration % traingSetSize;
	InputVector * chosenInputVector = trainingSet.at(index);
	return chosenInputVector;
}

void SelfOrganisingMap::shuffleTrainingSet()
{
	srand(time(NULL));
	random_shuffle(trainingSet.begin(), trainingSet.end());
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
				WeightCalculations * weightCalculations = new WeightCalculations(
					selectedTrainingVectorWeight,
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

void SelfOrganisingMap::setNewLearningRateAndKernelWidth()
{
    newLearningRate = calculateExponenialDecay(learningRate, iteration, learningDecay);
	// cout << "NewLearningRate: " << newLearningRate << endl;
    newKernelWidth = calculateExponenialDecay(kernelWidth, iteration, kernelDecay);
	// cout << "NewKernelWidth: " << newKernelWidth << endl;
}

void SelfOrganisingMap::performHypercubeWeightInitialization()
{
	const vector<double> bottomLeftTrainingVector = configurations->getCornerVectorAt(CornerVectors::bottomLeft)->getInputValues();
	const vector<double> topLeftTrainingVector = configurations->getCornerVectorAt(CornerVectors::topLeft)->getInputValues();
	const vector<double> bottomRightTrainingVector = configurations->getCornerVectorAt(CornerVectors::bottomRight)->getInputValues();
	const vector<double> topRightTrainingVector = configurations->getCornerVectorAt(CornerVectors::topRight)->getInputValues();

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

string SelfOrganisingMap::printNeuronMap()
{
	string map;
	for (size_t i = 0; i < neuronMap.size(); i++)
	{
		map = map + to_string(i) + ". ";
		vector<Neuron *> row = neuronMap.at(i);
		for (size_t j = 0; j < row.size(); j++)
		{
			vector<double> weights = row.at(j)->getWeights();
			map = map + "(";
			for (size_t k = 0; k < weights.size() - 1; k++)
			{
				map = map + to_string(weights.at(k)) + ", ";
			}
			map = map + to_string(weights.back()) + ")  ";
		}
		map = map + "\n\n";
	}

	if (configurations->fullOutput)
	{
		cout << map;
	}
	return map;
}

void SelfOrganisingMap::printInitialNeuronMap() {
	if (configurations->fullOutput)
	{
		cout << "Initial Neuron Map" << endl;
		printNeuronMap();
		cout << endl << endl;
	}
}

void SelfOrganisingMap::printQuantizationError(int slidingWindowSize)
{
	if (configurations->fullOutput)
	{
		const double quantizationError = quantizationErrorHistory.back();
		cout << "Quantization Error at Iteration: " << iteration << endl;
		cout << "\t" << quantizationError << endl;

		if (iteration % slidingWindowSize == 0)
		{
			cout << "Smooth Quantization Error: " << endl;
			cout << "\t" << calculateDecreaseInQE(slidingWindowSize) << endl;
		}
	}
}

void SelfOrganisingMap::printEndNeuronMap()
{
	if (configurations->fullOutput) {
		cout << "End Neuron Map" << endl;
		printNeuronMap();
		cout << endl << endl;
	}
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
	vector<double> bmuDistances;
	for (size_t i = 0; i < trainingVectorsSizeForTrainingVectorsLeft; i++)
    {
        InputVector * currentTrainingVector = trainingSet.at(i);
        Neuron * bmu = getBestMatchingUnit(currentTrainingVector);
        double bmuDistance = configurations->calculations->euclidianDistance(currentTrainingVector->getInputValues(), bmu->getWeights());
		bmuDistances.push_back(bmuDistance);
    }
	const double result = configurations->calculations->calculateAverage(bmuDistances);
	return result;
}

vector<double> SelfOrganisingMap::getLastWindowedQEHistory(int slidingWindowSize)
{
	vector<double> windowedQEHistory;
	int startingPosition = quantizationErrorHistory.size() - slidingWindowSize;
	if (startingPosition < 0)
	{
		startingPosition = 0;
	}
	for (size_t i = startingPosition; i < quantizationErrorHistory.size(); i++)
	{
		const double quantizationError = quantizationErrorHistory.at(i);
		windowedQEHistory.push_back(quantizationError);
	}
	return windowedQEHistory;
}

double SelfOrganisingMap::calculateDecreaseInQE(int slidingWindowSize)
{
	vector<double> windowedQEHistory = getLastWindowedQEHistory(slidingWindowSize);
	const double result = configurations->calculations->calculateAverage(windowedQEHistory);
	return result;
}

double SelfOrganisingMap::calculateStandardDeviationOfQE(int slidingWindowSize)
{
	if (iteration < slidingWindowSize)
	{
		return numeric_limits<double>::max();
	}
	const double decreaseInQE = calculateDecreaseInQE(slidingWindowSize);
	double sum = 0.0;
	vector<double> windowedQEHistory = getLastWindowedQEHistory(slidingWindowSize);
	for (double qe : windowedQEHistory)
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
	vector<double> bmuDistances;
	for (InputVector * testCandidate : testSet)
	{
		Neuron * bmu = getBestMatchingUnit(testCandidate);
		const double bmuDistance = configurations->calculations->euclidianDistance(testCandidate->getInputValues(), bmu->getWeights());
		bmuDistances.push_back(bmuDistance);
	}

	testSetsQEHistory.push_back(bmuDistances);
	const double result = configurations->calculations->calculateAverage(bmuDistances);
	cout << "Test Run Average,\t" << to_string(result) << endl;
	return result;
}

double SelfOrganisingMap::calculatePerformance()
{
	const double result = configurations->calculations->calculateAverage(testSetQEHistory);
	return result;
}

void SelfOrganisingMap::printTrainingAndTestSetsQEHistories()
{
	string fileName = configurations->calculations->getTimeString() + "_TestAndTrainingSetQEHistories";
	vector<string> output;
	int index = 0;
	for (vector<double> qeRow : trainingSetsQEHistory)
	{
		string outLine = "Training " + to_string(index++);
		for (double qe : qeRow)
		{
			outLine = outLine + " " + to_string(qe);
		}
		const double average = configurations->calculations->calculateAverage(qeRow);
		const double std = configurations->calculations->calculateStandardDeviation(qeRow);
		outLine = outLine + " Avg: " + to_string(average) + " Std: " + to_string(std) + "\n";
		output.push_back(outLine);
	}
	index = 0;
	for (vector<double> qeRow : testSetsQEHistory)
	{
		string outLine = "Test " + to_string(index++);
		for (double qe : qeRow)
		{
			outLine = outLine + " " + to_string(qe);
		}
		const double average = configurations->calculations->calculateAverage(qeRow);
		const double std = configurations->calculations->calculateStandardDeviation(qeRow);
		outLine = outLine + " Avg: " + to_string(average) + " Std: " + to_string(std) + "\n";
		output.push_back(outLine);
	}
	
	configurations->getWriter()->writeToFileWithName(fileName, output);
}

string SelfOrganisingMap::parametersToString()
{
	string output;
	output = "Self Organising Map Parameters:\n\tRows:\t\t\t" + to_string(rows)
		+ "\n\tColumns:\t\t" + to_string(columns)
		+ "\n\tLearning Rate:\t\t" + to_string(learningRate)
		+ "\n\tLearning Rate Decay:\t" + to_string(learningDecay)
		+ "\n\tKernel Width:\t\t" + to_string(kernelWidth)
		+ "\n\tKernel Width Decay:\t" + to_string(kernelDecay);
	return output;
}

void SelfOrganisingMap::printFinalNeuronMap()
{
	string output = printNeuronMap();
	string fileName = configurations->calculations->getTimeString()
		+ "_NeuronMap";
	configurations->getWriter()->writeToFileWithNameUsingOneLine(fileName, output);
}