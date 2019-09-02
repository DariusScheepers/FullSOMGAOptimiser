#include "WeightCalculations.hpp"

WeightCalculations::WeightCalculations(double selectedTrainingVectorWeight, Neuron * bestMatchingUnit,
        Neuron * currentNeuron, double currentWeight, 
        double newLearingRate, double newKernelWidth)
{
    WeightCalculations::selectedTrainingVectorWeight = selectedTrainingVectorWeight;
    WeightCalculations::bestMatchingUnit = bestMatchingUnit;
    WeightCalculations::currentNeuron = currentNeuron;
    WeightCalculations::currentWeight = currentWeight;
    WeightCalculations::newLearingRate = newLearingRate;
    WeightCalculations::newKernelWidth = newKernelWidth;
}

WeightCalculations::~WeightCalculations()
{
}

double WeightCalculations::returnNewNeuronWeight()
{
    const double weightDelta = calculateWeightDelta();
	const double result = currentWeight + weightDelta;
	return result;
}

double WeightCalculations::calculateWeightDelta()
{
	const double differenceInTrainingVectorAndCurrentWeight = selectedTrainingVectorWeight - currentWeight;
	const double gaussianKernel = neighbourhoodFunctionSmoothGaussianKernel();
	// cout << gaussianKernel << "|" << endl;
	const double result = gaussianKernel * differenceInTrainingVectorAndCurrentWeight;
	return result;
}

double WeightCalculations::neighbourhoodFunctionSmoothGaussianKernel()
{
    vector<double> bmuCoordinates;
    vector<double> currentNeuronCoordinates;

    bmuCoordinates.push_back(bestMatchingUnit->getXCoordinate());
    bmuCoordinates.push_back(bestMatchingUnit->getYCoordinate());
    currentNeuronCoordinates.push_back(currentNeuron->getXCoordinate());
    currentNeuronCoordinates.push_back(currentNeuron->getYCoordinate());
    CalculationHelper calculations;

    const double upperBracketValue = pow(calculations.euclidianDistance(bmuCoordinates, currentNeuronCoordinates), 2.0);
    const double lowerBracketValue = 2.0 * pow(newKernelWidth, 2.0);
	if (lowerBracketValue <= 0.0 || isnan(lowerBracketValue))
	{
		return 0.0;
	}
    const double wholeBrackerValue = -1.0 * (upperBracketValue / lowerBracketValue);
    const double exponentialValue = exp(wholeBrackerValue);
	const double result = newLearingRate * exponentialValue;
    return result;
}