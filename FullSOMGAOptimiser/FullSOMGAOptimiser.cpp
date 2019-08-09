// FullSOMGAOptimiser.cpp : Defines the entry point for the application.
//

#include "FullSOMGAOptimiser.h"

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
	const char seperator = values.at(1).at(0);
	vector<vector<float>> dataSet = readInput.readDataSet(values.at(0), seperator);
	const int maxEpochs = stoi(values.at(2));
	const int trainingSetPortion = stoi(values.at(3));
	return new SOMConfigurations(maxEpochs, trainingSetPortion, dataSet);
}

SelfOrganisingMap * getSelfOrganisingMap(vector<string> values)
{
	SOMConfigurations * somConfigurations = getSOMConfigurations(values);
	const int rows = stoi(values.at(4));
	const int columns = stoi(values.at(5));
	const float learningRate = stof(values.at(6));
	const float learningDecay = stof(values.at(7));
	const float kernelWidth = stof(values.at(8));
	const float kernelDecay = stof(values.at(9));

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

	cout << "Finished Project" << endl;
	int a;
	cin >> a;
	return 0;
}
