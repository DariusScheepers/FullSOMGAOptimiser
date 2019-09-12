#ifndef READ_INPUT_H
#define READ_INPUT_H

#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <string>
#include <regex>
#include "GeneRanges.h"

#define dataMatrix vector<vector<double>>

using namespace std;

class ReadInput
{
    private:
        vector<string> getFileContent(string fileName);
        vector<string> readKeysOfKeyValuePairs(vector<string>);

		string absolutePathToMain = "C:/Users/bbdnet1882/Documents/Universiteit/COS700/Code/working/FullSOMGAOptimiser/FullSOMGAOptimiser";
    public:
        ReadInput(string);
        ~ReadInput();

		dataMatrix readDataSet(string, char);
        vector<string> readArguments();
        vector<string> readSOMConfig();
		vector<string> readGAConfig();
		GeneRanges * readGAGenesConfig();
};

#endif