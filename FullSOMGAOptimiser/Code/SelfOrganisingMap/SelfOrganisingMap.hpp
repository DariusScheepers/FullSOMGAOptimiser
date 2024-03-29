#ifndef SELF_ORGANISING_MAP_H
#define SELF_ORGANISING_MAP_H

#include <vector>
#include <random>
#include <string>
#include <iostream>
#include "Neuron.hpp"
#include "InputVector.hpp"
#include "math.h"
#include "SOMConfigurations.hpp"
#include "WeightCalculations.hpp"
#include "../Helpers/CalculationHelper.hpp"
#include "../Writers/Writer.h"
using namespace std;

#define usint unsigned short int
#define NeuronMatrix vector<vector<Neuron*>>

enum class CornerVectors {
	topLeft,
	bottomRight,
	bottomLeft,
	topRight
};

class SelfOrganisingMap
{
    private:
        usint rows;
        usint columns;
        double learningRate;
        double learningDecay;
		double kernelWidth;
		double kernelWidthPortion;
        double kernelDecay;
        SOMConfigurations * configurations;

        NeuronMatrix neuronMap;
		InputVectors trainingSet;
		InputVectors testSet;
        dataMatrix trainingSetsQEHistory;
        dataMatrix testSetsQEHistory;
               
        double newLearningRate;
        double newKernelWidth;
        usint iteration;
        usint trainingWindowIteration;

		InputVector * topLeftTrainingVector;
		InputVector * bottomRightTrainingVector;
		InputVector * bottomLeftTrainingVector;
		InputVector * topRightTrainingVector;
		void findCornerVectors();
		vector<InputVector*> findTopLeftAndBottomRightTrainingVectors();
		InputVector * findBottomLeftTrainingVector(InputVector *, InputVector *);
		InputVector * findTopRightTrainingVector(InputVector *, InputVector *, InputVector *);
		void setKernelWidth();

		void deleteNeuronMap();
        void createNeuronMap();
		void trainSOM();
		void handleCurrentQEInfo(double, vector<string>&);
        void addToTrainingSetsQEHistory();
		void prepareTrainingSet(int offset, int crossFoldValidationNumber);
        InputVector * selectTrainingVector();
		void shuffleTrainingSet();
        Neuron * getBestMatchingUnit(InputVector *);
        void updateEachNeuronWeights(InputVector *, Neuron *);
        void setNewLearningRateAndKernelWidth();
        double calculateExponentialDecay(double intialValue, int iteration, double decayConstant);

        void performHypercubeWeightInitialization();
        vector<double> adjustedWeightByHypercube(vector<double>, vector<double>, int, int);
		double getTestRunAverageQuantizationError();
		double calculateQuantizationError();
		double calculateStandardDeviationOfQE(int slidingWindowSize);
		double calculateDecreaseInQE(int slidingWindowSize);
		vector<double> quantizationErrorHistory;
        vector<double> getLastWindowedQEHistory(int);
		vector<double> testSetQEHistory;
		vector<double> lineariseDataMatrix(dataMatrix);
		double overAllQEHistoryAverage(dataMatrix);
		double overAllQEHistoryStandardDeviation(dataMatrix);
		int overAllQEHistoryIterations(dataMatrix);

		string printNeuronMap();
		void printInitialNeuronMap();
		void printQuantizationError(int);
		void printEndNeuronMap();
       
    public:
        SelfOrganisingMap(
			SOMConfigurations*,
			usint,
			usint,
			double,
			double,
			double, 
			double
		);
        ~SelfOrganisingMap();

        void runSelfOrganisingMap();
        void runNFoldCrossValidation(int);
		double calculatePerformance();
        void printTrainingAndTestSetsQEHistories();
		void printFinalNeuronMap();
		string parametersToString();
};

#endif