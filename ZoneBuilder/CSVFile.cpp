// Load and parse a csv file
// they are used for all sorts of config stuff in this project

#include "StdInc.h"
#include "Tool.h"
#include "CSVFile.h"

CSVFile::CSVFile(char* input, int count)
{
	char * data = (char*)malloc(count + 1);
	memcpy(data, input, count);
	char * at = data;
	char* start = data;
	int curRow = 0, curCol = 0;
	int len = strlen(input);
	while(at < data + count)
	{
		if(*at == ',')
		{
			*at = 0;
			_data[make_pair(curRow, curCol)] = string(start);
			start = at + 1;
			curCol++;
		}

		if(*at == '\n' || *at == '\r')
		{
			*at = 0;
			_data[make_pair(curRow, curCol)] = string(start);
			at++;

			while(*at == '\n' || *at == '\r')
				at++;

			start = at;

			curRow++;
			curCol = 0;
		}

		at++;
	}

	if(start != at - 1)
	{
		*at = 0;
		_data[make_pair(curRow, curCol)] = string(start);
	}

	free(data);
}

char* CSVFile::getData(int row, int col)
{
	map<pair<int,int>,string>::iterator it;
	it = _data.find(make_pair(row,col));

	if (it == _data.end())
	{
		return NULL;
	}
	else
	{
		return (char*)_data[make_pair(row, col)].c_str();
	}
}