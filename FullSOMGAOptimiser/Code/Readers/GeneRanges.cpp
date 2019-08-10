#include "GeneRanges.h"

GeneRanges::GeneRanges(vector<string> values)
{
	handleValues(values);
}

GeneRanges::~GeneRanges()
{
}

void GeneRanges::handleValues(vector<string> values)
{
	for each (string value in values)
	{
		int index = 0;
		vector<bool> valueInclusive;
		vector<float> valueRange;
		bool commaRead = false;
		string valueBeingRead = "";
		for each (char character in value)
		{
			if (character == '[' || character == '(')
			{
				if (character == '[')
				{
					valueInclusive.push_back(true);
				}
				else
				{
					valueInclusive.push_back(false);
				}
			}
			else if (character == ',')
			{
				valueRange.push_back(stof(valueBeingRead));
				valueBeingRead = "";
				commaRead = true;
			}
			else if (!commaRead)
			{
				valueBeingRead += character;
			}
			else if (character == ']' || character == ')')
			{
				valueRange.push_back(stof(valueBeingRead));
				valueBeingRead = "";
				if (character == ']')
				{
					valueInclusive.push_back(true);
				}
				else
				{
					valueInclusive.push_back(false);
				}
			}
			else
			{
				valueBeingRead += character;
			}
		}
		rangesInclusive.push_back(valueInclusive);
		rangesValues.push_back(valueRange);
	}
}

vector<vector<bool>> GeneRanges::getRangesInclusive()
{
	return rangesInclusive;
}

vector<vector<float>> GeneRanges::getRangesValues()
{
	return rangesValues;
}