#ifndef CALCULATION_HELPER_H
#define CALCULATION_HELPER_H

#include <vector>
#include <ctime>
#include <random>
#include <cstdlib>
#include "math.h"
using namespace std;

#define dataMatrix vector<vector<double>>

class CalculationHelper
{
    private:
        /* data */
		static double normaliseValue(double, double, double);
		mt19937 generator;
    public:
        CalculationHelper(/* args */);
        ~CalculationHelper();

        static vector<double> differenceBetweenVectors(vector<double>, vector<double>);
        static double magnitudeOfVector(vector<double>);
        static double magintudeOfDifferenceBetweenVectors(vector<double>, vector<double>);
        static vector<double> sumOfVectors(vector<double>, vector<double>);
        static double euclidianDistance(vector<double>, vector<double>);
        static vector<double> scalarTimesVector(double, vector<double>);
        static double percentageToDouble(double);
		static dataMatrix normaliseDataSet(dataMatrix);
		double getRandomDouble(double, double);
		int getRandomInt(int, int);
		double getRandomNormalDistributionDouble(double, double);
		vector<double> randomShuffleDouble(vector<double>&);
		vector<vector<double>> randomShuffleVectors(vector<vector<double>>&);
};

#endif