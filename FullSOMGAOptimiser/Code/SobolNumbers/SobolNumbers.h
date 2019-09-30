#pragma once
#include <vector>
#include <string>
using namespace std;

#include "../SelfOrganisingMap/SOMConfigurations.hpp"
#include "../SelfOrganisingMap/SelfOrganisingMap.hpp"

class SobolNumbers
{
private:
    vector<vector<double>> sobolNumberSequences;
    SOMConfigurations * targetExperimentConfigurations;
	vector<vector<double>> parameterValueRanges;
    vector<vector<bool>> parameterValueRangesInclusiveOrExclusive;

    int iteration;
    vector<double> bestSobolNumberSequence;
    SelfOrganisingMap * bestSOM;
    double bestSOMFitness;
    
    void prepareParameters();
    void perpareDimensionsAsSqrtOfInputDataSize();
    void setExactRangesOnParameters();

    vector<double> normaliseParameters(vector<double>);
    SelfOrganisingMap * createSOM(vector<double>);
public:
    SobolNumbers(
        SOMConfigurations * targetExperimentConfigurations,
        vector<vector<double>> sobolNumberSequences,
        vector<vector<double>> parameterValueRanges,
        vector<vector<bool>> parameterValueRangesInclusiveOrExclusive
    );
    ~SobolNumbers();
    
    void runSobolNumberSequence();
    SelfOrganisingMap * returnBestSOM();
};