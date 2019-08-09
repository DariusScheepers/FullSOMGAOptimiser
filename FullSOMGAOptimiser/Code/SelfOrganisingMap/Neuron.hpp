#ifndef NEURON_H
#define NEURON_H

#include <vector>
using namespace std;

class Neuron
{
    private:
        vector<float> weights;
        float xCoordinate;
        float yCoordinate;
    public:
        Neuron(vector<float>, float, float);
        ~Neuron();

        vector<float> getWeights();
        float getWeightAt(int index);
        void setWeight(int, float);
        float getXCoordinate();
        float getYCoordinate();
        void setAllWeights(vector<float>);
};

#endif
