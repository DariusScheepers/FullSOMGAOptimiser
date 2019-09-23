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

class SelfOrganisingMap
{
    private:
        usint rows;
        usint columns;
        double learningRate;
        double learningDecay;
        double kernelWidth;
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

		void deleteNeuronMap();
        void createNeuronMap();
		void trainSOM();
		void handleCurrentQEInfo(double, vector<string>&);
        void addToTrainingSetsQEHistory();
		void prepareTrainingSet(int offset);
        InputVector * selectTrainingVector();
		void shuffleTrainingSet();
        Neuron * getBestMatchingUnit(InputVector *);
        void updateEachNeuronWeights(InputVector *, Neuron *);
        void setNewLearningRateAndKernelWidth();
        double calculateExponenialDecay(double intialValue, int iteration, double decayConstant);

        void performHypercubeWeightInitialization();
        vector<double> adjustedWeightByHypercube(vector<double>, vector<double>, int, int);
		double getTestRunAverageQuantizationError();
		double calculateQuantizationError();
		double calculateStandardDeviationOfQE(int slidingWindowSize);
		double calculateDecreaseInQE(int slidingWindowSize);
		vector<double> quantizationErrorHistory;
        vector<double> getLastWindowedQEHistory(int);
		vector<double> testSetQEHistory;

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
		double calculatePerformance();
        void printTrainingAndTestSetsQEHistories();
		void printFinalNeuronMap();
		string parametersToString();
};

#endif