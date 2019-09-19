#include "CalculationHelper.hpp"

CalculationHelper::CalculationHelper()
{
	srand(time(NULL));
	random_device rd;
	mt19937 generator(rd());
	CalculationHelper::generator = generator;
}

CalculationHelper::~CalculationHelper()
{

}

vector<double> CalculationHelper::differenceBetweenVectors(vector<double> vector1, vector<double> vector2)
{
    const size_t vectorSize = vector1.size();
    vector<double> result;
    for (size_t i = 0; i < vectorSize; i++)
    {
        const double difference = vector1.at(i) - vector2.at(i);
        result.push_back(difference);
    }
    return result;  

}

double CalculationHelper::magnitudeOfVector(vector<double> vectorValues)
{
    double sumValues = 0;
    for (size_t i = 0; i < vectorValues.size(); i++)
    {
        sumValues += vectorValues.at(i) * vectorValues.at(i);
    }
    return sqrt(sumValues); 

}

double CalculationHelper::magintudeOfDifferenceBetweenVectors(vector<double> vector1, vector<double> vector2)
{
    const vector<double> differenceInVectors = differenceBetweenVectors(vector1, vector2);
    return magnitudeOfVector(differenceInVectors);
}

vector<double> CalculationHelper::sumOfVectors(vector<double> vector1, vector<double> vector2)
{
    const size_t vectorSize = vector1.size();
    vector<double> result;
    for (size_t i = 0; i < vectorSize; i++)
    {
        const double sum = vector1.at(i) + vector2.at(i);
        result.push_back(sum);
    }
    return result;
}

double CalculationHelper::euclidianDistance(vector<double> vector1, vector<double> vector2)
{
	double sumOfDifferences = 0.0;
    const size_t vectorSize = vector1.size();
    for (size_t i = 0; i < vectorSize; i++)
    {
        const double difference = vector1.at(i) - vector2.at(i);
        const double sqaureValue = difference * difference;
        sumOfDifferences += sqaureValue;
    }
    const double squareRootValue = sqrt(sumOfDifferences);
    return squareRootValue;    
}

vector<double> CalculationHelper::scalarTimesVector(double scalar, vector<double> vector0)
{
    const size_t vectorSize = vector0.size();
    vector<double> result;
    for (size_t i = 0; i < vectorSize; i++)
    {
        const double product = vector0.at(i) * scalar;
        result.push_back(product);
    }
    return result;    
}

double CalculationHelper::percentageToDouble(double percentage)
{
    return percentage / (double)100;
}

dataMatrix CalculationHelper::normaliseDataSet(dataMatrix dataSet)
{
	vector<double> columnMaxValues;
	vector<double> columnMinValues;
	for (size_t i = 0; i < dataSet.at(0).size(); i++)
	{
		columnMaxValues.push_back(numeric_limits<double>::min());
		columnMinValues.push_back(numeric_limits<double>::max());
	}
	for (size_t i = 0; i < dataSet.size(); i++)
	{
		vector<double> row = dataSet.at(i);
		for (size_t j = 0; j < row.size(); j++)
		{
			double value = row.at(j);
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
		vector<double> row = dataSet.at(i);
		vector<double> normalisedRow;
		for (size_t j = 0; j < row.size(); j++)
		{
			normalisedRow.push_back(normaliseValue(row.at(j), columnMinValues.at(j), columnMaxValues.at(j)));
		}
		normalisedSet.push_back(normalisedRow);
	}
	
	return normalisedSet;
}

double CalculationHelper::normaliseValue(double value, double min, double max)
{
	const double upper = value - min;
	const double lower = max - min;
	return upper / lower;
}

double CalculationHelper::getRandomDouble(double minValue, double maxValue)
{
	uniform_real_distribution<double> distribution(minValue, maxValue);
	const double result = distribution(generator);
	return result;
}

int CalculationHelper::getRandomInt(int minValue, int maxValue)
{
	uniform_int_distribution<int> distribution(minValue, maxValue);
	const int result = distribution(generator);
	return result;
}


double CalculationHelper::getRandomNormalDistributionDouble(double mean, double std)
{
	normal_distribution<double> distribution(mean, std);
	const double result = distribution(generator);
	return result;
}

vector<double> CalculationHelper::randomShuffleDouble(vector<double> &randomOrdering)
{
	random_shuffle(randomOrdering.begin(), randomOrdering.end());
	return randomOrdering;
}

vector<vector<double>> CalculationHelper::randomShuffleVectors(vector<vector<double>> &randomOrdering)
{
	random_shuffle(randomOrdering.begin(), randomOrdering.end());
	return randomOrdering;
}

double CalculationHelper::calculateStandardDeviation(vector<double> values)
{
	double mean = calculateAverage(values);
	double sum = 0.0;
	for (size_t i = 0; i < values.size(); i++)
	{
		double difference = values.at(0) - mean;
		sum += pow(difference, 2);
	}
	double variance = sum / (double)values.size();

	double standardDeviation = sqrt(variance);
	return standardDeviation;
}

double CalculationHelper::calculateAverage(vector<double> values)
{
	double sum = 0;
	for (size_t i = 0; i < values.size(); i++)
	{
		sum += values.at(i);
	}
	double mean = (double)sum / (double)values.size();
	return mean;
}

string CalculationHelper::getTimeString()
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, sizeof(buffer), "%d-%m-%Y-%H%M%S", timeinfo);
	string time(buffer);

	return time;
}