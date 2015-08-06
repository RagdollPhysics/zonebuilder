#include "StdInc.h"
#include "Tool.h"

void writeFxImpactTable(zoneInfo_t* info, ZStream* buf, FxImpactTable* data)
{
	if (data->table)
	{
		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 31; j++)
			{
				FxEffectDef* effect = data->table[i].nonflesh[j];
				if (effect) data->table[i].nonflesh[j] = (FxEffectDef*)requireAsset(info, ASSET_TYPE_FX, effect->name, buf);
			}

			for (int j = 0; j< 4; j++)
			{
				FxEffectDef* effect = data->table[i].flesh[j];
				if (effect) data->table[i].flesh[j] = (FxEffectDef*)requireAsset(info, ASSET_TYPE_FX, effect->name, buf);
			}
		}
	}

	WRITE_ASSET(data, FxImpactTable);
	buf->pushStream(ZSTREAM_VIRTUAL);
	WRITE_NAME(data);

	if (data->table)
	{
		buf->write(data->table, sizeof(FxImpactEntry), 15);
		dest->table = (FxImpactEntry*)-1;
	}

	buf->popStream(); // VIRTUAL
}

void * addFxImpactTable(zoneInfo_t* info, const char* name, char* data, int dataLen)
{
	if (dataLen > 0)  { Com_Error(false, "Can only export built in FxImpactTables!");  return NULL; }
	return data;
}