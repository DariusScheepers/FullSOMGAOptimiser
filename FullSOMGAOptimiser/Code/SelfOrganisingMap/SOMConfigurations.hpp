#ifndef SOM_CONFIGURATIONS_H
#define SOM_CONFIGURATIONS_H

#define usint unsigned short int
#define Matrix vector<vector<double>>
#define InputVectors vector<InputVector *>

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
        Matrix dataSet;
		InputVectors input;
		string dataSetName;

		int slidingWindowOffset;
		double stoppingCriteriaThreshhold;
		Writer * writer;

		InputVector * topLeftTrainingVector;
		InputVector * bottomRightTrainingVector;
		InputVector * bottomLeftTrainingVector;
		InputVector * topRightTrainingVector;

		InputVectors convertMatrixToInputVectors(Matrix);
        void createTrainingAndTestSet();
		void findCornerVectors();
		vector<InputVector*> findTopLeftAndBottomRightTrainingVectors();
		InputVector * findBottomLeftTrainingVector(InputVector *, InputVector *);
		InputVector * findTopRightTrainingVector(InputVector *, InputVector *, InputVector *);

    public:
        SOMConfigurations(
			int maxEpochs,
			int trainingSetPortion,
			Matrix dataSet,
			int slidingWindowOffset,
			double stoppingCriteriaThreshhold,
			CalculationHelper * calculations,
			Writer * writer,
			string dataSetName,
			bool fullOutput
			);
        ~SOMConfigurations();

		CalculationHelper * calculations;
		void runDataPreperations();
		void shuffleDataSet();
        int getMaxEpochs();
		int getTrainingSetPortion();
        Matrix getDataSet();
		InputVectors getInput();
		InputVector * getTrainingVectorAt(int);
		InputVector * getCornerVectorAt(cornerVectors);
		double getStoppingCriteriaThreshhold();
		int getSlidingWindowOffset();
		string getDataSetName();
		bool fullOutput;

		Writer * getWriter();
};

#endif