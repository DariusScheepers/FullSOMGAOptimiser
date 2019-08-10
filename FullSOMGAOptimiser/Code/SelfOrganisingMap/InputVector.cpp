#include "InputVector.hpp"

InputVector::InputVector(vector<float> inputValues)
{
	InputVector::inputValues = inputValues;
}

InputVector::~InputVector()
{
	inputValues.clear();
	inputValues.shrink_to_fit();
}

float InputVector::getInputValueAt(int index)
{
    return inputValues.at(index);
}

vector<float> InputVector::getInputValues()
{
    return inputValues;
}