#include "SOMConfigurations.hpp"

SOMConfigurations::SOMConfigurations(int trainingSetPortion, matrix dataSet, CalculationHelper * calculations)
{
    SOMConfigurations::trainingSetPortion = trainingSetPortion;
    SOMConfigurations::dataSet = calculations->normaliseDataSet(dataSet);
    createTrainingSet();
	findCornerVectors();
}

SOMConfigurations::~SOMConfigurations()
{
	for (size_t i = 0; i < trainingSet.size(); i++)
	{
		InputVector * deletingVector = trainingSet.at(i);
		delete deletingVector;
	}
	trainingSet.clear();
	trainingSet.shrink_to_fit();
	for (size_t i = 0; i < dataSet.size(); i++)
	{
		vector<float> dataSetRow = dataSet.at(i);
		dataSetRow.clear();
		dataSetRow.shrink_to_fit();
	}
	dataSet.clear();
	dataSet.shrink_to_fit();
}


matrix SOMConfigurations::getDataSet()
{
    return dataSet;
}

int SOMConfigurations::getTrainingSetPortion()
{
    return trainingSetPortion;
}

void SOMConfigurations::createTrainingSet()
{
    matrix inputData = dataSet;
    random_shuffle(inputData.begin(), inputData.end());
    const int trainingSetSize = static_cast<int>(inputData.size() * calculations->percentageToFloat(trainingSetPortion));
    inputData.resize(trainingSetSize);
    trainingSet = convertMatrixToInputVectors(dataSet);
}

inputVectors SOMConfigurations::convertMatrixToInputVectors(matrix floatSet)
{
	inputVectors result;
	for each (vector<float> var in floatSet)
	{
		InputVector * current = new InputVector(var);
		result.push_back(current);
	}
	return result;
}

inputVectors SOMConfigurations::getTrainingSet()
{
    return trainingSet;
}

InputVector * SOMConfigurations::getTrainingVectorAt(int index)
{
	return trainingSet.at(index);
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
	float maxDistance = 0;
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
			const float currentDistance = calculations->euclidianDistance(
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
	float maxDistance = 0;
	const size_t trainingVectorsSize = trainingSet.size();
	for (size_t i = 0; i < trainingVectorsSize; i++)
	{
		InputVector * vector3 = trainingSet.at(i);
		const float distanceBetweenVector1And3 = calculations->euclidianDistance(
			vector1->getInputValues(),
			vector3->getInputValues()
		);
		const float distanceBetweenVector2And3 = calculations->euclidianDistance(
			vector2->getInputValues(),
			vector3->getInputValues()
		);
		const float currentDistance = distanceBetweenVector1And3 + distanceBetweenVector2And3;
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
	float maxDistance = 0;
	const size_t trainingVectorsSize = trainingSet.size();
	for (size_t i = 0; i < trainingVectorsSize; i++)
	{
		InputVector * vector4 = trainingSet.at(i);
		const float distanceBetweenVector1And4 = calculations->euclidianDistance(
			vector1->getInputValues(),
			vector4->getInputValues()
		);
		const float distanceBetweenVector2And4 = calculations->euclidianDistance(
			vector2->getInputValues(),
			vector4->getInputValues()
		);
		const float distanceBetweenVector3And4 = calculations->euclidianDistance(
			vector3->getInputValues(),
			vector4->getInputValues()
		);
		const float currentDistance = distanceBetweenVector1And4 + distanceBetweenVector2And4 + distanceBetweenVector3And4;
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

