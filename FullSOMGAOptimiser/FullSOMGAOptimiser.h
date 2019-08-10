﻿// FullSOMGAOptimiser.h : Include file for standard system include files,
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

using namespace std;

enum class somConfigurations {
	dataSet,
	dataSeperator,
	traningSetPortion,
	defaultMaximumTrainingIterations,
	defaultRows,
	defaultColumns,
	defaultLearningRate,
	defaultLearningRateDecay,
	defaultKernelWidth,
	defaultKernelWidthDecay
};

enum class gaConfigurations {
	chromosomePopulationSize,
	iterations,
	genesAmount,
	crossoverProbability,
	mutationProbability,
	selectionCutOffSize
};

enum class gaGenesConfigurations {
	rows,
	columns,
	learningRate,
	learningRateDecay,
	kernelWidth,
	kernelWidthDecay,
	trainingIterations
};

// TODO: Reference additional headers your program requires here.
