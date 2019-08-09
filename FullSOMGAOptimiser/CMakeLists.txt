# CMakeList.txt : CMake project for FullSOMGAOptimiser, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (FullSOMGAOptimiser "FullSOMGAOptimiser.cpp" "FullSOMGAOptimiser.h"
	"./Code/Readers/ReadInput.cpp"
	"./Code/Helpers/CalculationHelper.cpp"
	"./Code/SelfOrganisingMap/InputVector.cpp"
	"./Code/SelfOrganisingMap/Neuron.cpp"
	"./Code/SelfOrganisingMap/SelfOrganisingMap.cpp"
	"./Code/SelfOrganisingMap/SOMConfigurations.cpp"
	"./Code/SelfOrganisingMap/WeightCalculations.cpp"
)

# TODO: Add tests and install targets if needed.
