#include "StdInc.h"
#include "Tool.h"

#include <string>
#include <xhash>

// we don't like Boost
#define strtk_no_tr1_or_boost

// conflicting windows.h definitions
#undef max
#undef min

// strtk code
#include <strtk.hpp>

int StringTableHash(char* data)
{
	int hash = 0;

	while (*data != 0)
	{
		hash = tolower(*data) + (31 * hash);

		data++;
	}

	return hash;
}

void writeStringTable(zoneInfo_t* info, BUFFER* buf, StringTable* data)
{
	StringTable* outST = (StringTable*)buf->at();
	buf->write(data, sizeof(StringTable), 1);

	buf->write(data->name, strlen(data->name) + 1, 1);
	outST->name = (char*)-1;

	buf->write(-1, data->columns * data->rows * 2);
	for(int ri=0; ri<data->rows; ri++)
	{
		for(int ci=0; ci<data->columns; ci++)
		{
			buf->write(data->data[((ri * data->columns) + ci) * 2], strlen(data->data[((ri * data->columns) + ci) * 2]) + 1, 1);
		}
	}
	outST->data = (char**)-1;
}

void * addStringTable(zoneInfo_t* info, const char* name, char* data, size_t dataLen)
{
	strtk::token_grid::options options;
	options.set_row_delimiters("\r\n");
	options.set_column_delimiters(",");
	options.set_column_split_option(strtk::split_options::default_mode);
	options.set_row_split_option(strtk::split_options::default_mode);

	strtk::token_grid grid(data, strlen(data), options);
	int columns = grid.max_column_count();
	int rows = grid.row_count();

	StringTable* newTable = (StringTable*)malloc(sizeof(StringTable));
	newTable->name = strdup(name);
	newTable->data = (char**)malloc(sizeof(char*) * rows * columns * 2);

	for (std::size_t ri = 0; ri < (std::size_t)rows; ri++)
	{
		strtk::token_grid::row_type r = grid.row(ri);

		for (std::size_t ci = 0; ci < r.size(); ci++)
		{
			std::string strSource = r.get<std::string>(ci);
			char* str = (char*)malloc(strSource.length() + 1);
			memcpy(str, strSource.c_str(), strSource.length() + 1);

			newTable->data[((ri * columns) + ci) * 2] = str;
			newTable->data[(((ri * columns) + ci) * 2) + 1] = (char*)StringTableHash(str);
		}
	}

	newTable->columns = columns;
	newTable->rows = rows;

	return newTable;
}
