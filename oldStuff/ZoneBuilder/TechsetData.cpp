#include "StdInc.h"

void addVertexShader(zoneInfo_t* info, const char* name, char* data, size_t dataLen)
{
	BUFFER* buf = new BUFFER(dataLen + 16 + strlen(name) + 1);
	buf->write(-1, 1);
	buf->write(0, 1);
	buf->write(-1, 1);
	buf->write(dataLen >> 2, 1);
	buf->write((void*)name, strlen(name) + 1, 1);
	buf->write(data, dataLen, 1);
	addAsset(info, ASSET_TYPE_VERTEXSHADER, name, buf->data(), buf->getsize());
}

void addPixelShader(zoneInfo_t* info, const char* name, char* data, size_t dataLen)
{
	BUFFER* buf = new BUFFER(dataLen + 16 + strlen(name) + 1);
	buf->write(-1, 1);
	buf->write(0, 1);
	buf->write(-1, 1);
	buf->write(dataLen >> 2, 1);
	buf->write((void*)name, strlen(name) + 1, 1);
	buf->write(data, dataLen, 1);
	addAsset(info, ASSET_TYPE_PIXELSHADER, name, buf->data(), buf->getsize());
}

void addVertexDecl(zoneInfo_t* info, const char* name, char* data, size_t dataLen)
{
	addAsset(info, ASSET_TYPE_VERTEXDECL, name, data, dataLen);
}

char* techsetBuffer;
int techsetBufferCount;

void addTechset(zoneInfo_t* info, const char* name, char* data, size_t dataLen)
{
	MaterialTechniqueSet* techset = new MaterialTechniqueSet;
	memset(techset, 0, sizeof(MaterialTechniqueSet));
	techset->name = name;

	int curTechnique;
	int passCount;
	char techName[128];
	sscanlinef_init(data);
	while(_sscanlinef("%d:%d:%s", &curTechnique, &passCount, &techName))
	{
		_sscanlinef_inc();
		techset->techniques[curTechnique] = new MaterialTechnique;
		memset(techset->techniques[curTechnique], 0, sizeof(MaterialTechnique));
		techset->techniques[curTechnique]->name = (char*)malloc(strlen(techName) + 1);
		strcpy(techset->techniques[curTechnique]->name, techName);
		techset->techniques[curTechnique]->numPasses = passCount;
		for(int i=0; i<passCount; i++)
		{
			char vertexDecl[64];
			char vertexShader[64];
			char pixelShader[64];
			sscanlinef("%s", vertexDecl);
			sscanlinef("%s", vertexShader);
			sscanlinef("%s", pixelShader);

			MaterialPass* pass = &techset->techniques[curTechnique]->passes[i];
			// vertexdecl is special
			char fname[128];

			_snprintf(fname, 128, "vertexdecl/%s", vertexDecl);
			loadAsset(info, ASSET_TYPE_VERTEXDECL, fname, vertexDecl);
			pass->vertexDecl = (VertexDecl*)(info->assetCount - 1);

			_snprintf(fname, 128, "shader_bin/%s", vertexShader);
			loadAsset(info, ASSET_TYPE_VERTEXSHADER, fname, vertexShader);
			pass->vertexShader = (VertexShader*)(info->assetCount - 1);

			_snprintf(fname, 128, "shader_bin/%s", pixelShader);
			loadAsset(info, ASSET_TYPE_PIXELSHADER, fname, pixelShader);
			pass->pixelShader = (PixelShader*)(info->assetCount - 1);

			sscanlinef("%hhd,%hhd,%hhd", &pass->argCount1, &pass->argCount2, &pass->argCount3);
			pass->argumentDef = new ShaderArgumentDef[pass->argCount1 + pass->argCount2 + pass->argCount3];
			memset(pass->argumentDef, 0, sizeof(ShaderArgumentDef) * (pass->argCount1 + pass->argCount2 + pass->argCount3));
			for(int j=0; j<pass->argCount1 + pass->argCount2 + pass->argCount3; j++)
			{
				sscanlinef("%hd,%hd,%hd,%hd", &pass->argumentDef[j].type, 
					&pass->argumentDef[j].dest, 
					&pass->argumentDef[j].paramID,
					&pass->argumentDef[j].more);
			}
		}
	}

	int asset = addAsset(info, ASSET_TYPE_TECHSET, name, NULL, 0); // add blank asset

	BUFFER* buf = new BUFFER(4096);
	MaterialTechniqueSet * set = (MaterialTechniqueSet*)buf->at();
	buf->write(techset, sizeof(MaterialTechniqueSet), 1);
	buf->write((void*)set->name, strlen(set->name) + 1, 1);
	set->name = (char*)0xFFFFFFFF;

	for(int i=0; i<48; i++)
	{
		if(!set->techniques[i]) continue;
		MaterialTechnique * tech = (MaterialTechnique*)buf->at();
		buf->write(set->techniques[i], sizeof(MaterialTechnique), 1);

		int offset = (int)&tech->passes[0].vertexDecl - (int)set;
		addFixup(info, asset, offset, (int)tech->passes[0].vertexDecl);

		offset = (int)&tech->passes[0].vertexShader - (int)set;
		addFixup(info, asset, offset, (int)tech->passes[0].vertexShader);

		offset = (int)&tech->passes[0].pixelShader - (int)set;
		addFixup(info, asset, offset, (int)tech->passes[0].pixelShader);

		for(int k=0; k<tech->passes[0].argCount1 + tech->passes[0].argCount2 + tech->passes[0].argCount3; k++)
		{
			buf->write(&tech->passes[0].argumentDef[k], sizeof(ShaderArgumentDef), 1);
		}
		tech->passes[0].argumentDef = (ShaderArgumentDef*)0xFFFFFFFF;
		set->techniques[i] = (MaterialTechnique*)0xFFFFFFFF;

		buf->write(tech->name, strlen(tech->name) + 1, 1);
		tech->name = (char*)0xFFFFFFFF;
	}
	buf->resize(-1);
	// fix the data
	setAssetData(info, asset, buf->data(), buf->getsize());
}