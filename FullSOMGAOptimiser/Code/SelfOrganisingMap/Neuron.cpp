#include "Neuron.hpp"

Neuron::Neuron(vector<float> weights, float xCoordinate, float yCoordinate)
{
    Neuron::weights = weights;
    Neuron::xCoordinate = xCoordinate;
    Neuron::yCoordinate = yCoordinate;
}

vector<float> Neuron::getWeights()
{
    return weights;
}

float Neuron::getWeightAt(int index)
{
    return weights.at(index);
}

void Neuron::setWeight(int index, float weight)
{
    weights.at(index) = weight;
}

float Neuron::getXCoordinate()
{
    return xCoordinate;
}

float Neuron::getYCoordinate()
{
    return yCoordinate;
}

void Neuron::setAllWeights(vector<float> weights)
{
    Neuron::weights = weights;
}