// FullSOMGAOptimiser.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "./Code/Readers/ReadInput.hpp"
#include "./Code/SelfOrganisingMap/SOMConfigurations.hpp"
#include "./Code/SelfOrganisingMap/SelfOrganisingMap.hpp"

using namespace std;

enum class somConfigurations {
	dataSet,
	dataSeperator,
	maximumTrainingIterations,
	traningSetPortion,
	defaultRows,
	defaultColumns,
	defaultLearningRate,
	defaultKernelWidth,
	defaultLearningRateDecay,
	defaultKernelWidthDecay
};


// TODO: Reference additional headers your program requires here.
