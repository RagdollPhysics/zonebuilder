#include "StdInc.h"
#include "Tool.h"

extern void writeXModel(zoneInfo_t* info, BUFFER* buf, XModel* data)
{
	int * materialOffs = new int[data->numSurfaces];
	for(int i=0; i<data->numSurfaces;i++)
	{
		materialOffs[i] = requireAsset(info, ASSET_TYPE_MATERIAL, (char*)data->materials[i]->name, buf);
	}

	XModel* dest = (XModel*)buf->at();
	buf->write(data, sizeof(XModel), 1);
	buf->write(data->name, strlen(data->name) + 1, 1);
	dest->name = (char*)-1;

	if(dest->boneNames) {
		buf->write(dest->boneNames, sizeof(short), dest->numBones);
		dest->boneNames = (short*)-1;
	}

	if(dest->boneUnknown1) {
		buf->write(dest->boneUnknown1, 1, dest->numBones - dest->numSubBones);
		dest->boneUnknown1 = (char*)-1;
	}

	if(dest->tagAngles) {
		buf->write(dest->tagAngles, sizeof(XModelAngle), dest->numBones - dest->numSubBones);
		dest->tagAngles = (XModelAngle*)-1;
	}

	if(dest->tagPositions) {
		buf->write(dest->tagPositions, sizeof(XModelTagPos), dest->numBones - dest->numSubBones);
		dest->tagPositions = (XModelTagPos*)-1;
	}

	if(dest->boneUnknown4) {
		buf->write(dest->boneUnknown4, 1, dest->numBones);
		dest->boneUnknown4 = (char*)-1;
	}

	if(dest->animMatrix) {
		buf->write(dest->animMatrix, 32, dest->numBones);
		dest->animMatrix = (char*)-1;
	}

	if(dest->materials)
	{
		buf->write(materialOffs, sizeof(int), dest->numSurfaces); // should be just the offsets
		dest->materials = (Material**)-1;
	}

	for(int i=0; i<4; i++)
	{
		if(!dest->lods[i].surfaces) continue;
		XModelSurfaces* surfs = (XModelSurfaces*)buf->at();
		buf->write(dest->lods[i].surfaces, sizeof(XModelSurfaces), 1);
		buf->write(dest->lods[i].surfaces->name, strlen(dest->lods[i].surfaces->name) + 1, 1);

		if(surfs->surfaces)
		{
			XSurface* surf = (XSurface*)buf->at();
			buf->write(dest->lods[i].surfaces->surfaces, sizeof(XSurface) * surfs->numSurfaces, 1);
			for(int j=0; j<surfs->numSurfaces; j++)
			{
				if(surf[j].blendInfo)
				{
					buf->write(surf[j].blendInfo, surf[j].blendNum1 + 
						(surf[j].blendNum2 * 3) + (surf[j].blendNum3 * 5) + (surf[j].blendNum4 * 7), 1);
					surf[j].blendInfo = (char*)-1;
				}

				if(surf[j].vertexBuffer)
				{
					buf->write(surf[j].vertexBuffer, 32, surf[j].numVertices);
					surf[j].vertexBuffer = (char*)-1;
					addXZoneMemory(ZONE_STREAM_VERTEX, 32 * surf[j].numVertices);
				}

				if(surf[j].ct)
				{
					XSurfaceCT* ct = (XSurfaceCT*)buf->at();
					buf->write(surf[j].ct, 12, surf[j].numCT);
					
					for(int k=0;k<surf[j].numCT; k++)
					{
						XSurfaceCTEntry* entry = (XSurfaceCTEntry*)buf->at();
						buf->write(ct[k].entry, 40, 1);
						if(entry->node)
						{
							buf->write(entry->node, 16, entry->numNode);
							entry->node = (char*)-1;
						}
						if(entry->leaf)
						{
							buf->write(entry->leaf, 2, entry->numLeaf);
							entry->leaf = (short*)-1;
						}
						ct[k].entry = (XSurfaceCTEntry*)-1;
					}
					surf[j].ct = (XSurfaceCT*)-1;
				}

				if(surf[j].indexBuffer)
				{
					buf->write(surf[j].indexBuffer, 6, surf[j].numPrimitives);
					surf[j].indexBuffer = (Face*)-1;
					addXZoneMemory(ZONE_STREAM_FACE, 6 * surf[j].numPrimitives);
				}
			}
			surfs->name = (char*)-1;
			dest->lods[i].surfaces = (XModelSurfaces*)-1;
			surfs->surfaces = (XSurface*)-1;
		}
	}

	if(dest->colSurf)
	{
		buf->write(dest->colSurf, sizeof(XColSurf), dest->numColSurfs);
		for(int i=0; i<dest->numColSurfs; i++)
		{
			buf->write(dest->colSurf[i].tris, 44, dest->colSurf[i].count);
			dest->colSurf[i].tris = (void*)-1;
		}
		dest->colSurf = (XColSurf*)-1;
	}

	if(dest->unknowns)
	{
		buf->write(dest->unknowns, 28, dest->numBones);
		dest->unknowns = (char*)-1;
	}

	dest->physPreset = NULL;
	dest->physCollmap = NULL;
}

void * addXModel(zoneInfo_t* info, const char* name, char* data, size_t dataLen)
{
	if(dataLen == -1)
	{
		XModel * model = (XModel*)data;
		for(int i=0; i<model->numBones; i++)
		{
			model->boneNames[i] = addScriptString(info, SL_ConvertToString(model->boneNames[i]));
		}
		return data;
	}

	// copy stuff over
	XModel * base = (XModel*)DB_FindXAssetHeader(ASSET_TYPE_XMODEL, "viewmodel_mp5k");
	XModel * asset = new XModel;
	memcpy(asset, base, sizeof(XModel));
	asset->lods[0].surfaces = new XModelSurfaces;
	memcpy(asset->lods[0].surfaces, base->lods[0].surfaces, sizeof(XModelSurfaces));

	XModelSurfaces * surf = asset->lods[0].surfaces;
	surf->name = new char[strlen(name + 6)];
	sprintf((char*)surf->name, "%s_surf", name);

	BUFFER * buf = new BUFFER(data, dataLen);
	asset->name = new char[128];
	buf->readstr(asset->name, 128);
	buf->read(&asset->numBones, 4, 1);
	buf->read(&asset->numSubBones, 4, 1);
	buf->read(&asset->numSurfaces, 4, 1);
	surf->numSurfaces = asset->numSurfaces;
	asset->lods[0].numSurfs = surf->numSurfaces;

	asset->boneNames = new short[asset->numBones];
	for(int i=0; i<asset->numBones; i++)
	{
		char bone[64];
		buf->readstr(bone, 64);
		asset->boneNames[i] = addScriptString(info, bone);
	}

	// allocate stuff and load it
	if(asset->numBones - asset->numSubBones)
	{
		asset->boneUnknown1 = new char[asset->numBones - asset->numSubBones];
		asset->tagAngles = new XModelAngle[asset->numBones - asset->numSubBones];
		asset->tagPositions = new XModelTagPos[asset->numBones - asset->numSubBones];
		buf->read(asset->boneUnknown1, sizeof(char), asset->numBones - asset->numSubBones);
		buf->read(asset->tagAngles, sizeof(XModelAngle), asset->numBones - asset->numSubBones);
		buf->read(asset->tagPositions, sizeof(XModelTagPos), asset->numBones - asset->numSubBones);
	}

	if(asset->numBones)
	{
		asset->boneUnknown4 = new char[asset->numBones];
		asset->animMatrix = new char[32 * asset->numBones];
		buf->read(asset->boneUnknown4, sizeof(char), asset->numBones);
		buf->read(asset->animMatrix, 32, asset->numBones);
	}

	surf->surfaces = new XSurface[surf->numSurfaces];
	memset(surf->surfaces, 0, sizeof(XSurface) * surf->numSurfaces);
	
	for(int i=0; i<surf->numSurfaces; i++)
	{
		XSurface* s = &surf->surfaces[i];
		buf->read(&s->numVertices, 4, 1);
		buf->read(&s->numPrimitives, 4, 1);
		buf->read(&s->blendNum1, 4, 1);
		buf->read(&s->blendNum2, 4, 1);
		buf->read(&s->blendNum3, 4, 1);
		buf->read(&s->blendNum4, 4, 1);
		int blendCount = (s->blendNum4 * 7) + (s->blendNum3 * 5) + (s->blendNum2 * 3) + s->blendNum1;
		if(blendCount)
		{
			s->blendInfo = new char[blendCount * 2];
			buf->read(&s->blendInfo, 2, blendCount);
		}
		else 
		{
			s->blendInfo = NULL;	
		}

		s->vertexBuffer = new char[32 * s->numVertices];
		buf->read(s->vertexBuffer, 32, s->numVertices);

		int ct = 0;
		buf->read(&ct, 4, 1);
		if(ct)
		{
			buf->read(&s->numCT, 4, 1);
			s->ct = new XSurfaceCT[s->numCT];
			for(int j=0; j<s->numCT; j++)
			{
				XSurfaceCT* ct = &s->ct[j];
				buf->read(&ct->pad, 4, 1);
				buf->read(&ct->pad2, 4, 1);
				ct->entry = new XSurfaceCTEntry;
				buf->read(ct->entry, 24, 1);
				buf->read(&ct->entry->numNode, 4, 1);
				buf->read(&ct->entry->numLeaf, 4, 1);
				if(ct->entry->numNode)
				{
					ct->entry->node = new char[ct->entry->numNode * 16];
					buf->read(ct->entry->node, 16, ct->entry->numNode);
				}
				else
				{
					ct->entry->node = NULL;
				}

				if(ct->entry->numLeaf)
				{
					ct->entry->leaf = new short[ct->entry->numLeaf];
					buf->read(ct->entry->leaf, 2, ct->entry->numLeaf);
				}
				else
				{
					ct->entry->node = NULL;
				}
			}
		}
		else
		{
			s->ct = NULL;
			s->numCT = 0;
		}

		s->indexBuffer = new Face[s->numPrimitives];
		buf->read(s->indexBuffer, sizeof(Face), s->numPrimitives);
	}

	asset->materials = new Material*[asset->numSurfaces];

	// read the material stuff and load a material if we need it
	for(int i=0; i<asset->numSurfaces; i++)
	{
		char * matName = new char[64];
		char * matFileName = new char[78];
		char * techName = new char[64];
		buf->readstr(matName, 50);
		buf->readstr(techName, 64);

		if(!strncmp("mc/", matName, 3)) matName = matName + 3;

		_snprintf(matFileName, 78, "materials/%s.txt", matName);

		void* matBuf;
		int len = FS_ReadFile(matFileName, &matBuf);
		if(len > 0)
		{
			asset->materials[i] = (Material*)addMaterial(info, matName, (char*)matBuf, len);
		}
		else
		{
			asset->materials[i] = (Material*)DB_FindXAssetHeader(ASSET_TYPE_MATERIAL, matName);
		}
		addAsset(info, ASSET_TYPE_MATERIAL, matName, asset->materials[i]);
	}
	int test = 0;
	buf->read(&test, 4, 1);
	if(test) Com_Error(true, "Cause NTA said so!");
	buf->read(&test, 4, 1);
	if(!test) Com_Error(true, "Cause NTA said so!");
	asset->unknowns = new char[asset->numBones * 28];
	buf->read(asset->unknowns, 28, asset->numBones);

	return asset;
}