#include "StdInc.h"

DWORD Vec2PackTexCoords(float* coords)
{
	DWORD* asInt = (DWORD*)coords;

	int v1 = ((2 * asInt[0]) ^ 0x80003FFF) >> 14;
	int v2 = (asInt[0] >> 16) & 0xC000;

	if (v1 < 16383)
	{
		if (v1 < -16384)
		{
			v1 = -16384;
		}
	}
	else
	{
		v1 = 16383;
	}

	int c = (v2 | v1) & 0x3FFF;

	int v3 = ((2 * asInt[1]) ^ 0x80003FFF) >> 14;
	int v4 = (asInt[1] >> 16) & 0xC000;

	int c2;

	if (v3 < 16383)
	{
		if (v3 < -16384)
		{
			v3 = -16384;
		}

		c2 = (v3 | v4) & 0x3FFF;
	}
	else
	{
		v3 = 16383;

		c2 = v4 & 0x3FFF;
	}

	return c2 + (c << 16);
}

float Vec3NormalizeTo(float* from, float* to)
{
	float magnitude = sqrtf(from[0] * from[0] + from[1] * from[1] + from[1] * from[1]);
	to[0] = from[0] / magnitude;
	to[1] = from[1] / magnitude;
	to[2] = from[2] / magnitude;
	return magnitude;
}

float Vec3Normalize(float* vec)
{
	return Vec3NormalizeTo(vec, vec);
}

DWORD Vec3PackUnitVec(float* vec)
{
	float n[3];
	Vec3NormalizeTo(vec, n);

	DWORD v2 = 0;
	float v15 = FLT_MAX;
	float v16 = FLT_MAX;

	char v14[4];
	memset(v14, 0, sizeof(v14));

	for (int v1 = 0; ; v1++)
	{
		double v3 = v1 + 192.0;
		float v4 = 32385.0f / v3;

		v14[0] = (n[0] * v4 + 127.5f);
		v14[1] = (n[1] * v4 + 127.5f);
		v14[2] = (n[2] * v4 + 127.5f);

		float v5 = v3 / 32385.0f;

		float v18[3];
		v18[0] = (v14[0] - 127.0f) * v5;
		v18[1] = (v14[1] - 127.0f) * v5;
		v18[2] = (v14[2] - 127.0f) * v5;

		float v13 = fabs(Vec3Normalize(v18) - 1.0f);

		if (v13 < 0.001)
		{
			float v7 = (v18[0] * n[0]) + (v18[1] * n[1]) + (v18[2] * n[2]);
			float v8 = v7 - 1.0f;
			float v9, v10 = 0;

			float v17 = fabs(v8);

			if (v17 < v15)
			{
				v9 = v17;
				v10 = v13;
			}
			else
			{
				v9 = v17;

				if (v17 != v15)
				{
					goto LABEL_6;
				}

				v10 = v13;

				if (v13 >= v16)
				{
					goto LABEL_6;
				}
			}

			v15 = v9;
			v2 = *(DWORD*)v14;
			v16 = v10;

			if ((v9 + v10) == 0)
			{
				return *(DWORD*)v14;
			}
		}

LABEL_6:
		v14[3] = v1;

		if (v1 == 0xFF)
		{
			return v2;
		}
	}
}

void addXModelSurfs(zoneInfo_t* info, const char* name, char* data, size_t dataLen)
{
	BUFFER* buf = new BUFFER(0x800000);
	XModelSurfaces * surfs = new XModelSurfaces;
	memset(surfs, 0, sizeof(XModelSurfaces));
	surfs->numSurfaces = 1;
	//surfs->pad[3] = -64;
	surfs->name = (char*)0xFFFFFFFF;
	surfs->surfaces = (XSurface*)0xFFFFFFFF;
	buf->write(surfs, sizeof(XModelSurfaces), 1);
	buf->write(name, strlen(name) + 1, 1);

	XSurface * surf = new XSurface;
	memset(surf, 0, sizeof(XSurface));
	float nrml[3] = { 1.0f, 1.0f, 1.0f };
	DWORD coords = Vec2PackTexCoords(nrml);
	DWORD normal = Vec3PackUnitVec(nrml);

	sscanlinef_init(data);
	sscanlinef("%d", &surf->numVertices);
	GfxPackedVertex * verts = new GfxPackedVertex[surf->numVertices];
	memset(verts, 0, 32 * surf->numVertices);
	for(int i=0; i<surf->numVertices; i++)
	{
		sscanlinef("%f %f %f", &verts[i].x, &verts[i].y, &verts[i].z);
		verts[i].texCoords = coords;
		verts[i].normal = normal;
		verts[i].color = -1; // white
		verts[i].unk = 1.0f;
	}

	sscanlinef("%d", &surf->numPrimitives);
	Face* tris = new Face[surf->numPrimitives];
	for(int i=0; i<surf->numPrimitives; i++)
	{
		sscanlinef("%d %d %d", &tris[i].v1, &tris[i].v2, &tris[i].v3);
	}
	surf->streamHandle = 1;
	surf->vertexBuffer = (GfxPackedVertex*)0xFFFFFFFF;
	surf->indexBuffer = (Face*)0xFFFFFFFF;

	buf->write(surf, sizeof(XSurface), 1);

	buf->write(verts, 32, surf->numVertices);
	buf->write(verts, 6, surf->numPrimitives);

	buf->resize(-1);

	addAsset(info, ASSET_TYPE_XMODELSURFS, name, buf->data(), buf->getsize());
}