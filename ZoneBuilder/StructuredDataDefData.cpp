#include "StdInc.h"
#include "Tool.h"

void writeStructuredDataDefSet(zoneInfo_t* info, ZStream* buf, StructuredDataDefSet* data)
{
	WRITE_ASSET(data, StructuredDataDefSet);
	buf->pushStream(ZSTREAM_VIRTUAL);

	WRITE_NAME(data);

	if (data->defs)
	{
		buf->align(ALIGN_TO_4);
		StructuredDataDef* defs = (StructuredDataDef*)buf->at();
		buf->write(data->defs, sizeof(StructuredDataDef), data->defCount);

		for (int i = 0; i < data->defCount; i++)
		{
			StructuredDataDef* curdef = &data->defs[i];
			if (curdef->enums)
			{
				buf->align(ALIGN_TO_4);
				StructuredDataEnum* enums = (StructuredDataEnum*)buf->at();
				buf->write(curdef->enums, sizeof(StructuredDataEnum), curdef->enumCount);

				for (int j = 0; j < curdef->enumCount; j++)
				{
					if (enums[j].entries)
					{
						buf->align(ALIGN_TO_4);
						StructuredDataEnumEntry* entries = (StructuredDataEnumEntry*)buf->at();
						buf->write(enums[j].entries, sizeof(StructuredDataEnumEntry), enums[j].entryCount);

						for (int k = 0; k < enums[j].entryCount; k++)
						{
							if (entries[k].name)
							{
								buf->write(entries[k].name, 1, strlen(entries[k].name) + 1);
								entries[k].name = (const char*)-1;
							}
						}

						enums[j].entries = (StructuredDataEnumEntry*)-1;
					}
				}
				defs[i].enums = (StructuredDataEnum*)-1;
			}

			if (curdef->structs)
			{
				buf->align(ALIGN_TO_4);
				StructuredDataStruct* structs = (StructuredDataStruct*)buf->at();
				buf->write(curdef->structs, sizeof(StructuredDataStruct), curdef->structCount);

				for (int j = 0; j < curdef->structCount; j++)
				{
					if (structs[j].properties)
					{
						buf->align(ALIGN_TO_4);
						StructuredDataStructProperty* props = (StructuredDataStructProperty*)buf->at();
						buf->write(structs[j].properties, sizeof(StructuredDataStructProperty), structs[j].propertyCount);

						for (int k = 0; k < structs[j].propertyCount; k++)
						{
							if (props[k].name)
							{
								buf->write(props[k].name, 1, strlen(props[k].name) + 1);
								props[k].name = (const char*)-1;
							}
						}

						structs[j].properties = (StructuredDataStructProperty*)-1;
					}
				}
				defs[i].structs = (StructuredDataStruct*)-1;
			}

			if (curdef->indexedArrays)
			{
				buf->align(ALIGN_TO_4);
				buf->write(curdef->indexedArrays, sizeof(StructuredDataIndexedArray), curdef->indexedArrayCount);
				defs[i].indexedArrays = (StructuredDataIndexedArray*)-1;
			}

			if (curdef->enumedArrays)
			{
				buf->align(ALIGN_TO_4);
				buf->write(curdef->enumedArrays, sizeof(StructuredDataEnumedArray), curdef->enumedArrayCount);
				defs[i].enumedArrays = (StructuredDataEnumedArray*)-1;
			}
		}
	}
	buf->popStream();
}

void* addStructuredDataDefSet(zoneInfo_t* info, const char* name, char* data, int dataLen)
{
	if (dataLen > 0) { Com_Error(false, "No custom structureddatadefs."); return NULL; }

	return data;
}