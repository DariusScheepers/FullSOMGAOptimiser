// FullSOMGAOptimiser.cpp : Defines the entry point for the application.
//

#include "FullSOMGAOptimiser.h"

CalculationHelper * calculations;

ReadInput * reader;
Writer * writer;

string dataSetName;

bool useGA = false;
bool useSobolNumbers = false;
bool runThroughAllDataSets = false;
string currentDataSet;

vector<string> dataSets;

void populateDataSets()
{
	dataSets.push_back("iris/iris.data");
	dataSets.push_back("ionosphere/ionosphere.data");
	dataSets.push_back("monks/monks1.data");
	dataSets.push_back("monks/monks2.data");
	dataSets.push_back("monks/monks3.data");
	dataSets.push_back("diabetes/pima.data");
}

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
		case gaConfigurations::maxIterations:
			return 1;
		case gaConfigurations::standardDeviationThreshold:
			return 2;
		case gaConfigurations::mutationProbability:
			return 3;
		case gaConfigurations::selectionCutOffSize:
			return 4;
		case gaConfigurations::crossOverSplit:
			return 5;
		case gaConfigurations::mutationStandardDeviation:
			return 6;
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

void setDataSetFileName(string fullFileName)
{
	cout << "Setting Data File Name...\n";
	currentDataSet = fullFileName;
	int indexOfBeginningOfFileName = 0;
	int index = 0;
	for (char character : fullFileName)
	{
		if (character == '/')
		{
			indexOfBeginningOfFileName = index + 1;
		}
		index++;
	}
	dataSetName = "";
	for (size_t i = indexOfBeginningOfFileName; i < fullFileName.length(); i++)
	{
		dataSetName = dataSetName + fullFileName.at(i);
	}
	cout << "Sette Data File Name\n";
	writer->setDataSetName(dataSetName);
}

bool generateSingletons(int argc, char ** argv)
{
	string providedRoute;
	if (argc < 2)
	{
		cout << "ERROR: No path to main working directory is provided. Switching to default\n";
		providedRoute = "/home/ec2-user/Doc/FullSOMGAOptimiser/FullSOMGAOptimiser/";
		// return false;
	}
	else
	{
		providedRoute = argv[1];
	}
	cout << "In 0: " << argv[0] << endl;
	cout << "Provided route: " << providedRoute << endl;

	calculations = new CalculationHelper();
	writer = new Writer(providedRoute);
	reader = new ReadInput(providedRoute);
	return true;
}

SOMConfigurations * getSOMConfigurations(vector<string> values)
{
	cout << "Getting SOMConfig...\n";
	const char seperator = values.at(getSOMConfigIndex(somConfigurations::dataSeperator)).at(0);
	const int maxEpochs = stoi(values.at(getSOMConfigIndex(somConfigurations::maximumTrainingIterations)));
	const int trainingSetPortion = stoi(values.at(getSOMConfigIndex(somConfigurations::traningSetPortion)));
	const int slidingWindowOffset = stoi(values.at(getSOMConfigIndex(somConfigurations::slidingWindowOffset)));
	const double stoppingCriteriaThreshhold = stod(values.at(getSOMConfigIndex(somConfigurations::stoppingCriteriaThreshhold)));
	const bool showFullOutput = !useGA && !useSobolNumbers;
	vector<vector<double>> dataSet;
	if (!runThroughAllDataSets)
	{
		dataSet = reader->readDataSet(values.at(getSOMConfigIndex(somConfigurations::dataSet)), seperator);
		setDataSetFileName(values.at(getSOMConfigIndex(somConfigurations::dataSet)));
	}
	else
	{
		dataSet = reader->readDataSet(currentDataSet, seperator);
	}

	cout << "Got SOMConfig...\n";
	return new SOMConfigurations(maxEpochs,
		trainingSetPortion,
		dataSet,
		slidingWindowOffset,
		stoppingCriteriaThreshhold,
		calculations,
		writer,
		dataSetName,
		showFullOutput
	);
}

SelfOrganisingMap * getSelfOrganisingMap(vector<string> values)
{
	cout << "Getting SOM...\n";
	SOMConfigurations * somConfigurations = getSOMConfigurations(values);
	somConfigurations->runDataPreperations();
	const int rows = stoi(values.at(getSOMConfigIndex(somConfigurations::defaultRows)));
	const int columns = stoi(values.at(getSOMConfigIndex(somConfigurations::defaultColumns)));
	const double learningRate = stod(values.at(getSOMConfigIndex(somConfigurations::defaultLearningRate)));
	const double learningDecay = stod(values.at(getSOMConfigIndex(somConfigurations::defaultLearningRateDecay)));
	const double kernelWidth = stod(values.at(getSOMConfigIndex(somConfigurations::defaultKernelWidth)));
	const double kernelDecay = stod(values.at(getSOMConfigIndex(somConfigurations::defaultKernelWidthDecay)));
	
	cout << "Got SOM...\n";
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
	cout << "Getting GA...\n";
	vector<string> gaConfigurationFileValues = reader->readGAConfig();
	GeneRanges * gaGenesConfigurationFileValues = reader->readGAGenesConfig();

	unsigned int chromosomePopulationSize = stoi(gaConfigurationFileValues.at(getGAConfigIndex(gaConfigurations::chromosomePopulationSize)));
	unsigned int maxIterations = stoi(gaConfigurationFileValues.at(getGAConfigIndex(gaConfigurations::maxIterations)));
	double standardDeviationThreshold = stod(gaConfigurationFileValues.at(getGAConfigIndex(gaConfigurations::standardDeviationThreshold)));
	vector<vector<double>> gaGeneRanges = gaGenesConfigurationFileValues->getRangesValues();
	vector<vector<bool>> gaGeneInclusives = gaGenesConfigurationFileValues->getRangesInclusive();
	unsigned short int mutationProbability = stoi(gaConfigurationFileValues.at(getGAConfigIndex(gaConfigurations::mutationProbability)));
	unsigned short int selectionCutOffSize = stoi(gaConfigurationFileValues.at(getGAConfigIndex(gaConfigurations::selectionCutOffSize)));
	unsigned short int crossOverSplit = stoi(gaConfigurationFileValues.at(getGAConfigIndex(gaConfigurations::crossOverSplit)));
	unsigned short int mutationStandardDeviation = stoi(gaConfigurationFileValues.at(getGAConfigIndex(gaConfigurations::mutationStandardDeviation)));
	SOMConfigurations * somConfigurationsValues = getSOMConfigurations(somConfigValues);

	delete gaGenesConfigurationFileValues;

	GAConfigurations * gaConfiguration = new GAConfigurations(
		chromosomePopulationSize,
		maxIterations,
		standardDeviationThreshold,
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

	cout << "Got GA...\n";
	return new GeneticAlgorithm(gaConfiguration);
}

void handleBestSOM(SelfOrganisingMap * bestSolution)
{
	bestSolution->run30FoldCrossValidation();

	bestSolution->printTrainingAndTestSetsQEHistories();
	string fileName = calculations->getTimeString() + "_BestSolutionParameters_";
	string output = bestSolution->parametersToString();
	output = output + "\nFitness:\t" + to_string(bestSolution->calculatePerformance());
	writer->writeToFileWithNameUsingOneLine(fileName, output);
	bestSolution->printFinalNeuronMap();
}

void runGA(vector<string> somConfigurationFileValues)
{
	GeneticAlgorithm * geneticAlgorithm = getGeneticAlgorithm(somConfigurationFileValues);
	geneticAlgorithm->runGeneticAlgorithm();
	SelfOrganisingMap * bestSolution = geneticAlgorithm->returnBestChromsomes()->returnSolution();
	handleBestSOM(bestSolution);
	delete geneticAlgorithm;
}

SobolNumbers * getSobolNumbers(vector<string> somConfigurationFileValues)
{	
	SOMConfigurations * somConfigurations = getSOMConfigurations(somConfigurationFileValues);
	somConfigurations->runDataPreperations();
	const char sobolSeperator = ' ';
	const string sobolInputFile = "sobolNumbers/512 6 new-joe-kuo-6.21201.txt";
	vector<vector<double>> sobolNumbers = reader->readDataSet(sobolInputFile, sobolSeperator);
	GeneRanges * gaGenesConfigurationFileValues = reader->readGAGenesConfig();
	vector<vector<double>> parameterRanges = gaGenesConfigurationFileValues->getRangesValues();
	vector<vector<bool>> parameterInclusives = gaGenesConfigurationFileValues->getRangesInclusive();

	SobolNumbers * sn = new SobolNumbers(
		somConfigurations,
		sobolNumbers,
		parameterRanges,
		parameterInclusives
	);

	return sn;
}

void runSobolNumbers(vector<string> somConfigurationFileValues)
{
	SobolNumbers * sobolNumbers = getSobolNumbers(somConfigurationFileValues);
	sobolNumbers->runSobolNumberSequence();
	SelfOrganisingMap * bestSOM = sobolNumbers->returnBestSOM();
	handleBestSOM(bestSOM);
}

int main(int argc, char ** argv)
{
	generateSingletons(argc, argv);

	vector<string> somConfigurationFileValues = reader->readSOMConfig();
	vector<string> arguments = reader->readArguments();
	if (arguments.size() > 0)
	{
		if (arguments.at(0) == "1")
		{
			cout << "Using GA...\n";
			useGA = true;
		}
		else if (arguments.at(0) == "2")
		{
			cout << "Using Sobol Numbers...\n";
			useSobolNumbers = true;
		}

		if (arguments.size() > 1 && arguments.at(1) == "1")
		{
			cout << "Run Through All Data Sets...\n";
			runThroughAllDataSets = true;
		}		
	}
	else
	{
		cout << "No Args";
	}
	if (useGA)
	{
		if (runThroughAllDataSets)
		{
			populateDataSets();
			for (string dataSet : dataSets)
			{
				setDataSetFileName(dataSet);
				runGA(somConfigurationFileValues);
			}
		}
		else
		{
			runGA(somConfigurationFileValues);
		}
		
	}
	else if (useSobolNumbers)
	{
		if (runThroughAllDataSets)
		{
			populateDataSets();
			for (string dataSet : dataSets)
			{
				setDataSetFileName(dataSet);
				runSobolNumbers(somConfigurationFileValues);
			}
		}
		else
		{
			runSobolNumbers(somConfigurationFileValues);
		}
	}
	else
	{
		SelfOrganisingMap * selfOrganisingMap = getSelfOrganisingMap(somConfigurationFileValues);
		selfOrganisingMap->runSelfOrganisingMap();
		double fitnessValue = selfOrganisingMap->calculatePerformance();
		cout << "Overall fitness: " << fitnessValue << endl;
		delete selfOrganisingMap;
	}
	

	cout << "Finished Project.\nEnter any number to stop..." << endl;
	// _CrtDumpMemoryLeaks();
	int a;
	cin >> a;
	return 0;
}
