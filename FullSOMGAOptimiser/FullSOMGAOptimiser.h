// FullSOMGAOptimiser.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "./Code/Readers/ReadInput.hpp"
#include "./Code/SelfOrganisingMap/SOMConfigurations.hpp"
#include "./Code/SelfOrganisingMap/SelfOrganisingMap.hpp"
#include "./Code/Readers/GeneRanges.h"
#include "./Code/GeneticAlgorithm/GAConfigurations.hpp"
#include "./Code/GeneticAlgorithm/GeneticAlgorithm.hpp"
#include "./Code/SobolNumbers/SobolNumbers.h"

// #define _CRTDBG_MAP_ALLOC
// #include <stdlib.h>
// #include <crtdbg.h>

using namespace std;

enum class somConfigurations {
	dataSet,
	dataSeperator,
	maximumTrainingIterations,
	traningSetPortion,
	slidingWindowOffset,
	stoppingCriteriaThreshhold,
	defaultRows,
	defaultColumns,
	defaultLearningRate,
	defaultLearningRateDecay,
	defaultKernelWidth,
	defaultKernelWidthDecay
};

enum class gaConfigurations {
	chromosomePopulationSize,
	maxIterations,
	standardDeviationThreshold,
	mutationProbability,
	selectionCutOffSize,
	crossOverSplit,
	mutationStandardDeviation
};

enum class gaGenesConfigurations {
	rows,
	columns,
	learningRate,
	learningRateDecay,
	kernelWidth,
	kernelWidthDecay
};

// TODO: Reference additional headers your program requires here.
