#ifndef NEURON_H
#define NEURON_H

#include <vector>
using namespace std;

class Neuron
{
    private:
        vector<double> weights;
        double xCoordinate;
        double yCoordinate;
        vector<double> coordinatesVector;
    public:
        Neuron(vector<double>, double, double);
        ~Neuron();

        vector<double> getWeights();
        double getWeightAt(int index);
        void setWeight(int, double);
        double getXCoordinate();
        double getYCoordinate();
        void setAllWeights(vector<double>);
        vector<double> getCoordinatesVector();
};

#endif
