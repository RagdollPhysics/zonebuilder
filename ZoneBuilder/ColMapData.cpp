#include "StdInc.h"
#include "Tool.h"

void writeColMap(zoneInfo_t* info, ZStream* buf, clipMap_t* data)
{
	for(int i=0; i<data->numStaticModels; i++)
	{
		XModel* m = data->staticModelList[i].xmodel;
		data->staticModelList[i].xmodel = (XModel*)requireAsset(info, ASSET_TYPE_XMODEL, m->name, buf);
	}

	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < data->dynEntCount[j]; i++)
		{
			XModel* m = data->dynEntDefList[j]->xModel;
			data->dynEntDefList[j]->xModel = (XModel*)requireAsset(info, ASSET_TYPE_XMODEL, m->name, buf);
			FxEffectDef* fx = data->dynEntDefList[j]->destroyFx;
			data->dynEntDefList[j]->destroyFx = (FxEffectDef*)requireAsset(info, ASSET_TYPE_FX, fx->name, buf);
			PhysPreset* p = data->dynEntDefList[j]->physPreset;
			data->dynEntDefList[j]->physPreset = (PhysPreset*)requireAsset(info, ASSET_TYPE_PHYSPRESET, p->name, buf);
		}
	}
	data->mapEnts = (MapEnts*)(requireAsset(info, ASSET_TYPE_MAP_ENTS, (char*)data->name, buf));

	clipMap_t* dest = (clipMap_t*)buf->at();
	buf->write(data, sizeof(clipMap_t), 1);
	buf->pushStream(ZSTREAM_VIRTUAL);

	buf->write(dest->name, strlen(dest->name) + 1, 1); // bsp HAS to be the same name as the zone
	dest->name = (char*)-1;

	if(dest->cPlanes) // OffsetToPointer
	{
		buf->align(ALIGN_TO_4);
		buf->write(data->cPlanes, sizeof(cPlane), data->numCPlanes);
		dest->cPlanes = (cPlane*)-1;
	}

	if(dest->staticModelList)
	{
		// xmodel is already stored
		buf->align(ALIGN_TO_4);
		buf->write(data->staticModelList, sizeof(cStaticModel), data->numStaticModels);
		dest->staticModelList = (cStaticModel*)-1;
	}

	if(dest->materials)
	{
		buf->align(ALIGN_TO_4);
		dMaterial* mats = (dMaterial*)buf->at();
		buf->write(data->materials, sizeof(dMaterial), data->numMaterials);
		for(int i=0; i<data->numMaterials; i++)
		{
			buf->write(data->materials[i].name, strlen(data->materials[i].name) + 1, 1);
			mats[i].name = (char*)-1;
		}
		dest->materials = (dMaterial*)-1;
	}

	if(dest->cBrushSides)
	{
		buf->align(ALIGN_TO_4);
		cBrushSide* sides = (cBrushSide*)buf->at();
		buf->write(data->cBrushSides, sizeof(cBrushSide), data->numCBrushSides);
		for (int i = 0; i<data->numCBrushSides; i++)
		{
			if (sides[i].side) // OffsetToPointer
			{
				buf->align(ALIGN_TO_4);
				buf->write(sides[i].side, sizeof(cPlane), 1);
				sides[i].side = (cPlane*)-1;
			}
		}
		dest->cBrushSides = (cBrushSide*)-1;
	}

	if(dest->cBrushEdges)
	{
		buf->align(ALIGN_TO_1);
		buf->write(data->cBrushEdges, 1, data->numCBrushEdges);
		dest->cBrushEdges = (char*)-1;
	}

	if(dest->cNodes)
	{
		buf->align(ALIGN_TO_4);
		cNode* nodes = (cNode*)buf->at();
		buf->write(data->cNodes, sizeof(cNode), data->numCNodes);
		for (int i = 0; i<data->numCNodes; i++)
		{
			if (nodes[i].plane) // OffsetToPointer
			{
				buf->align(ALIGN_TO_4);
				buf->write(nodes[i].plane, sizeof(cPlane), 1);
				nodes[i].plane = (cPlane*)-1;
			}
		}
		dest->cNodes = (cNode*)-1;
	}

	if(dest->cLeaf)
	{
		buf->write(data->cLeaf, sizeof(cLeaf), data->numCLeaf);
		dest->cLeaf = (cLeaf*)-1;
	}

	if(dest->leafBrushes)
	{
		buf->write(data->leafBrushes, 2, data->numLeafBrushes);
		dest->leafBrushes = (short*)-1;
	}

	if(dest->cLeafBrushNodes)
	{
		cLeafBrushNode* node = (cLeafBrushNode*)buf->at();
		buf->write(data->cLeafBrushNodes, sizeof(cLeafBrushNode), data->numCLeafBrushNodes);
		for(int i=0; i<data->numCLeafBrushNodes; i++)
		{
			if(node[i].leafBrushCount > 0) // OffsetToPointer
			{
				buf->align(ALIGN_TO_2);
				buf->write(&node[i].data.leaf, 2, node[i].leafBrushCount);
				node[i].data.leaf.brushes = (unsigned short*)-1;
			}
		}
		dest->cLeafBrushNodes = (cLeafBrushNode*)-1;
	}

	if(dest->leafSurfaces)
	{
		buf->align(ALIGN_TO_4);
		buf->write(data->leafSurfaces, sizeof(int), data->numLeafSurfaces);
		dest->leafSurfaces = (int*)-1;
	}

	if(dest->verts)
	{
		buf->align(ALIGN_TO_4);
		buf->write(data->verts, sizeof(vec3_t), data->numVerts);
		dest->verts = (vec3_t*)-1;
	}

	if(dest->triIndices)
	{
		buf->align(ALIGN_TO_2);
		buf->write(data->triIndices, sizeof(short), 3 * data->numTriIndices);
		dest->triIndices = (short*)-1;
	}

	if(dest->triEdgeIsWalkable)
	{
		buf->align(ALIGN_TO_1);
		buf->write(data->triEdgeIsWalkable, sizeof(bool), ((3 * data->numTriIndices + 31) >> 3) & 0xFFFFFFFC);
		dest->triEdgeIsWalkable = (bool*)-1;
	}

	if (dest->collisionBorders)
	{
		buf->align(ALIGN_TO_4);
		buf->write(data->collisionBorders, sizeof(CollisionBorder), data->numCollisionBorders);
		dest->collisionBorders = (CollisionBorder*)-1;
	}

	if(dest->collisionPartitions)
	{
		buf->align(ALIGN_TO_4);
		CollisionPartition* border = (CollisionPartition*)buf->at();
		buf->write(data->collisionPartitions, sizeof(CollisionPartition), data->numCollisionPartitions);
		for (int i = 0; i<data->numCollisionPartitions; i++)
		{
			if (border[i].borders) // OffsetToPointer
			{
				buf->align(ALIGN_TO_4);
				buf->write(&border[i].borders, sizeof(CollisionBorder), border[i].borderCount);
				border[i].borders = (CollisionBorder*)-1;
			}
		}
		dest->collisionPartitions = (CollisionPartition*)-1;
	}

	if(dest->collisionAABBTrees)
	{
		buf->align(ALIGN_TO_16);
		buf->write(data->collisionAABBTrees, sizeof(CollisionAabbTree), data->numCollisionAABBTrees);
		dest->collisionAABBTrees = (CollisionAabbTree*)-1;
	}

	if(dest->cModels)
	{
		buf->align(ALIGN_TO_4);
		buf->write(data->cModels, sizeof(cModel), data->numCModels);
		dest->cModels = (cModel*)-1;
	}

	if(dest->cBrushes)
	{
		buf->align(ALIGN_TO_128);
		cBrush* brushes = (cBrush*)buf->at();
		buf->write(data->cBrushes, sizeof(cBrush), data->numCBrushes);
		for(int i=0; i<data->numCBrushes; i++)
		{
			if(brushes[i].brushSide)
			{
				buf->write(brushes[i].brushSide, sizeof(cBrushSide), 1);
				if (brushes[i].brushSide->side)
				{
					buf->align(ALIGN_TO_4);
					buf->write(brushes[i].brushSide->side, sizeof(cPlane), 1);
					brushes[i].brushSide->side = (cPlane*)-1;
				}
				brushes[i].brushSide = (cBrushSide*)-1;
			}
			if(brushes[i].brushEdge)
			{
				buf->write(brushes[i].brushEdge, 1, 1);
				brushes[i].brushEdge = (char*)-1;
			}
		}
		dest->cBrushes = (cBrush*)-1;
	}

	if(dest->unknown2)
	{
		buf->align(ALIGN_TO_128);
		buf->write(data->unknown2, 24, data->numCBrushes);
		dest->unknown2 = (char*)-1;
	}

	if(dest->unknown3)
	{
		buf->align(ALIGN_TO_4);
		buf->write(data->unknown3, sizeof(int), data->numCBrushes);
		dest->unknown3 = (int*)-1;
	}

	if(dest->unknown4)
	{
		buf->align(ALIGN_TO_4);
		buf->write(data->unknown4, 28, data->unkCount4);
		dest->unknown4 = (int*)-1;
	}

	buf->pushStream(ZSTREAM_RUNTIME);

	for (int i = 0; i < 2; i++)
	{
		if (dest->dynEntDefList[i])
		{
			buf->align(ALIGN_TO_4);
			buf->write(data->dynEntDefList[i], sizeof(DynEntityDef), data->dynEntCount[i]);
			dest->dynEntDefList[i] = (DynEntityDef*)-1;
		}
	}

	for (int i = 0; i < 2; i++)
	{
		if (dest->dynEntPoseList[i])
		{
			buf->align(ALIGN_TO_4);
			buf->write(data->dynEntPoseList[i], sizeof(DynEntityPose), data->dynEntCount[i]);
			dest->dynEntPoseList[i] = (DynEntityPose*)-1;
		}
	}

	for (int i = 0; i < 2; i++)
	{
		if (dest->dynEntClientList[i])
		{
			buf->align(ALIGN_TO_4);
			buf->write(data->dynEntClientList[i], sizeof(DynEntityClient), data->dynEntCount[i]);
			dest->dynEntClientList[i] = (DynEntityClient*)-1;
		}
	}

	for (int i = 0; i < 2; i++)
	{
		if (dest->dynEntCollList[i])
		{
			buf->align(ALIGN_TO_4);
			buf->write(data->dynEntCollList[i], sizeof(DynEntityColl), data->dynEntCount[i]);
			dest->dynEntCollList[i] = (DynEntityColl*)-1;
		}
	}
	
	buf->popStream();
	buf->popStream();
}

void * addColMap(zoneInfo_t* info, const char* name, char* data, int dataLen)
{
	if(dataLen < 0)
	{
		clipMap_t* asset = (clipMap_t*)data;
		char bspName[256];
		_snprintf(bspName, 256, "maps/mp/%s.d3dbsp", info->name);
		asset->name = strdup(bspName);

		for(int i=0; i<asset->numStaticModels; i++)
		{
			XModel* m = asset->staticModelList[i].xmodel;
			addAsset(info, ASSET_TYPE_XMODEL, m->name, addXModel(info, m->name, (char*)m, -1));
		}

		for (int j = 0; j < 2; j++)
		{
			for (int i = 0; i < asset->dynEntCount[j]; i++)
			{
				XModel* m = asset->dynEntDefList[j]->xModel;
				addAsset(info, ASSET_TYPE_XMODEL, m->name, addXModel(info, m->name, (char*)m, -1));
				FxEffectDef* fx = asset->dynEntDefList[j]->destroyFx;
				addAsset(info, ASSET_TYPE_FX, fx->name, addFxEffectDef(info, fx->name, (char*)fx, -1));
				PhysPreset* p = asset->dynEntDefList[j]->physPreset;
				addAsset(info, ASSET_TYPE_PHYSPRESET, p->name, addFxEffectDef(info, p->name, (char*)p, -1));
			}
		}

		addAsset(info, ASSET_TYPE_MAP_ENTS, asset->name, addMapEnts(info, asset->name, (char*)asset->mapEnts, -1));
	}
	return data;
}