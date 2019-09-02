#ifndef WEIGHT_CALCULATIONS_H
#define WEIGHT_CALCULATIONS_H

#include "InputVector.hpp"
#include "Neuron.hpp"
#include "math.h"
#include "../Helpers/CalculationHelper.hpp"
#include <iostream>
using namespace std;

class WeightCalculations
{
    private:
        double selectedTrainingVectorWeight;
        Neuron * bestMatchingUnit;
        Neuron * currentNeuron;
        double currentWeight;
        double newLearingRate;
        double newKernelWidth;

        double calculateWeightDelta();
        double neighbourhoodFunctionSmoothGaussianKernel();
    public:
        WeightCalculations(double selectedTrainingVectorWeight, Neuron * bestMatchingUnit, Neuron * currentNeuron, double currentWeight, double, double);
        ~WeightCalculations();

        double returnNewNeuronWeight();
};

#endif