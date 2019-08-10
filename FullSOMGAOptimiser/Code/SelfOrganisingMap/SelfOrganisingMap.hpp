#ifndef SELF_ORGANISING_MAP_H
#define SELF_ORGANISING_MAP_H

#include <vector>
#include <random>
#include <iostream>
#include "Neuron.hpp"
#include "InputVector.hpp"
#include "math.h"
#include "SOMConfigurations.hpp"
#include "WeightCalculations.hpp"
#include "../Helpers/CalculationHelper.hpp"
using namespace std;

#define usint unsigned short int
#define neuronMatrix vector<vector<Neuron*>>

class SelfOrganisingMap
{
    private:
        usint rows;
        usint columns;
        float learningRate;
        float learningDecay;
        float kernelWidth;
        float kernelDecay;
        SOMConfigurations * configurations;

        // important global variables
        neuronMatrix neuronMap;
               
        // important global variables
        float newLearningRate;
        float newKernelWidth;
        usint iteration;

        void createNeuronMap();
        InputVector * selectTrainingVector();
        Neuron * getBestMatchingUnit(InputVector*);
        void updateEachNeuronWeights(InputVector *, Neuron *);
        void setNewLearningRateAndKernelWidth(int);
        float calculateExponenialDecay(float intialValue, int iteration, float decayConstant);

        void performHypercubeWeightInitialization();
        vector<float> adjustedWeightByHypercube(vector<float>, vector<float>, int, int);

		void printNeuronMap();
		void printInitialNeuronMap();
		void printQuantizationError();
		void printEndNeuronMap();
       
    public:
        SelfOrganisingMap(
			SOMConfigurations*,
			usint,
			usint,
			float,
			float,
			float, 
			float
		);
        ~SelfOrganisingMap();

        void runSelfOrganisingMap();
        float calculateQuantizationError();
};

#endif