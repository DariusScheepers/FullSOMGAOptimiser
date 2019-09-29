#include "SOMConfigurations.hpp"

SOMConfigurations::SOMConfigurations(int maxEpochs,
	int trainingSetPortion,
	Matrix dataSet,
	int slidingWindowOffset,
	double stoppingCriteriaThreshhold,
	CalculationHelper * calculations,
	Writer * writer,
	string dataSetName,
	bool fullOutput)
{
    SOMConfigurations::maxEpochs = maxEpochs;
	SOMConfigurations::trainingSetPortion = trainingSetPortion;
	SOMConfigurations::calculations = calculations;
	SOMConfigurations::slidingWindowOffset = slidingWindowOffset;
	SOMConfigurations::stoppingCriteriaThreshhold = stoppingCriteriaThreshhold;
    SOMConfigurations::dataSet = dataSet;
	SOMConfigurations::writer = writer;
	SOMConfigurations::dataSetName = dataSetName;
	SOMConfigurations::fullOutput = fullOutput;
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
	trainingSetPortion = static_cast<int>(dataSet.size() * calculations->percentageToDouble(trainingSetPortion));
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

int SOMConfigurations::getTrainingSetPortion()
{
	const double portion = dataSet.size() - (dataSet.size() / 30.0);
	return round(portion);
}
