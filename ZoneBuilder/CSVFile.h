#pragma once
#include "StdInc.h"
#include "Tool.h"
#include <map>

class CSVFile
{
public:
	CSVFile(char* data, int count);
	char* getData(int row, int col);
private:
	map<pair<int, int>, string> _data;
};