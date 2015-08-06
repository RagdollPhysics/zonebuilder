#include "StdInc.h"
#include "Tool.h"

// NOTE: None of these assets can be added... only exported

void* addTechset(zoneInfo_t* info, const char* name, char* data, int dataLen)
{
	if (dataLen > 0) { Com_Error(false, "Can't create Techsets directly. Use Materials\n"); return NULL; }

	MaterialTechniqueSet* asset = (MaterialTechniqueSet*)data;

	for(int i=0; i<48; i++)
	{
		MaterialTechnique* tech = asset->techniques[i];
		if(!tech) continue;

		if(asset->techniques[i]->numPasses != 1) Com_Error(true, "Um why does this technique have more than 1 pass?");
		addAsset(info, ASSET_TYPE_VERTEXDECL, tech->passes[0].vertexDecl->name, tech->passes[0].vertexDecl);
		addAsset(info, ASSET_TYPE_VERTEXSHADER, tech->passes[0].vertexShader->name, tech->passes[0].vertexShader);
		addAsset(info, ASSET_TYPE_PIXELSHADER, tech->passes[0].pixelShader->name, tech->passes[0].pixelShader);		
	}

	return data;
}


void writeTechset(zoneInfo_t* info, ZStream* buf, MaterialTechniqueSet* data)
{
	// get all our pointers straight
	int vshader[48];
	int pshader[48];
	int vdecl[48];

	memset(&vshader, 0, 48 * 4);
	memset(&pshader, 0, 48 * 4);
	memset(&vdecl, 0, 48 * 4);

	for(int i=0; i<48; i++)
	{
		MaterialTechnique* tech = data->techniques[i];
		if(!tech) continue;

		vdecl[i] = requireAsset(info, ASSET_TYPE_VERTEXDECL, (char*)tech->passes[0].vertexDecl->name, buf);
		vshader[i] = requireAsset(info, ASSET_TYPE_VERTEXSHADER, (char*)tech->passes[0].vertexShader->name, buf);
		pshader[i] = requireAsset(info, ASSET_TYPE_PIXELSHADER, (char*)tech->passes[0].pixelShader->name, buf);
	}

	MaterialTechniqueSet * dest = (MaterialTechniqueSet*)buf->at();
	buf->write(data, sizeof(MaterialTechniqueSet), 1);
	buf->pushStream(ZSTREAM_VIRTUAL);

	buf->write(data->name, strlen(data->name) + 1, 1);
	dest->name = (char*)-1;
	dest->remappedTechniques = NULL;

	for(int i=0; i<48; i++)
	{
		if(!dest->techniques[i]) continue;

		if (dest->techniques[i]->numPasses > 1) Com_Error(true, "Error in techset %s: more than one pass in technique?\n", data->name);

		buf->align(ALIGN_TO_4);
		MaterialTechnique* tech = (MaterialTechnique*)buf->at();
		buf->write(dest->techniques[i], sizeof(MaterialTechnique), 1);

		tech->passes[0].vertexDecl = (VertexDecl*)(vdecl[i]);
		tech->passes[0].vertexShader = (VertexShader*)(vshader[i]);
		tech->passes[0].pixelShader = (PixelShader*)(pshader[i]);

		if (tech->passes[0].argumentDef)
		{
			buf->align(ALIGN_TO_4);
			for (int k = 0; k < tech->passes[0].argCount1 + tech->passes[0].argCount2 + tech->passes[0].argCount3; k++)
			{
				buf->write(&tech->passes[0].argumentDef[k], sizeof(ShaderArgumentDef), 1);
			}
		}

		buf->write(tech->name, strlen(tech->name) + 1, 1);

		tech->name = (char*)-1;
		tech->passes[0].argumentDef = (ShaderArgumentDef*)-1;
		dest->techniques[i] = (MaterialTechnique*)-1;
	}
	buf->popStream();
}

void* addPixelShader(zoneInfo_t* info, const char* name, char* data, int dataLen)
{
	if (dataLen > 0) { Com_Error(false, "Can't create PixelShaders directly. Use Materials\n"); return NULL; }
	return data;
}

void writePixelShader(zoneInfo_t* info, ZStream* buf, PixelShader* data)
{
	WRITE_ASSET(data, PixelShader);
	buf->pushStream(ZSTREAM_VIRTUAL);
	WRITE_NAME(data);

	WRITE_FIELD_ALIGNED(data, bytecode, DWORD, codeLen, ALIGN_TO_4);
	buf->popStream();
}

void* addVertexShader(zoneInfo_t* info, const char* name, char* data, int dataLen)
{
	if (dataLen > 0) { Com_Error(false, "Can't create VertexShaders directly. Use Materials\n"); return NULL; }
	return data;
}

void writeVertexShader(zoneInfo_t* info, ZStream* buf, VertexShader* data)
{
	WRITE_ASSET(data, VertexShader);
	buf->pushStream(ZSTREAM_VIRTUAL);
	WRITE_NAME(data);

	WRITE_FIELD_ALIGNED(data, bytecode, DWORD, codeLen, ALIGN_TO_4);
	buf->popStream();
}

void* addVertexDecl(zoneInfo_t* info, const char* name, char* data, int dataLen)
{
	if (dataLen > 0) { Com_Error(false, "Can't create VertexDecl's directly. Use Materials\n"); return NULL; }
	return data;
}

void writeVertexDecl(zoneInfo_t* info, ZStream* buf, VertexDecl* data)
{
	VertexDecl* dest = (VertexDecl*)buf->at();
	buf->write(data, sizeof(VertexDecl), 1);
	buf->pushStream(ZSTREAM_VIRTUAL);

	buf->write(data->name, strlen(data->name) + 1, 1);
	dest->name = (char*)-1;

	buf->popStream();
}
