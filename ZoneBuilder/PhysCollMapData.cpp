#include "StdInc.h"
#include "Tool.h"

void writecbrushside_t(ZStream* buf, cBrushSide* data, int num)
{
	static_assert(sizeof(cBrushSide) == 8, "struct wrong size");
	WRITE_ASSET_NUM(data, cBrushSide, num);
	
	for(int i=0; i<num; i++)
	{	
		if(HAS_FIELD((&data[i]), side))
		{
			buf->write(data[i].side, sizeof(cPlane), 1);
			dest[i].side = (cPlane*)-1;
		}
	}
}

void writeBrush(ZStream* buf, BrushWrapper * data, BrushWrapper* dest)
{
	writecbrushside_t(buf, data->brush.brushSide, data->brush.count);
	data->brush.brushSide = (cBrushSide*)-1;

	WRITE_FIELD(data, brush.brushEdge, char, totalEdgeCount);
}

void writeBrushWrapper(ZStream* buf, BrushWrapper* data)
{
	static_assert(sizeof(BrushWrapper) == 0x44, "struct wrong size");
	WRITE_ASSET(data, BrushWrapper);

	writeBrush(buf, data, dest);

	static_assert(sizeof(cPlane) == 0x14, "struct wrong size");
	WRITE_FIELD(data, planes, cPlane, brush.count);
}

void writePhysGeomInfo(ZStream* buf, PhysGeomInfo* data, int num)
{
	static_assert(sizeof(PhysGeomInfo) == 0x44, "struct wrong size");
	WRITE_ASSET_NUM(data, PhysGeomInfo, num);

	for(int i=0; i<num; i++)
	{
		if(dest[i].brush)
		{
			writeBrushWrapper(buf, dest[i].brush);
		}
	}
}

void writePhysCollmap(zoneInfo_t* info, ZStream* buf, PhysGeomList* data)
{
	static_assert(sizeof(PhysGeomList) == 0x48, "struct wrong size");
	WRITE_ASSET(data, PhysGeomList);
	WRITE_NAME(data);

	writePhysGeomInfo(buf, data->geoms, data->count);
}

void * addPhysCollmap(zoneInfo_t* info, const char* name, char* data, size_t dataLen)
{
	return data;
}