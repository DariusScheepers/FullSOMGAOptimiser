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
	vector<vector<double>> getRangesValues();

private:
	vector<vector<bool>> rangesInclusive;
	vector<vector<double>> rangesValues;

	void handleValues(vector<string>);
};
#endif // !GENE_RANGES_H

