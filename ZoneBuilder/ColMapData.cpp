#include "StdInc.h"
#include "Tool.h"

/*

void writeColMap(zoneInfo_t* info, ZStream* buf, Col_Map* data)
{
	int * modelOffset = new int[data->numStaticModels];
	for(int i=0; i<data->numStaticModels; i++)
	{
		XModel* m = *(XModel**)(data->staticModelList[i]);
		modelOffset[i] = requireAsset(info, ASSET_TYPE_XMODEL, m->name, buf);
	}

	data->mapEnts = (MapEnts*)(requireAsset(info, ASSET_TYPE_MAP_ENTS, (char*)data->name, buf));

	Col_Map* dest = (Col_Map*)buf->at();
	buf->write(data, sizeof(Col_Map), 1);
	buf->write(dest->name, strlen(dest->name) + 1, 1); // bsp HAS to be the same name as the zone
	dest->name = (char*)-1;

	if(dest->cPlanes)
	{
		buf->write(data->cPlanes, sizeof(cPlane), dest->numCPlanes);
		dest->cPlanes = (cPlane*)-1;
	}

	if(dest->staticModelList)
	{
		DebugBreak();
	}

	if(dest->materials)
	{
		dMaterial* mats = (dMaterial*)buf->at();
		buf->write(data->materials, sizeof(dMaterial), dest->numMaterials);
		for(int i=0; i<data->numMaterials; i++)
		{
			buf->write(data->materials[i].name, strlen(data->materials[i].name) + 1, 1);
			mats[i].name = (char*)-1;
		}
	}

	if(dest->cBrushSides)
	{
		DebugBreak();
	}

	if(dest->cBrushEdges)
	{
		buf->write(data->cBrushEdges, 1, data->numCBrushEdges);
		dest->cBrushEdges = (char*)-1;
	}

	if(dest->cNodes)
	{
		cNode* node = (cNode*)buf->at();
		buf->write(data->cNodes, sizeof(cNode), data->numCNodes);	
		for(int i=0; i<data->numCNodes; i++)
		{
			buf->write(data->cNodes[i].plane, sizeof(cPlane), 1);
			node[i].plane = (cPlane*)-1;
		}
	}

	if(dest->cLeaf)
	{
		buf->write(data->cLeaf, 40, data->numCLeaf);
		dest->cLeaf = (char*)-1;
	}

	if(dest->cLeafBrushes)
	{
		buf->write(data->cLeafBrushes, 2, data->numCLeafBrushes);
		dest->cLeafBrushes = (char*)-1;
	}

	if(dest->cLeafBrushNodes)
	{
		cLeafBrushNode* node = (cLeafBrushNode*)buf->at();
		buf->write(data->cLeafBrushNodes, sizeof(cLeafBrushNode), data->numCLeafBrushNodes);
		for(int i=0; i<data->numCLeafBrushNodes; i++)
		{
			if(node[i].numBrushes > 0)
			{
				buf->write(data->cLeafBrushNodes[i].leaf, 2, node[i].numBrushes);
				node[i].leaf = (short*)-1;
			}
		}
	}

	if(dest->unknown1)
	{
		DebugBreak();
	}

	if(dest->verts)
	{
		DebugBreak();
	}

	if(dest->triIndicies)
	{
		DebugBreak();
	}

	if(dest->triEdgeIsWalkable)
	{
		DebugBreak();
	}

	if(dest->collisionPartitions)
	{
		DebugBreak();
	}

	if(dest->collisionAABBTrees)
	{
		DebugBreak();
	}

	if(dest->cModels)
	{
		buf->write(data->cModels, 68, data->numCModels);
		dest->cModels = (char*)-1;
	}

	if(dest->cBrushes)
	{
		cBrush* brushes = (cBrush*)buf->at();
		buf->write(data->cBrushes, sizeof(cBrush), data->numCBrushes);
		for(int i=0; i<data->numCBrushes; i++)
		{
			if(brushes[i].brushSide)
			{
				buf->write(brushes[i].brushSide, sizeof(cBrushSide), 1);
				buf->write(brushes[i].brushSide->side, sizeof(cPlane), 1);
				brushes[i].brushSide->side = (cPlane*)-1;
				brushes[i].brushSide = (cBrushSide*)-1;
			}
			if(brushes[i].brushEdge)
			{
				buf->write(brushes[i].brushEdge, 1, 1);
				brushes[i].brushEdge = (char*)-1;
			}
		}
	}

	if(dest->unknown5)
	{
		buf->write(data->unknown5, 24, data->numCBrushes);
		dest->unknown5 = (char*)-1;
	}

	if(dest->unkInt)
	{
		buf->write(data->unkInt, 4, data->numCBrushes);
		dest->unkInt = (int*)-1;
	}

	if(dest->unknown6)
	{
		buf->write(data->unknown6, 28, data->unkCount6);
		dest->unknown6 = (int*)-1;
	}

	// dynent shit here later

	delete[] modelOffset;
}

void * addColMap(zoneInfo_t* info, const char* name, char* data, int dataLen)
{
	if(dataLen == 0)
	{
		Col_Map* asset = (Col_Map*)data;
		char bspName[256];
		_snprintf(bspName, 256, "maps/mp/%s.d3dbsp", info->name);
		asset->name = strdup(bspName);

		for(int i=0; i<asset->numStaticModels; i++)
		{
			//XModel* m = *(XModel**)(asset->staticModelList[i]);

		}
		addAsset(info, ASSET_TYPE_MAP_ENTS, asset->name, addMapEnts(info, asset->name, (char*)asset->mapEnts, 0));
	}
	return data;
}

*/