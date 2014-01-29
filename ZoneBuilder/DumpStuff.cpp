#include "StdInc.h"
#include "Hooking.h"
#include <direct.h>

void fwritestr(FILE* file, const char* str)
{
	while (*str)
	{
		fwrite(str, 1, 1, file);

		str++;
	}

	fwrite(str, 1, 1, file);
}

void fwriteint(FILE* file, int value)
{
	fwrite(&value, 4, 1, file);
}

void dumpModel(char * name)
{
	XModel* mod = (XModel*)DB_FindXAssetHeader(ASSET_TYPE_XMODEL, name);

	char path[256];
	_snprintf(path, 256, "zonebuilder/%s.xme", name);
	FILE* f = fopen(path, "wb");

	fwritestr(f, mod->name);
	fwriteint(f, mod->numBones);
	fwriteint(f, mod->numSubBones);
	fwriteint(f, mod->numSurfaces);

	for (int i = 0; i < mod->numBones; i++)
	{
		fwritestr(f, SL_ConvertToString(mod->boneNames[i]));
	}

	fwrite(mod->boneUnknown1, 1, (mod->numBones - mod->numSubBones), f);

	fwrite(mod->tagAngles, sizeof(XModelAngle), (mod->numBones - mod->numSubBones), f);

	fwrite(mod->tagPositions, sizeof(XModelTagPos), (mod->numBones - mod->numSubBones), f);

	fwrite(mod->boneUnknown4, 1, mod->numBones, f);

	fwrite(mod->animMatrix, 32, mod->numBones, f);

	for (int i = 0; i < mod->numSurfaces; i++)
	{
		XSurface* surf = &mod->lods[0].surfaces->surfaces[i];
		fwriteint(f, surf->numVertices);
		fwriteint(f, surf->numPrimitives);
		fwriteint(f, surf->blendNum1);
		fwriteint(f, surf->blendNum2);
		fwriteint(f, surf->blendNum3);
		fwriteint(f, surf->blendNum4);

		fwrite(surf->blendInfo, 2, surf->blendNum1 + (3 * surf->blendNum2) + (5 * surf->blendNum3) + (7 * surf->blendNum4), f);

		fwrite(surf->vertexBuffer, 32, surf->numVertices, f);

		fwriteint(f, (surf->ct) ? 1 : 0);

		if (surf->ct)
		{
			fwriteint(f, surf->numCT);

			for (int i = 0; i < surf->numCT; i++)
			{
				XSurfaceCT* ct = &surf->ct[i];
				fwriteint(f, ct->pad);
				fwriteint(f, ct->pad2);

				XSurfaceCTEntry* entry = ct->entry;

				fwrite(entry->pad, 1, sizeof(entry->pad), f);

				fwriteint(f, entry->numNode);
				fwriteint(f, entry->numLeaf);

				fwrite(entry->node, 16, entry->numNode, f);
				fwrite(entry->leaf, 2, entry->numLeaf, f);
			}
		}

		fwrite(surf->indexBuffer, 6, surf->numPrimitives, f);
	}

	for (int i = 0; i < mod->numSurfaces; i++)
	{
		fwritestr(f, mod->materials[i]->name);
		fwritestr(f, mod->materials[i]->techniqueSet->name);
	}

	fwriteint(f, (mod->colSurf) ? 1 : 0);
	if(mod->colSurf)
	{
		XColSurf* colSurf = mod->colSurf;
		DebugBreak(); // i dont think this is supported
	}

	fwriteint(f, (mod->unknowns) ? 1 : 0);

	if (mod->unknowns)
	{
		for (int i = 0; i < mod->numBones; i++)
		{
			float thisUnknowns[7];
			float* unknown = (float*)&mod->unknowns[40 * i];

			thisUnknowns[0] = unknown[0];
			thisUnknowns[1] = unknown[1];
			thisUnknowns[2] = unknown[2];
			thisUnknowns[3] = unknown[3];
			thisUnknowns[4] = unknown[4];
			thisUnknowns[5] = unknown[5];
			thisUnknowns[6] = unknown[9];

			fwrite(thisUnknowns, sizeof(float), 7, f);
		}
	}
	fclose(f);
}