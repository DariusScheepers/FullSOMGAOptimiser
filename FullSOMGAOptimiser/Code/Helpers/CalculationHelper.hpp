#ifndef CALCULATION_HELPER_H
#define CALCULATION_HELPER_H

#include <vector>
#include <ctime>
#include <random>
#include <cstdlib>
#include "math.h"
using namespace std;

#define dataMatrix vector<vector<float>>

class CalculationHelper
{
    private:
        /* data */
		static float normaliseValue(float, float, float);
		mt19937 generator;
    public:
        CalculationHelper(/* args */);
        ~CalculationHelper();

        static vector<float> differenceBetweenVectors(vector<float>, vector<float>);
        static float magnitudeOfVector(vector<float>);
        static float magintudeOfDifferenceBetweenVectors(vector<float>, vector<float>);
        static vector<float> sumOfVectors(vector<float>, vector<float>);
        static float euclidianDistance(vector<float>, vector<float>);
        static vector<float> scalarTimesVector(float, vector<float>);
        static float percentageToFloat(float);
		static dataMatrix normaliseDataSet(dataMatrix);
		float getRandomFloat(float, float);
		int getRandomInt(int, int);
		float getRandomNormalDistributionFloat(float, float);
		vector<float> randomShuffleFloat(vector<float>&);
		vector<vector<float>> randomShuffleVectors(vector<vector<float>>&);
};

#endif