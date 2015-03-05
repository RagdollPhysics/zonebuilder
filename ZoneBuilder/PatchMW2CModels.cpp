// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: Model loading code for 10c/Raven FS surfaces.
//
// Initial author: NTAuthority
// Started: 2012-12-07
// ==========================================================

#include "StdInc.h"
#include <d3d9.h>
#include "Hooking.h"
#include "Tool.h"

static unordered_map<DWORD, void*> bufferMap;

FS_FOpenFileRead_t FS_FOpenFileReadDatabase = (FS_FOpenFileRead_t)0x42ECA0;

void FS_FOpenFileReadCurrentThread(char* filename, int* handle)
{
	if (GetCurrentThreadId() == *(DWORD*)0x1CDE7FC)
	{
		FS_FOpenFileRead(filename, handle, 0);
	}
	else if (GetCurrentThreadId() == *(DWORD*)0x1CDE814)
	{
		FS_FOpenFileReadDatabase(filename, handle, 0);
	}
	else
	{
		*handle = NULL;
	}
}

struct CModelSectionHeader
{
	int size;
	int offset;
	int fixupStart;
	int fixupCount;
	char* buffer;
};

struct CModelHeader
{
	int version;
	unsigned int signature;
	CModelSectionHeader main;
	CModelSectionHeader index;
	CModelSectionHeader vertex;
	CModelSectionHeader fixup;
};

void ReadCModelSection(int handle, CModelSectionHeader& header)
{
	FS_Seek(handle, header.offset, FS_SEEK_SET);
	FS_Read(header.buffer, header.size, handle);
}

void FixupCModelSection(CModelHeader& header, CModelSectionHeader& section, DWORD* fixups)
{
	for (int i = section.fixupStart; i < section.fixupStart + section.fixupCount; i++)
	{
		DWORD fixup = fixups[i];
		int targetSectionNum = fixup & 3;
		CModelSectionHeader* targetSection;

		if (targetSectionNum == 0)
		{
			targetSection = &header.main;
		}
		else if (targetSectionNum == 1)
		{
			targetSection = &header.index;
		}
		else if (targetSectionNum == 2)
		{
			targetSection = &header.vertex;
		}

		*(DWORD*)(section.buffer + (fixup >> 3)) += (DWORD)targetSection->buffer;
	}
}

void Load_VertexBuffer(void* data, void** where, int len)
{
	DWORD func = 0x5112C0;

	__asm
	{
		push edi

		mov eax, len
		mov edi, where
		push data

		call func

		add esp, 4
		pop edi
	}
}

typedef void* (__cdecl * R_AllocStaticIndexBuffer_t)(void** store, int length);
R_AllocStaticIndexBuffer_t R_AllocStaticIndexBuffer = (R_AllocStaticIndexBuffer_t)0x51E7A0;

void Load_IndexBuffer(void* data, void** storeHere, int count)
{
	static dvar_t* r_loadForRenderer = *(dvar_t**)0x69F0ED4;

	if (r_loadForRenderer->current.boolean)
	{
		void* buffer = R_AllocStaticIndexBuffer(storeHere, 2 * count);
		memcpy(buffer, data, 2 * count);

		__asm
		{
			push ecx
			mov ecx, storeHere
			mov ecx, [ecx]

			mov eax, [ecx]
			add eax, 30h
			mov eax, [eax]

			push ecx
			call eax

			pop ecx
		}
	}
}

void CreateCModelBuffers(void* surfs)
{
	XModelSurfaces* model = (XModelSurfaces*)surfs;

	for (int i = 0; i < model->numSurfaces; i++)
	{
		XSurface* surface = &model->surfaces[i];

		void* vertexBuffer;
		void* indexBuffer;

		Load_VertexBuffer(surface->vertexBuffer, &vertexBuffer, surface->numVertices * 32);
		Load_IndexBuffer(surface->indexBuffer, &indexBuffer, surface->numPrimitives * 3);

		bufferMap[(DWORD)surface->vertexBuffer] = vertexBuffer;
		bufferMap[(DWORD)surface->indexBuffer] = indexBuffer;
	}
}

struct CModelAllocData
{
	void* mainArray;
	void* vertexBuffer;
	void* indexBuffer;
};

static unordered_map<DWORD, CModelAllocData*> allocData;

char* LoadCModel(const char* name)
{
	char filename[512];
	sprintf_s(filename, sizeof(filename), "models/%s", name);

	int handle;
	FS_FOpenFileReadCurrentThread(filename, &handle);

	if (handle <= 0)
	{
		Com_Error(1, "Error opening %s", filename);
	}

	CModelHeader header;

	if (FS_Read(&header, sizeof(header), handle) != sizeof(header))
	{
		FS_FCloseFile(handle);
		Com_Error(1, "%s: header could not be read", filename);
	}

	if (header.version != 1)
	{
		FS_FCloseFile(handle);
		Com_Error(1, "%s: header version is not '1'", filename);
	}

	static DWORD fixups[4096];

	if (header.fixup.size >= sizeof(fixups))
	{
		FS_FCloseFile(handle);
		Com_Error(1, "%s: fixup size too big", filename);
	}

	header.main.buffer = (char*)malloc(header.main.size);
	header.index.buffer = (char*)_aligned_malloc(header.index.size, 16);
	header.vertex.buffer = (char*)_aligned_malloc(header.vertex.size, 16);
	header.fixup.buffer = (char*)fixups;

	ReadCModelSection(handle, header.main);
	ReadCModelSection(handle, header.index);
	ReadCModelSection(handle, header.vertex);
	ReadCModelSection(handle, header.fixup);

	FS_FCloseFile(handle);

	FixupCModelSection(header, header.main, fixups);
	FixupCModelSection(header, header.index, fixups);
	FixupCModelSection(header, header.vertex, fixups);

	CModelAllocData* allocationData = (CModelAllocData*)malloc(sizeof(CModelAllocData));
	allocationData->mainArray = header.main.buffer;
	allocationData->indexBuffer = header.index.buffer;
	allocationData->vertexBuffer = header.vertex.buffer;

	*(void**)(header.main.buffer + 44) = allocationData;

	// maybe the +36/48 = 0 stuff here?

	// move buffers to work with iw4
	int numSurfaces = *(short*)(header.main.buffer + 8);

	char* tempSurface = new char[84 * numSurfaces];
	char* surface = *(char**)(header.main.buffer + 4);

	for (int i = 0; i < numSurfaces; i++)
	{
		char* source = &surface[84 * i];
		char* dest = &tempSurface[64 * i];

		memcpy(dest, source, 12);
		memcpy(dest + 12, source + 16, 20);
		memcpy(dest + 32, source + 40, 8);
		memcpy(dest + 40, source + 52, 24);

		dest[6] = 0xFF; // fake stream handle for the vertex/index buffer get code to use
	}

	memcpy(surface, tempSurface, 84 * numSurfaces);

	delete[] tempSurface;

	// create vertex/index buffers
	CreateCModelBuffers(header.main.buffer);

	// store the buffer bit
	allocData[(DWORD)header.vertex.buffer] = allocationData;

	return header.main.buffer;
}

void CModel_Test_f()
{
	if (Cmd_Argc() < 2)
	{
		return;
	}

	char* cmodel = LoadCModel(Cmd_Argv(1));
	char* xmodel = (char*)DB_FindXAssetHeader(ASSET_TYPE_XMODELSURFS, Cmd_Argv(1));

	if (!cmodel || !xmodel)
	{
		Com_Printf(0, "no such thing in at least one of the two databases\n");
		return;
	}

	int xsize = 0x24;
	int csize = 0x30;

	Com_Printf(0, "x           c\n");

	for (int i = 0; i < max(xsize, csize); i += 4)
	{
		Com_Printf(0, "%08x    %08x\n", (i < xsize) ? *(DWORD*)(xmodel + i) : 0x101c0de, (i < csize) ? *(DWORD*)(cmodel + i) : 0x101c0de);
	}

	xmodel = *(char**)(xmodel + 4);
	cmodel = *(char**)(cmodel + 4);

	xsize = 64;
	csize = 84;

	Com_Printf(0, "x           c\n");

	for (int i = 0; i < max(xsize, csize); i += 4)
	{
		Com_Printf(0, "%08x    %08x\n", (i < xsize) ? *(DWORD*)(xmodel + i) : 0x101c0de, (i < csize) ? *(DWORD*)(cmodel + i) : 0x101c0de);
	}
}

CallHook loadXModelAssetHook;
DWORD loadXModelAssetHookLoc = 0x47A6BD;

extern std::unordered_map<std::string, bool> _allowedAssetMap;

bool Load_XModelAssetHookFunc(char* xmodel)
{
	bool didStuff = false;
	short totalv = 0;

	for (int i = 0; i < xmodel[241]; i++)
	{
		XModelSurfaces* surfs = *(XModelSurfaces**)(xmodel + 72 + (44 * i));

		if (!surfs->surfaces)
		{
			char* newSurfs = LoadCModel(surfs->name);

			memcpy(xmodel + 76 + (44 * i), &newSurfs[12], 24);
			memcpy(xmodel + 100 + (44 * i), &newSurfs[4], 4);

			short v = *(short*)(newSurfs + 8);
			*(short*)(xmodel + 68 + (44 * i)) = v;
			*(short*)(xmodel + 70 + (44 * i)) = totalv;

			totalv += v;

			surfs->numSurfaces = ((XModelSurfaces*)newSurfs)->numSurfaces;
			surfs->surfaces = ((XModelSurfaces*)newSurfs)->surfaces;

			didStuff = true;
		}
	}

	return didStuff;
}

#pragma optimize("", off)
void __declspec(naked) Load_XModelAssetHookStub()
{
	__asm
	{
		mov eax, [esp + 4]
		push eax
		call Load_XModelAssetHookFunc
		add esp, 4h

		cmp al, al
		jnz justReturn

		retn

justReturn:
		jmp loadXModelAssetHook.pOriginal
	}
}
#pragma optimize("", on)

StompHook getIndexBufferHook;
DWORD getIndexBufferHookLoc = 0x4B4DE0;
DWORD getIndexBufferHookRet = 0x4B4DE5;

void GetIndexBufferHookFunc(char streamHandle, void* buffer, void** bufferOut, int* offsetOut)
{
	*offsetOut = 0;
	*bufferOut = bufferMap[(DWORD)buffer];//buffer;
}

#pragma optimize("", off)
void __declspec(naked) GetIndexBufferHookStub()
{
	__asm
	{
		mov eax, [esp + 4h]
		cmp al, 0FFh

		je handleOwn
		movzx eax, [esp + 4h]
		jmp getIndexBufferHookRet

handleOwn:
		jmp GetIndexBufferHookFunc
	}
}
#pragma optimize("", on)

StompHook getVertexBufferHook;
DWORD getVertexBufferHookLoc = 0x5BC050;
DWORD getVertexBufferHookRet = 0x5BC055;

void GetVertexBufferHookFunc(char streamHandle, void* buffer, void** bufferOut, int* offsetOut)
{
	*offsetOut = 0;
	*bufferOut = bufferMap[(DWORD)buffer];//buffer;
}

#pragma optimize("", off)
void __declspec(naked) GetVertexBufferHookStub()
{
	__asm
	{
		mov eax, [esp + 4h]
		cmp al, 0FFh

		je handleOwnVertex
		movzx eax, [esp + 4h]
		jmp getVertexBufferHookRet

handleOwnVertex:
		jmp GetVertexBufferHookFunc
	}
}

/*CallHook getIndexBuffer2Hook;
DWORD getIndexBuffer2HookLoc = 0x558F12;

void* GetIndexBuffer2HookFunc(char streamHandle, void* buffer)
{
	return buffer;
}

void __declspec(naked) GetIndexBuffer2HookStub()
{
	__asm
	{
		mov eax, [esp + 4h]
		cmp al, 0FFh

		je handleOwn
		movzx eax, [esp + 4h]
		jmp getIndexBuffer2Hook.pOriginal

handleOwn:
		jmp GetIndexBuffer2HookFunc
	}
}*/

CallHook getIndexBuffer3Hook;
DWORD getIndexBuffer3HookLoc = 0x558E70;

void* GetIndexBuffer3HookFunc(DWORD buffer)
{
	return bufferMap[buffer];
}

void __declspec(naked) GetIndexBuffer3HookStub()
{
	__asm
	{
		mov eax, [esp + 4h]
		cmp al, 0FFh

		je handleOwn
		movzx eax, [esp + 4h]
		jmp getIndexBuffer3Hook.pOriginal

handleOwn:
		mov eax, [edi + 0Ch]
		push eax
		call GetIndexBuffer3HookFunc
		add esp, 4h
		retn
	}
}

CallHook getIndexBaseHook;
DWORD getIndexBaseHookLoc = 0x558F12;

void __declspec(naked) GetIndexBaseHookStub()
{
	__asm
	{
		mov eax, [esp + 4h]
		cmp al, 0FFh

		je handleOwn
		jmp getIndexBaseHook.pOriginal

handleOwn:
		xor eax, eax
		retn
	}
}
#pragma optimize("", on)

typedef char* (__cdecl * SL_ConvertToString_t)(unsigned short);
extern SL_ConvertToString_t SL_ConvertToString;

void XModel_ListTags_f()
{
	if (Cmd_Argc() < 2)
	{
		return;
	}

	char* xmodel = (char*)DB_FindXAssetHeader(ASSET_TYPE_XMODEL, Cmd_Argv(1));

	int numTags = xmodel[4];

	Com_Printf(0, "%i tags:\n", numTags);
	
	short* tagNames = *(short**)(xmodel + 36);

	for (int i = 0; i < numTags; i++)
	{
		Com_Printf(0, "%s\n", SL_ConvertToString(tagNames[i]));
	}
}

CallHook getBoneIndexHook1;
DWORD getBoneIndexHook1Loc = 0x64F63F;

CallHook getBoneIndexHook2;
DWORD getBoneIndexHook2Loc = 0x64F783;

void LogBoneIndexGet(int loopIdx, int counter, XModel** modelList, short name, XModel* model)
{
	const char* tag = SL_ConvertToString(name);

	OutputDebugString(va("get %s on %s (%i, loopIdx = %i)\n", tag, model->name, counter, loopIdx));

	for (int i = 0; i < counter; i++)
	{
		int flags = (DWORD)modelList[(i * 2) + 1];
		OutputDebugString(va("%i: %s (%i, %i, %s)\n", i, modelList[i * 2]->name, flags & 0xFFFF, flags >> 16, (flags & 0xFFFF) ? SL_ConvertToString(flags & 0xFFFF) : "neither"));
	}
}

#pragma optimize("", off)
void __declspec(naked) GetBoneIndexHookStub()
{
	__asm
	{
		mov ecx, [esp + 7D8h]

		mov eax, [esp + 4]
		push eax

		mov eax, [esp + 4 + 8]
		push eax

		mov eax, [esp + 7DCh]
		push eax

		push ecx

		push ebp

		call LogBoneIndexGet

		add esp, 14h

		jmp getBoneIndexHook1.pOriginal
	}
}
#pragma optimize("", on)

typedef struct DObjModel_s
{
	XModel* model;
	short attachTag;
	short someFlag;
} DObjModel_t;

static int hideBitsMap[192];

int CL_DObjCreateHookFunc(DObjModel_t* models, int count, int a3, int a4, int a5, int a6)
{
	static DObjModel_t newModels[32];
	static DObjModel_t tempModels[4];
	static int tempBit[4];
	int newI = 0;
	int tempI = 0;
	int newBit = 0;
	int oldBit = 0;

	for (int i = 0; i < count; i++)
	{
		if (models[i].attachTag)
		{
			const char* tag = SL_ConvertToString(models[i].attachTag);

			if (!_strnicmp(tag, "tag_weapon_", 11))
			{
				tempBit[tempI] = oldBit;
				oldBit += models[i].model->numBones;

				tempModels[tempI] = models[i];
				tempI++;
				continue;
			}
		}

		for (int j = 0; j < models[i].model->numBones; j++)
		{
			hideBitsMap[newBit] = oldBit;
			oldBit++;
			newBit++;
		}

		newModels[newI] = models[i];
		newI++;
	}

	if (tempI)
	{
		for (int i = 0; i < tempI; i++)
		{
			newModels[newI] = tempModels[i];
			newI++;

			int startBit = tempBit[i];

			for (int j = 0; j < tempModels[i].model->numBones; j++)
			{
				hideBitsMap[newBit] = startBit;
				newBit++;
				startBit++;
			}
		}
	}

	// call the actual function
	return ((int (__cdecl *)(DObjModel_t* models, int count, int a3, int a4, int a5, int a6))0x589DA0)(newModels, newI, a3, a4, a5, a6);
}

CallHook updateDObjSetBitsHook;
DWORD updateDObjSetBitsHookLoc = 0x56D60E;

void UpdateDObjSetBitsHookFunc(DWORD* bits)
{
#define GET_BIT(b, x) ((b[(x) >> 5]) & (0x80000000 >> ((x) & 31)))
#define SET_BIT(b, x) ((b[(x) >> 5]) |= (0x80000000 >> ((x) & 31)))

	DWORD newBits[6];

	for (int i = 0; i < 192; i++)
	{
		int newBit = hideBitsMap[i];

		if (GET_BIT(bits, i))
		{
			SET_BIT(newBits, newBit);
		}
	}

	memcpy(bits, newBits, sizeof(newBits));

#undef GET_BIT
#undef SET_BIT
}

#pragma optimize("", off)
void __declspec(naked) UpdateDObjSetBitsHookStub()
{
	__asm
	{
		mov eax, [esp + 8h]
		push eax
		call UpdateDObjSetBitsHookFunc
		add esp, 4h

		jmp updateDObjSetBitsHook.pOriginal
	}
}
#pragma optimize("", on)

void DB_RemoveXModelSurfaces(XModelSurfaces* model)
{
	if (model->numSurfaces > 0)
	{
		for (int i = 0; i < model->numSurfaces; i++)
		{
			XSurface* surface = &model->surfaces[i];

			if (!IsBadReadPtr(surface, 4) && surface->streamHandle == 0xFF)
			{
				unordered_map<DWORD, void*>::iterator it = bufferMap.find((DWORD)surface->indexBuffer);

				if (it != bufferMap.end())
				{
					((IDirect3DIndexBuffer9*)(*it).second)->Release();
					bufferMap.erase(it);
				}

				it = bufferMap.find((DWORD)surface->vertexBuffer);

				if (it != bufferMap.end())
				{
					((IDirect3DVertexBuffer9*)(*it).second)->Release();
					bufferMap.erase(it);
				}

				DWORD allocIdx = (DWORD)surface->vertexBuffer; // save this as we free the alloc struct later on

				unordered_map<DWORD, CModelAllocData*>::iterator ait = allocData.find(allocIdx);

				if (ait != allocData.end())
				{
					CModelAllocData* alloc = (*ait).second;
					_aligned_free(alloc->indexBuffer);
					_aligned_free(alloc->vertexBuffer);
					free(alloc->mainArray);
					free(alloc);

					allocData.erase(ait);
				}
			}
		}
	}
}

void PatchMW2_CModels()
{

	static cmd_function_t cmodel_test;
	static cmd_function_t xmListTags;

	Cmd_AddCommand("cmodel_test", CModel_Test_f, &cmodel_test, 0);
	Cmd_AddCommand("xmListTags", XModel_ListTags_f, &xmListTags, 0);

	loadXModelAssetHook.initialize(loadXModelAssetHookLoc, Load_XModelAssetHookStub);
	loadXModelAssetHook.installHook();

	getIndexBufferHook.initialize(getIndexBufferHookLoc, GetIndexBufferHookStub);
	getIndexBufferHook.installHook();

	//getIndexBuffer2Hook.initialize(getIndexBuffer2HookLoc, GetIndexBuffer2HookStub);
	//getIndexBuffer2Hook.installHook();

	getIndexBuffer3Hook.initialize(getIndexBuffer3HookLoc, GetIndexBuffer3HookStub);
	getIndexBuffer3Hook.installHook();

	getIndexBaseHook.initialize(getIndexBaseHookLoc, GetIndexBaseHookStub);
	getIndexBaseHook.installHook();

	getVertexBufferHook.initialize(getVertexBufferHookLoc, GetVertexBufferHookStub);
	getVertexBufferHook.installHook();

	*(DWORD*)0x799AC4 = (DWORD)DB_RemoveXModelSurfaces;

	//getBoneIndexHook1.initialize(getBoneIndexHook1Loc, GetBoneIndexHookStub);
	//getBoneIndexHook1.installHook();

	//*(void**)0x4E3409 = CL_DObjCreateHookFunc;

	//updateDObjSetBitsHook.initialize(updateDObjSetBitsHookLoc, UpdateDObjSetBitsHookStub);
	//updateDObjSetBitsHook.installHook();

	//getBoneIndexHook2.initialize(getBoneIndexHook2Loc, GetBoneIndexHookStub);
	//getBoneIndexHook2.installHook();
}

typedef void (__cdecl * DB_EnumXAssets_t)(assetType_t type, void (*handler)(void*, int), int userData);
static DB_EnumXAssets_t DB_EnumXAssets = (DB_EnumXAssets_t)0x42A770;

void ReleaseCModels()
{
	for (unordered_map<DWORD, void*>::const_iterator iter = bufferMap.begin(); iter != bufferMap.end(); iter++)
	{
		((IUnknown*)iter->second)->Release();
	}

	bufferMap.clear();
}

void ReloadCModel(void* varModel, int isRelease)
{
	CreateCModelBuffers(varModel);
}

void ReloadCModels()
{
	DB_EnumXAssets(ASSET_TYPE_XMODELSURFS, ReloadCModel, 0);
}