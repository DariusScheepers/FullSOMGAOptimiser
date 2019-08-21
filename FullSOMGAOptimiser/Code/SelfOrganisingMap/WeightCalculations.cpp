#include "WeightCalculations.hpp"

WeightCalculations::WeightCalculations(float selectedTrainingVectorWeight, Neuron * bestMatchingUnit,
        Neuron * currentNeuron, float currentWeight, 
        float newLearingRate, float newKernelWidth, int iterations)
{
    WeightCalculations::selectedTrainingVectorWeight = selectedTrainingVectorWeight;
    WeightCalculations::bestMatchingUnit = bestMatchingUnit;
    WeightCalculations::currentNeuron = currentNeuron;
    WeightCalculations::currentWeight = currentWeight;
    WeightCalculations::newLearingRate = newLearingRate;
    WeightCalculations::newKernelWidth = newKernelWidth;
    WeightCalculations::iterations = iterations;
}

WeightCalculations::~WeightCalculations()
{
}

float WeightCalculations::returnNewNeuronWeight()
{
    const float weightDelta = calculateWeightDelta();
    return currentWeight + weightDelta;
}

float WeightCalculations::calculateWeightDelta()
{
    const float differenceInTrainingVectorAndCurrentWeight = selectedTrainingVectorWeight - currentWeight;
    return neighbourhoodFunctionSmoothGaussianKernel() * differenceInTrainingVectorAndCurrentWeight;
}

float WeightCalculations::neighbourhoodFunctionSmoothGaussianKernel()
{
    vector<float> bmuCoordinates;
    vector<float> currentNeuronCoordinates;

    bmuCoordinates.push_back(bestMatchingUnit->getXCoordinate());
    bmuCoordinates.push_back(bestMatchingUnit->getYCoordinate());
    currentNeuronCoordinates.push_back(currentNeuron->getXCoordinate());
    currentNeuronCoordinates.push_back(currentNeuron->getYCoordinate());
    CalculationHelper calculations;

    const float upperBracketValue = pow(calculations.euclidianDistance(bmuCoordinates, currentNeuronCoordinates), 2.0);
    const float lowerBracketValue = 2.0 * pow(newKernelWidth, 2.0);
    const float wholeBrackerValue = -1.0 * (upperBracketValue / lowerBracketValue);
    const float exponentialValue = exp(wholeBrackerValue);
	const float result = newLearingRate * exponentialValue;
    return result;
}