#include "InputVector.hpp"

InputVector::InputVector(vector<double> inputValues)
{
	InputVector::inputValues = inputValues;
}

InputVector::~InputVector()
{
	inputValues.clear();
	inputValues.shrink_to_fit();
}

double InputVector::getInputValueAt(int index)
{
    return inputValues.at(index);
}

vector<double> InputVector::getInputValues()
{
    return inputValues;
}