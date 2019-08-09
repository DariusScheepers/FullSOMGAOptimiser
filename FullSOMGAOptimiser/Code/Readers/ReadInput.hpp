#ifndef READ_INPUT_H
#define READ_INPUT_H

#include <vector>
#include <fstream>
#include <string>
#include <iostream>

#define dataMatrix vector<vector<float>>

using namespace std;

class ReadInput
{
    private:
        vector<string> getFileContent(string fileName);
        vector<string> readKeysOfKeyValuePairs(vector<string>);

		string absolutePathToMain = "C:/Users/bbdnet1882/Documents/Universiteit/COS700/Code/working/FullSOMGAOptimiser/FullSOMGAOptimiser";
    public:
        ReadInput(/* args */);
        ~ReadInput();

		dataMatrix readDataSet(string, char);
        vector<string> readArguments();
        vector<string> readSOMConfig();

};

#endif