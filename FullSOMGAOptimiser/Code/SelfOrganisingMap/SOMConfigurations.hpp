#ifndef SOM_CONFIGURATIONS_H
#define SOM_CONFIGURATIONS_H

#define usint unsigned short int
#define matrix vector<vector<float>>
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
		inputVectors trainingSet;
		int slidingWindowOffset;
		float stoppingCriteriaThreshhold;
		Writer * writer;

		InputVector * topLeftTrainingVector;
		InputVector * bottomRightTrainingVector;
		InputVector * bottomLeftTrainingVector;
		InputVector * topRightTrainingVector;

		inputVectors convertMatrixToInputVectors(matrix);
        void createTrainingSet();
		void findCornerVectors();
		vector<InputVector*> findTopLeftAndBottomRightTrainingVectors();
		InputVector * findBottomLeftTrainingVector(InputVector *, InputVector *);
		InputVector * findTopRightTrainingVector(InputVector *, InputVector *, InputVector *);

    public:
        SOMConfigurations(int, int, matrix, int, float, CalculationHelper *, Writer *);
        ~SOMConfigurations();

		CalculationHelper * calculations;
		void runDataPreperations();
        int getMaxEpochs();
        matrix getDataSet();
        int getTrainingSetPortion();
		inputVectors getTrainingSet();
		InputVector * getTrainingVectorAt(int);
		InputVector * getCornerVectorAt(cornerVectors);
		float getStoppingCriteriaThreshhold();
		int getSlidingWindowOffset();
		InputVector * sliceInputVectorAtIndex(int);
		void addTrainingVector(InputVector *);

		Writer * getWriter();
};

#endif