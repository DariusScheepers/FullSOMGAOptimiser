#include "SOMConfigurations.hpp"

SOMConfigurations::SOMConfigurations(int maxEpochs,
	int trainingSetPortion,
	matrix dataSet,
	int slidingWindowOffset,
	double stoppingCriteriaThreshhold,
	CalculationHelper * calculations,
	Writer * writer)
{
    SOMConfigurations::maxEpochs = maxEpochs;
	SOMConfigurations::trainingSetPortion = trainingSetPortion;
	SOMConfigurations::calculations = calculations;
	SOMConfigurations::slidingWindowOffset = slidingWindowOffset;
	SOMConfigurations::stoppingCriteriaThreshhold = stoppingCriteriaThreshhold;
    SOMConfigurations::dataSet = dataSet;
	SOMConfigurations::writer = writer;
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
	createTrainingAndTestSet();
	findCornerVectors();
}

void SOMConfigurations::shuffleDataSet()
{
	calculations->randomShuffleVectors(dataSet);
}

int SOMConfigurations::getMaxEpochs()
{
    return maxEpochs;
}

matrix SOMConfigurations::getDataSet()
{
    return dataSet;
}

void SOMConfigurations::createTrainingAndTestSet()
{
	shuffleDataSet();
	trainingSetPortion = static_cast<int>(dataSet.size() * calculations->percentageToDouble(trainingSetPortion));
	input = convertMatrixToInputVectors(dataSet);
}

inputVectors SOMConfigurations::convertMatrixToInputVectors(matrix doubleSet)
{
	inputVectors result;
	for each (vector<double> var in doubleSet)
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

void SOMConfigurations::findCornerVectors()
{
	vector<InputVector *> topLeftBottomRightTrainingVectors = findTopLeftAndBottomRightTrainingVectors();
	topLeftTrainingVector = topLeftBottomRightTrainingVectors.front();
	bottomRightTrainingVector = topLeftBottomRightTrainingVectors.back();
	bottomLeftTrainingVector = findBottomLeftTrainingVector(topLeftTrainingVector, bottomRightTrainingVector);
	topRightTrainingVector = findTopRightTrainingVector(topLeftTrainingVector, bottomRightTrainingVector, bottomLeftTrainingVector);
}


vector<InputVector*> SOMConfigurations::findTopLeftAndBottomRightTrainingVectors()
{
	vector<InputVector *> result(2);
	double maxDistance = 0;
	const size_t trainingVectorsSize = input.size();
	for (size_t i = 0; i < trainingVectorsSize; i++)
	{
		InputVector * firstSelectedTrainingVector = input.at(i);
		for (size_t j = 0; j < trainingVectorsSize; j++)
		{
			if (i == j)
			{
				continue;
			}
			InputVector * secondSelectedTrainingVector = input.at(j);
			const double currentDistance = calculations->euclidianDistance(
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

InputVector * SOMConfigurations::findBottomLeftTrainingVector(InputVector * vector1, InputVector * vector2)
{
	InputVector * bestVector;
	double maxDistance = 0;
	const size_t trainingVectorsSize = input.size();
	for (size_t i = 0; i < trainingVectorsSize; i++)
	{
		InputVector * vector3 = input.at(i);
		const double distanceBetweenVector1And3 = calculations->euclidianDistance(
			vector1->getInputValues(),
			vector3->getInputValues()
		);
		const double distanceBetweenVector2And3 = calculations->euclidianDistance(
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

InputVector * SOMConfigurations::findTopRightTrainingVector(InputVector * vector1, InputVector * vector2, InputVector * vector3)
{
	InputVector * bestVector;
	double maxDistance = 0;
	const size_t trainingVectorsSize = input.size();
	for (size_t i = 0; i < trainingVectorsSize; i++)
	{
		InputVector * vector4 = input.at(i);
		const double distanceBetweenVector1And4 = calculations->euclidianDistance(
			vector1->getInputValues(),
			vector4->getInputValues()
		);
		const double distanceBetweenVector2And4 = calculations->euclidianDistance(
			vector2->getInputValues(),
			vector4->getInputValues()
		);
		const double distanceBetweenVector3And4 = calculations->euclidianDistance(
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

InputVector * SOMConfigurations::getCornerVectorAt(cornerVectors cornerVector)
{
	switch (cornerVector)
	{
		case cornerVectors::topLeft:
			return topLeftTrainingVector;
		case cornerVectors::bottomRight:
			return bottomRightTrainingVector;
		case cornerVectors::bottomLeft:
			return bottomLeftTrainingVector;
		case cornerVectors::topRight:
			return topRightTrainingVector;
		default:
			break;
	}
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

inputVectors SOMConfigurations::getInput()
{
	return input;
}

int SOMConfigurations::getTrainingSetPortion()
{
	return trainingSetPortion;
}

