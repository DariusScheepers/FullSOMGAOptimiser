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
#define neuronMatrix vector<vector<Neuron*>>

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

        // important global variables
        neuronMatrix neuronMap;
		inputVectors trainingSet;
		inputVectors testSet;
               
        // important global variables
        double newLearningRate;
        double newKernelWidth;
        usint iteration;

		void deleteNeuronMap();
        void createNeuronMap();
		void trainSOM();
		void prepareTrainingSet(int offset);
        InputVector * selectTrainingVector();
        Neuron * getBestMatchingUnit(InputVector *);
        void updateEachNeuronWeights(InputVector *, Neuron *);
        void setNewLearningRateAndKernelWidth(int);
        double calculateExponenialDecay(double intialValue, int iteration, double decayConstant);

        void performHypercubeWeightInitialization();
        vector<double> adjustedWeightByHypercube(vector<double>, vector<double>, int, int);
		double getTestRunAverageQuantizationError();
		double calculateQuantizationError();
		double calculateStandardDeviationOfQE(int slidingWindowSize);
		double calculateDecreaseInQE(int slidingWindowSize);
		void addToQEHistory(double, int);
		vector<double> quantizationErrorHistory;
		vector<double> testSetQEHistory;

		void printNeuronMap();
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
};

#endif