#include "Neuron.hpp"

Neuron::Neuron(vector<double> weights, double xCoordinate, double yCoordinate)
{
    Neuron::weights = weights;
    Neuron::xCoordinate = xCoordinate;
    Neuron::yCoordinate = yCoordinate;
}
Neuron::~Neuron()
{
	weights.clear();
	weights.shrink_to_fit();
}

vector<double> Neuron::getWeights()
{
    return weights;
}

double Neuron::getWeightAt(int index)
{
    return weights.at(index);
}

void Neuron::setWeight(int index, double weight)
{
    weights.at(index) = weight;
}

double Neuron::getXCoordinate()
{
    return xCoordinate;
}

double Neuron::getYCoordinate()
{
    return yCoordinate;
}

void Neuron::setAllWeights(vector<double> weights)
{
    Neuron::weights = weights;
}