#ifndef INPUT_VECTOR_H
#define INPUT_VECTOR_H

#include <vector>
using namespace std;

class InputVector
{
    private:
        vector<float> inputValues;
    public:
        InputVector(vector<float> inputValues);
        ~InputVector();

        float getInputValueAt(int index);
        vector<float> getInputValues();
};

#endif
