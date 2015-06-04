#include "StdInc.h"
#include "Tool.h"

// NOTE: None of these assets can be added... only exported

void addTechset(zoneInfo_t* info, const char* name, char* data, size_t dataLen)
{
	if(dataLen != 0) { Com_Error(false, "How did we get a non bulitin techset?"); return; }

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

	addAsset(info, ASSET_TYPE_TECHSET, asset->name, asset);
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
	buf->write(data->name, strlen(data->name) + 1, 1);
	dest->name = (char*)-1;

	for(int i=0; i<48; i++)
	{
		if(!dest->techniques[i]) continue;

		MaterialTechnique* tech = (MaterialTechnique*)buf->at();
		buf->write(dest->techniques[i], sizeof(MaterialTechnique), 1);

		tech->passes[0].vertexDecl = (VertexDecl*)(vdecl[i]);
		tech->passes[0].vertexShader = (VertexShader*)(vshader[i]);
		tech->passes[0].pixelShader = (PixelShader*)(pshader[i]);

		for(int k=0; k<tech->passes[0].argCount1 + tech->passes[0].argCount2 + tech->passes[0].argCount3; k++)
		{
			buf->write(&tech->passes[0].argumentDef[k], sizeof(ShaderArgumentDef), 1);
		}

		buf->write(tech->name, strlen(tech->name) + 1, 1);

		tech->name = (char*)-1;
		tech->passes[0].argumentDef = (ShaderArgumentDef*)-1;
		dest->techniques[i] = (MaterialTechnique*)-1;
	}

}

void writePixelShader(zoneInfo_t* info, ZStream* buf, PixelShader* data)
{
	PixelShader* dest = (PixelShader*)buf->at();

	buf->write(data, sizeof(PixelShader), 1);
	buf->write(data->name, strlen(data->name) + 1, 1);
	buf->write(data->bytecode, data->codeLen * 4, 1);

	dest->name = (char*)-1;
	dest->bytecode = (DWORD*)-1;
}

void writeVertexShader(zoneInfo_t* info, ZStream* buf, VertexShader* data)
{
	VertexShader* dest = (VertexShader*)buf->at();

	buf->write(data, sizeof(VertexShader), 1);
	buf->write(data->name, strlen(data->name) + 1, 1);
	buf->write(data->bytecode, data->codeLen * 4, 1);

	dest->name = (char*)-1;
	dest->bytecode = (DWORD*)-1;
}

void writeVertexDecl(zoneInfo_t* info, ZStream* buf, VertexDecl* data)
{
	VertexDecl* dest = (VertexDecl*)buf->at();

	buf->write(data, sizeof(VertexDecl), 1);
	buf->write(data->name, strlen(data->name) + 1, 1);
	dest->name = (char*)-1;
	// seems too easy....
}
