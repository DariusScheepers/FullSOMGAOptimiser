#include "CalculationHelper.hpp"

CalculationHelper::CalculationHelper()
{
}

CalculationHelper::~CalculationHelper()
{

}

vector<float> CalculationHelper::differenceBetweenVectors(vector<float> vector1, vector<float> vector2)
{
    const size_t vectorSize = vector1.size();
    vector<float> result;
    for (size_t i = 0; i < vectorSize; i++)
    {
        const float difference = vector1.at(i) - vector2.at(i);
        result.push_back(difference);
    }
    return result;  

}

float CalculationHelper::magnitudeOfVector(vector<float> vectorValues)
{
    float sumValues = 0;
    for (size_t i = 0; i < vectorValues.size(); i++)
    {
        sumValues += vectorValues.at(i) * vectorValues.at(i);
    }
    return sqrt(sumValues); 

}

float CalculationHelper::magintudeOfDifferenceBetweenVectors(vector<float> vector1, vector<float> vector2)
{
    const vector<float> differenceInVectors = differenceBetweenVectors(vector1, vector2);
    return magnitudeOfVector(differenceInVectors);
}

vector<float> CalculationHelper::sumOfVectors(vector<float> vector1, vector<float> vector2)
{
    const size_t vectorSize = vector1.size();
    vector<float> result;
    for (size_t i = 0; i < vectorSize; i++)
    {
        const float sum = vector1.at(i) + vector2.at(i);
        result.push_back(sum);
    }
    return result;
}

float CalculationHelper::euclidianDistance(vector<float> vector1, vector<float> vector2)
{
    float sumOfDifferences = 0;
    const size_t vectorSize = vector1.size();
    for (size_t i = 0; i < vectorSize; i++)
    {
        const float difference = vector1.at(i) - vector2.at(i);
        const float sqaureValue = difference * difference;
        sumOfDifferences += sqaureValue;
    }
    const float squareRootValue = sqrt(sumOfDifferences);
    return squareRootValue;    
}

vector<float> CalculationHelper::scalarTimesVector(float scalar, vector<float> vector0)
{
    const size_t vectorSize = vector0.size();
    vector<float> result;
    for (size_t i = 0; i < vectorSize; i++)
    {
        const float product = vector0.at(i) * scalar;
        result.push_back(product);
    }
    return result;    
}

float CalculationHelper::percentageToFloat(float percentage)
{
    return percentage / (float)100;
}

dataMatrix CalculationHelper::normaliseDataSet(dataMatrix dataSet)
{
	vector<float> columnMaxValues;
	vector<float> columnMinValues;
	for (size_t i = 0; i < dataSet.at(0).size(); i++)
	{
		columnMaxValues.push_back(numeric_limits<float>::min());
		columnMinValues.push_back(numeric_limits<float>::max());
	}
	for (size_t i = 0; i < dataSet.size(); i++)
	{
		vector<float> row = dataSet.at(i);
		for (size_t j = 0; j < row.size(); j++)
		{
			float value = row.at(j);
			if (value < columnMinValues.at(j))
			{
				columnMinValues.at(j) = value;
			}
			if (value > columnMaxValues.at(j))
			{
				columnMaxValues.at(j) = value;
			}
		}
	}
	dataMatrix normalisedSet;
	for (size_t i = 0; i < dataSet.size(); i++)
	{
		vector<float> row = dataSet.at(i);
		vector<float> normalisedRow;
		for (size_t j = 0; j < row.size(); j++)
		{
			normalisedRow.push_back(normaliseValue(row.at(j), columnMinValues.at(j), columnMaxValues.at(j)));
		}
		normalisedSet.push_back(normalisedRow);
	}
	
	return normalisedSet;
}

float CalculationHelper::normaliseValue(float value, float min, float max)
{
	const float upper = value - min;
	const float lower = max - min;
	return upper / lower;
}

float CalculationHelper::getRandomFloat(float minValue, float maxValue)
{
	float random = ((float)rand()) / (float)RAND_MAX;

	// generate (in your case) a float between 0 and (4.5-.78)
	// then add .78, giving you a float between .78 and 4.5
	float range = maxValue - minValue;
	float result = (random*range) + minValue;
	return result;
}

int CalculationHelper::getRandomInt(int minValue, int maxValue)
{
	float random = ((float)rand()) / (float)RAND_MAX;

	// generate (in your case) a float between 0 and (4.5-.78)
	// then add .78, giving you a float between .78 and 4.5
	int range = maxValue - minValue;
	int result = (random*range) + minValue;
	return result;
}