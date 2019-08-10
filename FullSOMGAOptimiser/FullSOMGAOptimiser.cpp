// FullSOMGAOptimiser.cpp : Defines the entry point for the application.
//

#include "FullSOMGAOptimiser.h"

int getSOMConfigIndex(somConfigurations somConfigValue)
{
	switch (somConfigValue)
	{
		case somConfigurations::dataSet:
			return 0;
		case somConfigurations::dataSeperator:
			return 1;
		case somConfigurations::traningSetPortion:
			return 2;
		case somConfigurations::defaultMaximumTrainingIterations:
			return 3;
		case somConfigurations::defaultRows:
			return 4;
		case somConfigurations::defaultColumns:
			return 5;
		case somConfigurations::defaultLearningRate:
			return 6;
		case somConfigurations::defaultKernelWidth:
			return 7;
		case somConfigurations::defaultLearningRateDecay:
			return 8;
		case somConfigurations::defaultKernelWidthDecay:
			return 9;
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
		case gaConfigurations::genesAmount:
			return 2;
		case gaConfigurations::crossoverProbability:
			return 3;
		case gaConfigurations::mutationProbability:
			return 4;
		case gaConfigurations::selectionCutOffSize:
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
		case gaGenesConfigurations::trainingIterations:
			return 6;
	default:
		break;
	}
}

void printCommandLineArguments(int argc, char ** argv)
{
	cout << "You have entered " << argc
		<< " arguments:" << "\n";

	if (argc > 0)
	{
		cout << "Arguments:" << endl;
		for (int i = 0; i < argc; ++i) {
			cout << argv[i] << "\n";
		}
	}
}

SOMConfigurations * getSOMConfigurations(vector<string> values)
{
	ReadInput readInput;
	const char seperator = values.at(getSOMConfigIndex(somConfigurations::dataSeperator)).at(0);
	vector<vector<float>> dataSet = readInput.readDataSet(values.at(getSOMConfigIndex(somConfigurations::dataSet)), seperator);
	const int trainingSetPortion = stoi(values.at(getSOMConfigIndex(somConfigurations::traningSetPortion)));
	return new SOMConfigurations(trainingSetPortion, dataSet);
}

SelfOrganisingMap * getSelfOrganisingMap(vector<string> values)
{
	SOMConfigurations * somConfigurations = getSOMConfigurations(values);
	const int rows = stoi(values.at(getSOMConfigIndex(somConfigurations::defaultRows)));
	const int columns = stoi(values.at(getSOMConfigIndex(somConfigurations::defaultColumns)));
	const float learningRate = stof(values.at(getSOMConfigIndex(somConfigurations::defaultLearningRate)));
	const float learningDecay = stof(values.at(getSOMConfigIndex(somConfigurations::defaultLearningRateDecay)));
	const float kernelWidth = stof(values.at(getSOMConfigIndex(somConfigurations::defaultKernelWidth)));
	const float kernelDecay = stof(values.at(getSOMConfigIndex(somConfigurations::defaultKernelWidthDecay)));
	const int trainingIterations = stoi(values.at(getSOMConfigIndex(somConfigurations::defaultMaximumTrainingIterations)));

	return new SelfOrganisingMap(
		somConfigurations,
		rows,
		columns,
		learningRate,
		learningDecay,
		kernelWidth,
		kernelDecay,
		trainingIterations
	);
}

GeneticAlgorithm * getGeneticAlgorithm(vector<string> somConfigValues)
{
	ReadInput readInput;
	vector<string> gaConfigurationFileValues = readInput.readGAConfig();
	GeneRanges * gaGenesConfigurationFileValues = readInput.readGAGenesConfig();

	unsigned int chromosomePopulationSize = stoi(gaConfigurationFileValues.at(getGAConfigIndex(gaConfigurations::chromosomePopulationSize)));
	unsigned int iterations = stoi(gaConfigurationFileValues.at(getGAConfigIndex(gaConfigurations::iterations)));
	unsigned int genesAmount = stoi(gaConfigurationFileValues.at(getGAConfigIndex(gaConfigurations::genesAmount)));
	vector<vector<float>> gaGeneRanges = gaGenesConfigurationFileValues->getRangesValues();
	vector<vector<bool>> gaGeneInclusives = gaGenesConfigurationFileValues->getRangesInclusive();
	unsigned short int crossoverProbability = stoi(gaConfigurationFileValues.at(getGAConfigIndex(gaConfigurations::crossoverProbability)));
	unsigned short int mutationProbability = stoi(gaConfigurationFileValues.at(getGAConfigIndex(gaConfigurations::mutationProbability)));
	unsigned short int selectionCutOffSize = stoi(gaConfigurationFileValues.at(getGAConfigIndex(gaConfigurations::selectionCutOffSize)));
	SOMConfigurations * somConfigurationsValues = getSOMConfigurations(somConfigValues);

	GAConfigurations * gaConfiguration = new GAConfigurations(
		chromosomePopulationSize,
		iterations,
		genesAmount,
		gaGeneRanges,
		gaGeneInclusives,
		crossoverProbability,
		mutationProbability,
		selectionCutOffSize,
		somConfigurationsValues
	);

	return new GeneticAlgorithm(gaConfiguration);
}

int main(int argc, char ** argv)
{
	printCommandLineArguments(argc, argv);

	ReadInput readInput;

	vector<string> somConfigurationFileValues = readInput.readSOMConfig();
	vector<string> arguments = readInput.readArguments();
	if (arguments.at(0) == "0")
	{
		SelfOrganisingMap * selfOrganisingMap = getSelfOrganisingMap(somConfigurationFileValues);
		selfOrganisingMap->runSelfOrganisingMap();
	}
	else
	{
		GeneticAlgorithm * geneticAlgorithm = getGeneticAlgorithm(somConfigurationFileValues);
		geneticAlgorithm->runGeneticAlgorithm();
	}

	cout << "Finished Project" << endl;
	int a;
	cin >> a;
	return 0;
}
