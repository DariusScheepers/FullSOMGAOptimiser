#include "SelfOrganisingMap.hpp"

SelfOrganisingMap::SelfOrganisingMap(SOMConfigurations * configurations, usint rows, usint columns, double learningRate, double learningDecay, double kernelWidthPortion, double kernelDecay)
{
    SelfOrganisingMap::configurations = configurations;
    SelfOrganisingMap::rows = rows;
    SelfOrganisingMap::columns = columns;
    SelfOrganisingMap::learningRate = learningRate;
    SelfOrganisingMap::learningDecay = learningDecay;
    SelfOrganisingMap::kernelWidthPortion = kernelWidthPortion;
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

void SelfOrganisingMap::runNFoldCrossValidation(int crossValidationNumber)
{
	cout << "Running " << to_string(crossValidationNumber) << "-Fold Cross Validation..." << endl;
	for (trainingWindowIteration = 0; trainingWindowIteration < crossValidationNumber; trainingWindowIteration++)
	{
		cout << "Training in window " << trainingWindowIteration << endl;
		deleteNeuronMap();
		createNeuronMap();
		prepareTrainingSet(trainingWindowIteration, crossValidationNumber);
		trainSOM();
		testSetQEHistory.push_back(getTestRunAverageQuantizationError());
	}
}

void SelfOrganisingMap::runSelfOrganisingMap() ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{	
	cout << "Starting normal Training..." << endl;
	deleteNeuronMap();
	createNeuronMap();
	prepareTrainingSet(0, 5);
	trainSOM();
	testSetQEHistory.push_back(getTestRunAverageQuantizationError());
}

void SelfOrganisingMap::prepareTrainingSet(int offset, int crossFoldValidationNumber)
{
	trainingSet.clear();
	testSet.clear();
	InputVectors inputSet = configurations->getInput();
	int trainingSetStartIndex = 0 + offset;
	int trainingSetPortion = round((double)inputSet.size() - ((double)inputSet.size() / crossFoldValidationNumber));
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
	performHypercubeWeightInitialization();
	setKernelWidth();
}

void SelfOrganisingMap::trainSOM()
{
	const double stoppingCriteriaThreshhold = configurations->getStoppingCriteriaThreshhold();
	const double slidingWindowSize = configurations->getSlidingWindowOffset();
	const int maxIterations = configurations->getMaxEpochs();
	double std = numeric_limits<double>::max();
	vector<string> output;

	for (iteration = 0;
		iteration < maxIterations 
			&& stoppingCriteriaThreshhold < std;
		iteration++)
	{ 
		setNewLearningRateAndKernelWidth();
		InputVector * selectedVector = selectTrainingVector();
		Neuron * bestMatchingUnit = getBestMatchingUnit(selectedVector);
		updateEachNeuronWeights(selectedVector, bestMatchingUnit);

		const double qe = calculateQuantizationError();
		handleCurrentQEInfo(qe, output);
		printQuantizationError(slidingWindowSize);
		std = calculateStandardDeviationOfQE(slidingWindowSize);
	}

	string outputFileName = "SOMTrainings/"
		+ configurations->calculations->getTimeString()
		+ "_SOMTraining_"
		+ to_string(trainingWindowIteration);
	configurations->getWriter()->writeToFileWithName(outputFileName, output);
	// printEndNeuronMap();
	addToTrainingSetsQEHistory();
}

void SelfOrganisingMap::findCornerVectors()
{
	vector<InputVector *> topLeftBottomRightTrainingVectors = findTopLeftAndBottomRightTrainingVectors();
	topLeftTrainingVector = topLeftBottomRightTrainingVectors.front();
	bottomRightTrainingVector = topLeftBottomRightTrainingVectors.back();
	bottomLeftTrainingVector = findBottomLeftTrainingVector(topLeftTrainingVector, bottomRightTrainingVector);
	topRightTrainingVector = findTopRightTrainingVector(topLeftTrainingVector, bottomRightTrainingVector, bottomLeftTrainingVector);
}

vector<InputVector*> SelfOrganisingMap::findTopLeftAndBottomRightTrainingVectors()
{
	vector<InputVector *> result(2);
	double maxDistance = 0;
	const size_t trainingVectorsSize = trainingSet.size();
	for (size_t i = 0; i < trainingVectorsSize; i++)
	{
		InputVector * firstSelectedTrainingVector = trainingSet.at(i);
		for (size_t j = 0; j < trainingVectorsSize; j++)
		{
			if (i == j)
			{
				continue;
			}
			InputVector * secondSelectedTrainingVector = trainingSet.at(j);
			const double currentDistance = configurations->calculations->euclidianDistance(
				firstSelectedTrainingVector->getInputValues(),
				secondSelectedTrainingVector->getInputValues()
			);
			if (currentDistance > maxDistance)
			{
				maxDistance = currentDistance;
				result.at(0) = firstSelectedTrainingVector;
				result.at(1) = secondSelectedTrainingVector;
			}
		}
	}

	return result;
}

InputVector * SelfOrganisingMap::findBottomLeftTrainingVector(InputVector * vector1, InputVector * vector2)
{
	InputVector * bestVector;
	double maxDistance = 0;
	const size_t trainingVectorsSize = trainingSet.size();
	for (size_t i = 0; i < trainingVectorsSize; i++)
	{
		InputVector * vector3 = trainingSet.at(i);
		const double distanceBetweenVector1And3 = configurations->calculations->euclidianDistance(
			vector1->getInputValues(),
			vector3->getInputValues()
		);
		const double distanceBetweenVector2And3 = configurations->calculations->euclidianDistance(
			vector2->getInputValues(),
			vector3->getInputValues()
		);
		const double currentDistance = distanceBetweenVector1And3 + distanceBetweenVector2And3;
		if (currentDistance > maxDistance)
		{
			maxDistance = currentDistance;
			bestVector = vector3;
		}
	}
	return bestVector;
}

InputVector * SelfOrganisingMap::findTopRightTrainingVector(InputVector * vector1, InputVector * vector2, InputVector * vector3)
{
	InputVector * bestVector;
	double maxDistance = 0;
	const size_t trainingVectorsSize = trainingSet.size();
	for (size_t i = 0; i < trainingVectorsSize; i++)
	{
		InputVector * vector4 = trainingSet.at(i);
		const double distanceBetweenVector1And4 = configurations->calculations->euclidianDistance(
			vector1->getInputValues(),
			vector4->getInputValues()
		);
		const double distanceBetweenVector2And4 = configurations->calculations->euclidianDistance(
			vector2->getInputValues(),
			vector4->getInputValues()
		);
		const double distanceBetweenVector3And4 = configurations->calculations->euclidianDistance(
			vector3->getInputValues(),
			vector4->getInputValues()
		);
		const double currentDistance = distanceBetweenVector1And4 + distanceBetweenVector2And4 + distanceBetweenVector3And4;
		if (currentDistance > maxDistance)
		{
			maxDistance = currentDistance;
			bestVector = vector4;
		}
	}
	return bestVector;
}

void SelfOrganisingMap::setKernelWidth()
{
	vector<double> topLeftCoordinates = neuronMap.at(0).at(0)->getCoordinatesVector();
	vector<double> bottomRightCoordinates = neuronMap.at(rows - 1).at(columns - 1)->getCoordinatesVector();
	double diagonalDistanceOfMapCoordinates = configurations->calculations->euclidianDistance(topLeftCoordinates, bottomRightCoordinates);
	kernelWidth = configurations->calculations->percentageToDouble(kernelWidthPortion) * diagonalDistanceOfMapCoordinates;
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

double SelfOrganisingMap::calculateExponentialDecay(double intialValue, int iteration, double decayConstant)
{
    const double exponent = (double)(-1 * iteration) / decayConstant;
    const double exponentialValue = exp(exponent);
    return intialValue * exponentialValue;
}

void SelfOrganisingMap::setNewLearningRateAndKernelWidth()
{
    newLearningRate = calculateExponentialDecay(learningRate, iteration, learningDecay);
	// cout << "NewLearningRate: " << newLearningRate << endl;
    newKernelWidth = calculateExponentialDecay(kernelWidth, iteration, kernelDecay);
	// cout << "NewKernelWidth: " << newKernelWidth << endl;
}

void SelfOrganisingMap::performHypercubeWeightInitialization()
{
	findCornerVectors();

	const vector<double> bottomLeftTrainingVector = this->bottomLeftTrainingVector->getInputValues();
	const vector<double> topLeftTrainingVector = this->topLeftTrainingVector->getInputValues();
	const vector<double> bottomRightTrainingVector = this->bottomRightTrainingVector->getInputValues();
	const vector<double> topRightTrainingVector = this->topRightTrainingVector->getInputValues();

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
	dataMatrix allQEs;
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
		
		allQEs.push_back(qeRow);
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
		
		allQEs.push_back(qeRow);
	}
	string outLine = "Cross Fold Out Quantization Error:\t" + to_string(calculatePerformance()) + "\n";
	output.push_back(outLine);
	outLine = "TraingSet QE Avg:\t" + to_string(overAllQEHistoryAverage(trainingSetsQEHistory)) + "\n";
	output.push_back(outLine);
	outLine = "TestSet QE Avg:\t" + to_string(overAllQEHistoryAverage(testSetsQEHistory)) + "\n";
	output.push_back(outLine);
	outLine = "ALL QEs Avg:\t" + to_string(overAllQEHistoryAverage(allQEs)) + "\n";
	output.push_back(outLine);
	outLine = "TraingSet QE STD:\t" + to_string(overAllQEHistoryStandardDeviation(trainingSetsQEHistory)) + "\n";
	output.push_back(outLine);
	outLine = "TestSet QE STD:\t" + to_string(overAllQEHistoryStandardDeviation(testSetsQEHistory)) + "\n";
	output.push_back(outLine);
	outLine = "ALL QEs STD:\t" + to_string(overAllQEHistoryStandardDeviation(allQEs)) + "\n";
	output.push_back(outLine);
	outLine = "TraingSet QE Iterations:\t" + to_string(overAllQEHistoryIterations(trainingSetsQEHistory)) + "\n";
	output.push_back(outLine);
	outLine = "TestSet QE Iterations:\t" + to_string(overAllQEHistoryIterations(testSetsQEHistory)) + "\n";
	output.push_back(outLine);
	outLine = "ALL QEs Iterations:\t" + to_string(overAllQEHistoryIterations(allQEs)) + "\n";
	output.push_back(outLine);
	
	configurations->getWriter()->writeToFileWithName(fileName, output);
}
vector<double> SelfOrganisingMap::lineariseDataMatrix(dataMatrix qeHistory)
{
	vector<double> overAllQEHistory;
	for (vector<double> qeRow : qeHistory)
	{
		for (double qe : qeRow)
		{
			overAllQEHistory.push_back(qe);
		}
	}
	return overAllQEHistory;
}

double SelfOrganisingMap::overAllQEHistoryAverage(dataMatrix qeHistory)
{
	vector<double> overAllQEHistory = lineariseDataMatrix(qeHistory);
	const double result = configurations->calculations->calculateAverage(overAllQEHistory);
	return result;
}

double SelfOrganisingMap::overAllQEHistoryStandardDeviation(dataMatrix qeHistory)
{
	vector<double> overAllQEHistory = lineariseDataMatrix(qeHistory);
	const double result = configurations->calculations->calculateStandardDeviation(overAllQEHistory);
	return result;
}

int SelfOrganisingMap::overAllQEHistoryIterations(dataMatrix qeHistory)
{
	vector<double> overAllQEHistory = lineariseDataMatrix(qeHistory);
	const int result = overAllQEHistory.size();
	return result;
}

string SelfOrganisingMap::parametersToString()
{
	string output;
	output = "Self Organising Map Parameters:\n\tRows:\t\t\t" + to_string(rows)
		+ "\n\tColumns:\t\t" + to_string(columns)
		+ "\n\tLearning Rate:\t\t" + to_string(learningRate)
		+ "\n\tLearning Rate Decay:\t" + to_string(learningDecay)
		+ "\n\tKernel Width Portion:\t\t" + to_string(kernelWidthPortion)
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