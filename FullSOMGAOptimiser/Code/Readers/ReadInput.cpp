#include "ReadInput.hpp"

ReadInput::ReadInput(string absolutePathToMain)
{
	ReadInput::absolutePathToMain = absolutePathToMain;
}

ReadInput::~ReadInput()
{
    
}

dataMatrix ReadInput::readDataSet(string fileName, char seperator)
{
	const string filePath = absolutePathToMain + "Data/" + fileName;
	cout << "Reading file path: " << filePath << endl;
    vector<string> lines = getFileContent(filePath);

	dataMatrix data;
    const int linesAmount = lines.size();
    for (size_t i = 0; i < linesAmount; i++)
    {
        const string line = lines.at(i);
        vector<double> lineData;
        const int linesLength = line.length();
        string dataPiece = "";
        for (size_t i = 0; i < linesLength; i++)
        {
            const char currentChar = line.at(i);
            if (currentChar != seperator)
            {
                dataPiece += currentChar;
            }
            else
            {
                try
                {
                    const double finalData = stod(dataPiece);
                    lineData.push_back(finalData);
                }
                catch(const exception& e)
                {}
                dataPiece = "";
            }
        }
		if (dataPiece.size() > 0 && isdigit(dataPiece.at(0)))
		{
			const double finalData = stod(dataPiece);
			lineData.push_back(finalData);
		}
        data.push_back(lineData);       
    }
    return data;
}

vector<string> ReadInput::getFileContent(string fileName)
{
	cout << "ReadFile: " << fileName << endl;
	ifstream in(fileName.c_str());

    vector<string> out;
	string str;
	while (std::getline(in, str))
	{
		// Line contains string of length > 0 then save it in vector
		if (str.size() > 0)
        {
			out.push_back(str);
        }
	}
	//Close The File
	in.close();
    out.shrink_to_fit();
	cout << "Shap" << endl;
	return out;
}

vector<string> ReadInput::readKeysOfKeyValuePairs(vector<string> lines)
{
    vector<string> values;
    string key = "";
    bool keyRead = false;
    string value = "";
    for (string line : lines)
    {
        for (size_t i = 0; i < line.length(); i++)
        {
            char character = line.at(i);
			//if (character == '/')
			//{
			//	break;
			//}
            if (character == ':') {
                i += 1;
                keyRead = true;
            } else if (keyRead)
            {
                value += character;
            } else {
                key += character;
            }
        }
		if (value != "") {
			values.push_back(value);
		}
        keyRead = false;
        key = "";
        value = "";
    }
    return values;
}

vector<string> ReadInput::readArguments()
{
    const string argumentsPath = absolutePathToMain + "Configurations/Arguments.txt";
	//cout << "Args Full Path: " << argumentsPath << endl;
    vector<string> lines = getFileContent(argumentsPath);
	cout << "Arguments Length: " << to_string(lines.size()) << endl;
    return readKeysOfKeyValuePairs(lines);
}

vector<string> ReadInput::readSOMConfig()
{
    const string argumentsPath = absolutePathToMain + "Configurations/SelfOrganisingMap_Config.txt";
	//cout << "SOMConfig Full Path: " << argumentsPath << endl;
    vector<string> lines = getFileContent(argumentsPath);
    return readKeysOfKeyValuePairs(lines);
}

vector<string> ReadInput::readGAConfig()
{
	const string argumentsPath = absolutePathToMain + "Configurations/GeneticAlgorithm_Config.txt";
	vector<string> lines = getFileContent(argumentsPath);
	return readKeysOfKeyValuePairs(lines);
}

GeneRanges * ReadInput::readGAGenesConfig()
{
	const string argumentsPath = absolutePathToMain + "Configurations/GeneticAlgorithmGenes_Config.txt";
	vector<string> lines = getFileContent(argumentsPath);
	vector<string> values = readKeysOfKeyValuePairs(lines);
	return new GeneRanges(values);
}