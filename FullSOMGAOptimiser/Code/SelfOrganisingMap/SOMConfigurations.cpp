#include "SOMConfigurations.hpp"

SOMConfigurations::SOMConfigurations(int maxEpochs,
	int trainingSetSize,
	Matrix dataSet,
	int slidingWindowOffset,
	int crossValidationNumber,
	double stoppingCriteriaThreshhold,
	CalculationHelper * calculations,
	Writer * writer,
	string dataSetName,
	bool fullOutput)
{
    SOMConfigurations::maxEpochs = maxEpochs;
	SOMConfigurations::calculations = calculations;
	SOMConfigurations::slidingWindowOffset = slidingWindowOffset;
	SOMConfigurations::stoppingCriteriaThreshhold = stoppingCriteriaThreshhold;
	SOMConfigurations::crossValidationNumber = crossValidationNumber;
    SOMConfigurations::dataSet = dataSet;
	SOMConfigurations::writer = writer;
	SOMConfigurations::dataSetName = dataSetName;
	SOMConfigurations::fullOutput = fullOutput;

	SOMConfigurations::trainingSetSize = trainingSetSize;
}

SOMConfigurations::~SOMConfigurations()
{
	for (size_t i = 0; i < input.size(); i++)
	{
		InputVector * deletingVector = input.at(i);
		delete deletingVector;
	}
	input.clear();
	input.shrink_to_fit();
	for (size_t i = 0; i < dataSet.size(); i++)
	{
		vector<double> dataSetRow = dataSet.at(i);
		dataSetRow.clear();
		dataSetRow.shrink_to_fit();
	}
	dataSet.clear();
	dataSet.shrink_to_fit();
}

void SOMConfigurations::runDataPreperations()
{
	dataSet = calculations->normaliseDataSet(dataSet);
	shuffleAndFormatData();
}

void SOMConfigurations::shuffleDataSet()
{
	calculations->randomShuffleVectors(dataSet);
}

int SOMConfigurations::getMaxEpochs()
{
    return maxEpochs;
}

Matrix SOMConfigurations::getDataSet()
{
    return dataSet;
}

void SOMConfigurations::shuffleAndFormatData()
{
	shuffleDataSet();
	input = convertMatrixToInputVectors(dataSet);
}

InputVectors SOMConfigurations::convertMatrixToInputVectors(Matrix doubleSet)
{
	InputVectors result;
	for (vector<double> var : doubleSet)
	{
		InputVector * current = new InputVector(var);
		result.push_back(current);
	}
	return result;
}

InputVector * SOMConfigurations::getTrainingVectorAt(int index)
{
	return input.at(index);
}

double SOMConfigurations::getStoppingCriteriaThreshhold()
{
	return stoppingCriteriaThreshhold;
}

int SOMConfigurations::getSlidingWindowOffset()
{
	return slidingWindowOffset;
}

Writer * SOMConfigurations::getWriter()
{
	return writer;
}

InputVectors SOMConfigurations::getInput()
{
	return input;
}

string SOMConfigurations::getDataSetName()
{
	return dataSetName;
}

int SOMConfigurations::getCrossValidationNumber()
{
	return crossValidationNumber;
}

int SOMConfigurations::getTrainingSetSize()
{
	return 0; // TODO
}

vector<double> SOMConfigurations::getMinMaxDimensions()
{
	const double max = sqrt(input.size());
	const int maxTrunc = trunc(max);
	const double addedToMaxForRounding = 0.5 - 0.0000000001; // numeric_limits<double>::min(); // 0.0000000001
	const double addedToMinForRounding = 0.5;

	const double fullMax = maxTrunc + addedToMaxForRounding;
	const double fullMin = 2 - addedToMinForRounding;
	vector<double> dimension;
	dimension.push_back(fullMin);
	dimension.push_back(fullMax);

	return dimension;
}
