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
class SOMConfigurations
{
    private:
        int maxEpochs;
        int trainingSetSize;
        Matrix dataSet;
		InputVectors input;
		string dataSetName;
		int crossValidationNumber;

		int slidingWindowOffset;
		double stoppingCriteriaThreshhold;
		Writer * writer;

		InputVectors convertMatrixToInputVectors(Matrix);
        void shuffleAndFormatData();

    public:
        SOMConfigurations(
			int maxEpochs,
			int trainingSetSize,
			Matrix dataSet,
			int slidingWindowOffset,
			int crossValidationNumber,
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
        Matrix getDataSet();
		InputVectors getInput();
		InputVector * getTrainingVectorAt(int);
		double getStoppingCriteriaThreshhold();
		int getSlidingWindowOffset();
		int getTrainingSetSize();
		int getCrossValidationNumber();
		string getDataSetName();
		bool fullOutput;

		Writer * getWriter();
};

#endif