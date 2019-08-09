#ifndef WEIGHT_CALCULATIONS_H
#define WEIGHT_CALCULATIONS_H

#include "InputVector.hpp"
#include "Neuron.hpp"
#include "math.h"
#include "../Helpers/CalculationHelper.hpp"
using namespace std;

class WeightCalculations
{
    private:
        float selectedTrainingVectorWeight;
        Neuron * bestMatchingUnit;
        Neuron * currentNeuron;
        float currentWeight;
        float newLearingRate;
        float newKernelWidth;
        int iterations;

        float calculateWeightDelta();
        float neighbourhoodFunctionSmoothGaussianKernel();
    public:
        WeightCalculations(float selectedTrainingVectorWeight, Neuron * bestMatchingUnit, Neuron * currentNeuron, float currentWeight, float, float, int);
        ~WeightCalculations();

        float returnNewNeuronWeight();
};

#endif