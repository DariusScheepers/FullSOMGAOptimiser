#ifndef CALCULATION_HELPER_H
#define CALCULATION_HELPER_H

#include <vector>
#include "math.h"
using namespace std;

#define dataMatrix vector<vector<float>>

class CalculationHelper
{
    private:
        /* data */
		static float normaliseValue(float, float, float);
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
};

#endif