#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

class Writer
{
public:
	Writer(string);
	~Writer();
	void writeToFileWithName(string name, vector<string> lines);
	void writeToFileWithNameUsingOneLine(string name, string line);
	void setDataSetName(string);

private:
	string absolutePathToMain = "C:/Users/bbdnet1882/Documents/Universiteit/COS700/Code/working/FullSOMGAOptimiser/FullSOMGAOptimiser/";
	string dataSetName;
};
