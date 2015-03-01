#include "StdInc.h"
#pragma once

struct XModelAngle
{
	short x;
	short y;
	short z;
	short base; // defines the 90-degree point for the shorts
};

struct XModelTagPos
{
	float x;
	float y;
	float z;
};

struct XSurfaceCTEntry
{
	char pad[24];
	int numNode;
	char* node; // el size 16
	int numLeaf;
	short* leaf;
};

struct XSurfaceCT
{
	int pad;
	int pad2;
	XSurfaceCTEntry* entry;
};

struct GfxPackedVertex
{
	float x;
	float y;
	float z;
	DWORD color;
	WORD texCoords[2];
	float normal[3];
};

struct Face
{
	short v1;
	short v2;
	short v3;
};

struct XSurface
{
	short pad; // +0
	short numVertices; // +2
	short numPrimitives; // +4
	unsigned char streamHandle; // something to do with buffers, +6
	char pad2; // +7
	int pad3; // +8
	Face* indexBuffer; // +12
	short blendNum1; // +16
	short blendNum2; // +18
	short blendNum3; // +20
	short blendNum4; // +22
	char* blendInfo; // +24
	GfxPackedVertex* vertexBuffer; // +28
	int numCT; // +32
	XSurfaceCT* ct; // +36
	char pad5[24]; // +40
	// pad5 matches XModelSurfaces pad
	// total size, 64
};

struct XModelSurfaces
{
	const char* name; // +0
	XSurface* surfaces; // +4
	int numSurfaces; // +8
	char pad[24]; // +12, matches XSurface pad5
}; // total size, 36

struct XSurfaceLod
{
	char pad[4]; // +0
	short numSurfs; // +4
	short pad2;// +6
	XModelSurfaces* surfaces; // +8
	char pad3[32]; // +12
}; // +44

struct XColSurf
{
	void* tris; // +0, sizeof 48
	int count; // +4
	char pad[36]; // +8
}; // +44

struct XModel
{
	char* name; // +0
	char numBones; // +4
	char numSubBones; // +5
	char numSurfaces; // +6
	char pad2; // +7
	char pad3[28]; // +8
	short* boneNames; // +36
	char* boneUnknown1; // +40
	XModelAngle* tagAngles; // +44, element size 8
	XModelTagPos* tagPositions; // +48, element size 12
	char* boneUnknown4; // +52
	char* animMatrix; // +56, element size 32
	Material** materials; // +60
	XSurfaceLod lods[4]; // +64
	int pad4; // +240
	XColSurf* colSurf; // +244
	int numColSurfs; // +248
	int pad6;
	char* unknowns; // bone count, +256, element size 28
	char pad5[36];
	PhysPreset* physPreset;
	struct PhysGeomList* physCollmap;
}; // total size 304