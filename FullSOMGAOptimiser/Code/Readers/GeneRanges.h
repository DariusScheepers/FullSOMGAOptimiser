#ifndef GENE_RANGES_H
#define GENE_RANGES_H

#include <vector>
#include <iostream>
#include <string>
using namespace std;

class GeneRanges
{
public:
	GeneRanges(vector<string>);
	~GeneRanges();

	vector<vector<bool>> getRangesInclusive();
	vector<vector<float>> getRangesValues();

private:
	vector<vector<bool>> rangesInclusive;
	vector<vector<float>> rangesValues;

	void handleValues(vector<string>);
};
#endif // !GENE_RANGES_H

