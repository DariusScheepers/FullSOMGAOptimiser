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

private:
	string absolutePathToMain = "C:/Users/bbdnet1882/Documents/Universiteit/COS700/Code/working/FullSOMGAOptimiser/FullSOMGAOptimiser/";
};
