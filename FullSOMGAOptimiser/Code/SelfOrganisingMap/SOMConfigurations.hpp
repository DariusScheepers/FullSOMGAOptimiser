#ifndef SOM_CONFIGURATIONS_H
#define SOM_CONFIGURATIONS_H

#define usint unsigned short int
#define matrix vector<vector<double>>
#define inputVectors vector<InputVector *>

#include <vector>
#include <algorithm>
#include "../Helpers/CalculationHelper.hpp"
#include "../Writers/Writer.h"
#include "InputVector.hpp"
using namespace std;

enum class cornerVectors {
	topLeft,
	bottomRight,
	bottomLeft,
	topRight
};

class SOMConfigurations
{
    private:
        int maxEpochs;
        int trainingSetPortion;
        matrix dataSet;
		inputVectors input;

		int slidingWindowOffset;
		double stoppingCriteriaThreshhold;
		Writer * writer;

		InputVector * topLeftTrainingVector;
		InputVector * bottomRightTrainingVector;
		InputVector * bottomLeftTrainingVector;
		InputVector * topRightTrainingVector;

		inputVectors convertMatrixToInputVectors(matrix);
        void createTrainingAndTestSet();
		void findCornerVectors();
		vector<InputVector*> findTopLeftAndBottomRightTrainingVectors();
		InputVector * findBottomLeftTrainingVector(InputVector *, InputVector *);
		InputVector * findTopRightTrainingVector(InputVector *, InputVector *, InputVector *);

    public:
        SOMConfigurations(int, int, matrix, int, double, CalculationHelper *, Writer *);
        ~SOMConfigurations();

		CalculationHelper * calculations;
		void runDataPreperations();
		void shuffleDataSet();
        int getMaxEpochs();
		int getTrainingSetPortion();
        matrix getDataSet();
		inputVectors getInput();
		InputVector * getTrainingVectorAt(int);
		InputVector * getCornerVectorAt(cornerVectors);
		double getStoppingCriteriaThreshhold();
		int getSlidingWindowOffset();

		Writer * getWriter();
};

#endif