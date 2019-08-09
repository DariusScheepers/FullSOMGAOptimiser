#include "InputVector.hpp"

InputVector::InputVector(vector<float> inputValues)
{
	InputVector::inputValues = inputValues;
}

InputVector::~InputVector()
{
}

float InputVector::getInputValueAt(int index)
{
    return inputValues.at(index);
}

vector<float> InputVector::getInputValues()
{
    return inputValues;
}