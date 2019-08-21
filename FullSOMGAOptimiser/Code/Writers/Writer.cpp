#include "Writer.h"

Writer::Writer()
{

}

Writer::~Writer()
{

}

void Writer::writeToFileWithName(string name, vector<string> lines)
{
	string fileName = absolutePathToMain + name + ".txt";
	ofstream outputFile;
	outputFile.open (fileName, ios::out | ios::trunc);

	for each (string line in lines)
	{
		outputFile << line << endl;
	}

	outputFile.close();
}