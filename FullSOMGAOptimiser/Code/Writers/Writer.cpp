#include "Writer.h"

Writer::Writer(string absolutePathToMain)
{
	Writer::absolutePathToMain = absolutePathToMain + "Output/";
}

Writer::~Writer()
{

}

void Writer::writeToFileWithName(string name, vector<string> lines)
{
	string fileName = absolutePathToMain + name + ".txt";
	ofstream outputFile;
	outputFile.open (fileName, ios::out | ios::trunc);

	for (string line : lines)
	{
		outputFile << line << endl;
	}

	outputFile.close();
}

void Writer::writeToFileWithNameUsingOneLine(string name, string line)
{
	string fileName = absolutePathToMain + name + ".txt";
	ofstream outputFile;
	outputFile.open(fileName, ios::out | ios::trunc);

	outputFile << line << endl;

	outputFile.close();
}