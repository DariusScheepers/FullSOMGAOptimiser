// FullSOMGAOptimiser.cpp : Defines the entry point for the application.
//

#include "FullSOMGAOptimiser.h"

CalculationHelper * calculations;

ReadInput * reader;
Writer * writer;

int getSOMConfigIndex(somConfigurations somConfigValue)
{
	switch (somConfigValue)
	{
		case somConfigurations::dataSet:
			return 0;
		case somConfigurations::dataSeperator:
			return 1;
		case somConfigurations::maximumTrainingIterations:
			return 2;
		case somConfigurations::traningSetPortion:
			return 3;
		case somConfigurations::slidingWindowOffset:
			return 4;
		case somConfigurations::stoppingCriteriaThreshhold:
			return 5;
		case somConfigurations::defaultRows:
			return 6;
		case somConfigurations::defaultColumns:
			return 7;
		case somConfigurations::defaultLearningRate:
			return 8;
		case somConfigurations::defaultKernelWidth:
			return 9;
		case somConfigurations::defaultLearningRateDecay:
			return 10;
		case somConfigurations::defaultKernelWidthDecay:
			return 11;
	default:
		break;
	}
}

int getGAConfigIndex(gaConfigurations gaConfigValue)
{
	switch (gaConfigValue)
	{
		case gaConfigurations::chromosomePopulationSize:
			return 0;
		case gaConfigurations::iterations:
			return 1;
		case gaConfigurations::mutationProbability:
			return 2;
		case gaConfigurations::selectionCutOffSize:
			return 3;
		case gaConfigurations::crossOverSplit:
			return 4;
		case gaConfigurations::mutationStandardDeviation:
			return 5;
	default:
		break;
	}
}

int getGAGeneConfigIndex(gaGenesConfigurations gaGeneConfigValue)
{
	switch (gaGeneConfigValue)
	{
		case gaGenesConfigurations::rows:
			return 0;
		case gaGenesConfigurations::columns:
			return 1;
		case gaGenesConfigurations::learningRate:
			return 2;
		case gaGenesConfigurations::learningRateDecay:
			return 3;
		case gaGenesConfigurations::kernelWidth:
			return 4;
		case gaGenesConfigurations::kernelWidthDecay:
			return 5;
	default:
		break;
	}
}

bool generateSingletons(int argc, char ** argv)
{
	if (argc < 2)
	{
		cout << "ERROR: No path to main working directory is provided. Exiting...\n";
		return false;
	}

	calculations = new CalculationHelper();
	writer = new Writer(argv[1]);
	reader = new ReadInput(argv[1]);
}

SOMConfigurations * getSOMConfigurations(vector<string> values)
{
	const char seperator = values.at(getSOMConfigIndex(somConfigurations::dataSeperator)).at(0);
	vector<vector<float>> dataSet = reader->readDataSet(values.at(getSOMConfigIndex(somConfigurations::dataSet)), seperator);
	const int maxEpochs = stoi(values.at(getSOMConfigIndex(somConfigurations::maximumTrainingIterations)));
	const int trainingSetPortion = stoi(values.at(getSOMConfigIndex(somConfigurations::traningSetPortion)));
	const int slidingWindowOffset = stoi(values.at(getSOMConfigIndex(somConfigurations::slidingWindowOffset)));
	const float stoppingCriteriaThreshhold = stof(values.at(getSOMConfigIndex(somConfigurations::stoppingCriteriaThreshhold)));

	return new SOMConfigurations(maxEpochs,
		trainingSetPortion,
		dataSet,
		slidingWindowOffset,
		stoppingCriteriaThreshhold,
		calculations,
		writer
	);
}

SelfOrganisingMap * getSelfOrganisingMap(vector<string> values)
{
	SOMConfigurations * somConfigurations = getSOMConfigurations(values);
	somConfigurations->runDataPreperations();
	const int rows = stoi(values.at(getSOMConfigIndex(somConfigurations::defaultRows)));
	const int columns = stoi(values.at(getSOMConfigIndex(somConfigurations::defaultColumns)));
	const float learningRate = stof(values.at(getSOMConfigIndex(somConfigurations::defaultLearningRate)));
	const float learningDecay = stof(values.at(getSOMConfigIndex(somConfigurations::defaultLearningRateDecay)));
	const float kernelWidth = stof(values.at(getSOMConfigIndex(somConfigurations::defaultKernelWidth)));
	const float kernelDecay = stof(values.at(getSOMConfigIndex(somConfigurations::defaultKernelWidthDecay)));

	return new SelfOrganisingMap(
		somConfigurations,
		rows,
		columns,
		learningRate,
		learningDecay,
		kernelWidth,
		kernelDecay
	);
}

GeneticAlgorithm * getGeneticAlgorithm(vector<string> somConfigValues)
{
	vector<string> gaConfigurationFileValues = reader->readGAConfig();
	GeneRanges * gaGenesConfigurationFileValues = reader->readGAGenesConfig();

	unsigned int chromosomePopulationSize = stoi(gaConfigurationFileValues.at(getGAConfigIndex(gaConfigurations::chromosomePopulationSize)));
	unsigned int iterations = stoi(gaConfigurationFileValues.at(getGAConfigIndex(gaConfigurations::iterations)));
	vector<vector<float>> gaGeneRanges = gaGenesConfigurationFileValues->getRangesValues();
	vector<vector<bool>> gaGeneInclusives = gaGenesConfigurationFileValues->getRangesInclusive();
	unsigned short int mutationProbability = stoi(gaConfigurationFileValues.at(getGAConfigIndex(gaConfigurations::mutationProbability)));
	unsigned short int selectionCutOffSize = stoi(gaConfigurationFileValues.at(getGAConfigIndex(gaConfigurations::selectionCutOffSize)));
	unsigned short int crossOverSplit = stoi(gaConfigurationFileValues.at(getGAConfigIndex(gaConfigurations::crossOverSplit)));
	unsigned short int mutationStandardDeviation = stoi(gaConfigurationFileValues.at(getGAConfigIndex(gaConfigurations::mutationStandardDeviation)));
	SOMConfigurations * somConfigurationsValues = getSOMConfigurations(somConfigValues);

	delete gaGenesConfigurationFileValues;

	GAConfigurations * gaConfiguration = new GAConfigurations(
		chromosomePopulationSize,
		iterations,
		gaGeneRanges,
		gaGeneInclusives,
		mutationProbability,
		selectionCutOffSize,
		crossOverSplit,
		mutationStandardDeviation,
		somConfigurationsValues,
		calculations,
		writer
	);

	return new GeneticAlgorithm(gaConfiguration);
}

int main(int argc, char ** argv)
{
	generateSingletons(argc, argv);

	vector<string> somConfigurationFileValues = reader->readSOMConfig();
	vector<string> arguments = reader->readArguments();
	if (arguments.at(0) == "0")
	{
		SelfOrganisingMap * selfOrganisingMap = getSelfOrganisingMap(somConfigurationFileValues);
		selfOrganisingMap->runSelfOrganisingMap();
		delete selfOrganisingMap;
	}
	else
	{
		GeneticAlgorithm * geneticAlgorithm = getGeneticAlgorithm(somConfigurationFileValues);
		geneticAlgorithm->runGeneticAlgorithm();
		delete geneticAlgorithm;
	}

	cout << "Finished Project" << endl;
	// _CrtDumpMemoryLeaks();
	int a;
	cin >> a;
	return 0;
}
