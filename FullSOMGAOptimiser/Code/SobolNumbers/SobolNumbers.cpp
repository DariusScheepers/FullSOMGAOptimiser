#include "SobolNumbers.h"

SobolNumbers::SobolNumbers(
    SOMConfigurations * targetExperimentConfigurations,
    vector<vector<double>> sobolNumberSequences,
    vector<vector<double>> parameterValueRanges,
    vector<vector<bool>> parameterValueRangesInclusiveOrExclusive
    )
{
    SobolNumbers::targetExperimentConfigurations = targetExperimentConfigurations;
    SobolNumbers::sobolNumberSequences = sobolNumberSequences;
    SobolNumbers::parameterValueRanges = parameterValueRanges;
    SobolNumbers::parameterValueRangesInclusiveOrExclusive = parameterValueRangesInclusiveOrExclusive;

    prepareParameters();
}

SobolNumbers::~SobolNumbers()
{
    delete bestSOM;
}

void SobolNumbers::runSobolNumberSequence()
{
    bestSOMFitness = numeric_limits<double>::max();
	iteration = 0;
	bestSOM = NULL;
    for (vector<double> sobolNumberSequence : sobolNumberSequences)
    {
		cout << "At iteration: " << iteration++ << " with best " << to_string(bestSOMFitness) << endl;
        vector<double> parameters = normaliseParameters(sobolNumberSequence);
        SelfOrganisingMap * selfOrganisingMap = createSOM(parameters);
        selfOrganisingMap->runNFoldCrossValidation(targetExperimentConfigurations->getCrossValidationNumber());

        const double fitnessValue = selfOrganisingMap->calculatePerformance();
        if (fitnessValue < bestSOMFitness)
        {
            bestSOMFitness = fitnessValue;
			if (bestSOM != NULL)
			{
				delete bestSOM;
			}
            bestSOM = selfOrganisingMap;
        }
        else 
        {
            delete selfOrganisingMap;
        }
    }
}

SelfOrganisingMap * SobolNumbers::returnBestSOM()
{
    return bestSOM;
}

void SobolNumbers::prepareParameters()
{
    setExactRangesOnParameters();
    perpareDimensionsAsSqrtOfInputDataSize();
}

void SobolNumbers::setExactRangesOnParameters()
{
    for (size_t i = 0; i < parameterValueRanges.size(); i++)
    {
		const bool openingInclusive = parameterValueRangesInclusiveOrExclusive.at(i)[0];
        if (!openingInclusive) {
			const double currentparameterValue = parameterValueRanges.at(i)[0];
            parameterValueRanges.at(i)[0] = currentparameterValue + numeric_limits<double>::min();
        }
		const bool closingInclusive = parameterValueRangesInclusiveOrExclusive.at(i)[1];
		if (!closingInclusive) {
			const double currentparameterValue = parameterValueRanges.at(i)[1];
            parameterValueRanges.at(i)[1] = currentparameterValue - numeric_limits<double>::min();
        }
    }
}

void SobolNumbers::perpareDimensionsAsSqrtOfInputDataSize()
{
	const double max = sqrt(targetExperimentConfigurations->getInput().size());
	const int maxTrunc = trunc(max);
	const double addedToMaxForRounding = 0.5 - 0.0000000001; // numeric_limits<double>::min(); // 0.0000000001
	const double addedToMinForRounding = 0.5;

	const double fullMax = maxTrunc + addedToMaxForRounding;
	const double fullMin = parameterValueRanges.at(0).at(0) - addedToMinForRounding;
	vector<double> dimension;
	dimension.push_back(fullMin);
	dimension.push_back(fullMax);

	parameterValueRanges.at(0) = dimension;
	parameterValueRanges.at(1) = dimension;
}

vector<double> SobolNumbers::normaliseParameters(vector<double> sobolNumbers)
{
    vector<double> newParameters;
    for (size_t i = 0; i < sobolNumbers.size(); i++)
    {
        const vector<double> parameterValueMinAndMax = parameterValueRanges.at(i);
        const double sobolNumber = sobolNumbers.at(i);
        const double normalisedParameter = targetExperimentConfigurations->calculations->minMaxNormalise(
            sobolNumber,
            0,
            1,
            parameterValueMinAndMax.at(0),
            parameterValueMinAndMax.at(1)
        );
        newParameters.push_back(normalisedParameter);
    }
    return newParameters;
}

SelfOrganisingMap * SobolNumbers::createSOM(vector<double> parameters)
{    
	const unsigned short int rows = round(parameters.at(0));
	const unsigned short int columns = round(parameters.at(1));
	const double learningRate = parameters.at(2);
	const double learningDecay = parameters.at(3);
	const double kernelWidth = parameters.at(4);	
	const double kernelDecay = parameters.at(5);
    SelfOrganisingMap * som = new SelfOrganisingMap(
		targetExperimentConfigurations,
		rows,
		columns,
		learningRate,
        learningDecay,
        kernelWidth,
        kernelDecay
    );

    return som;
}