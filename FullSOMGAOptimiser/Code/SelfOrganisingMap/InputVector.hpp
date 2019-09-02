#ifndef INPUT_VECTOR_H
#define INPUT_VECTOR_H

#include <vector>
using namespace std;

class InputVector
{
    private:
        vector<double> inputValues;
    public:
        InputVector(vector<double> inputValues);
        ~InputVector();

        double getInputValueAt(int index);
        vector<double> getInputValues();
};

#endif
