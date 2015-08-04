#include "StdInc.h"
#include "Tool.h"

void writeLeaderboardDef(zoneInfo_t* info, ZStream* buf, LeaderboardDef* data)
{
	WRITE_ASSET(data, LeaderboardDef);
	WRITE_NAME(data);

	if (data->columns)
	{
		buf->align(ALIGN_TO_4);
		LbColumnDef* cols = (LbColumnDef*)buf->at();
		buf->write(data->columns, sizeof(LbColumnDef), data->columnCount);
		for (int i = 0; i < data->columnCount; i++)
		{
			if (cols[i].title)
			{
				buf->align(ALIGN_TO_1);
				buf->write(data->columns[i].title, strlen(data->columns[i].title) + 1, 1);
				cols[i].title = (const char*)-1;
			}

			if (cols[i].statName)
			{
				buf->align(ALIGN_TO_1);
				buf->write(data->columns[i].statName, strlen(data->columns[i].statName) + 1, 1);
				cols[i].statName = (const char*)-1;
			}
		}
		dest->columns = (LbColumnDef*)-1;
	}
}

void * addLeaderboardDef(zoneInfo_t* info, const char* name, char* data, int dataLen)
{
	if (dataLen > 0)  { Com_Error(false, "Can only export built in Leaderboards!");  return NULL; }
	return data;
}