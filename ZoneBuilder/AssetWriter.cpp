#include "StdInc.h"
#include "Tool.h"

void AssetWriter::write(void* asset, BUFFER* buf)
{
	char* data = buf->at();

	// do asset body
	buf->write(asset, _size, 1);

	// do name
	const char* name = *(const char**)asset;
	buf->write(name, strlen(name) + 1, 1);
	*(const char**)data = (const char*)-1;

	// do props
	for(list<AssetProperty*>::iterator it = _prop.begin(); it!=_prop.end(); ++it)
	{
		AssetProperty * p = *it;
		if(p->doCustomWrite(asset, buf)) { *(int*)(data + p->offset) = -1; continue; }

		if(p->getCountCustom(asset) > 0)
		{
			buf->write(*(char**)((int)asset + p->offset), p->size, p->getCountCustom(asset));
			*(int*)(data + p->offset) = -1;
		}
		else if(p->offsetOfNum != -1) // ptr
		{
			buf->write(*(char**)((int)asset + p->offset), p->size, *(int*)((int)asset + p->offsetOfNum));
			*(int*)(data + p->offset) = -1;
		}
		else
		{
			buf->write(*(char**)((int)asset + p->offset), p->size, 1);
		}
	}
}

AssetWriter* createAssetWriters()
{
	AssetWriter* writers = new AssetWriter[ASSET_TYPE_MAX];

	// Rawfile
	writers[ASSET_TYPE_RAWFILE] = AssetWriter(sizeof(Rawfile));
	writers[ASSET_TYPE_RAWFILE].addProp(
		new AssetProperty(offsetof(Rawfile, compressedData), 1, offsetof(Rawfile, sizeCompressed))
	);
	
	// Shaders
	writers[ASSET_TYPE_VERTEXSHADER] = AssetWriter(sizeof(VertexShader));
	writers[ASSET_TYPE_VERTEXSHADER].addProp(
		new AssetProperty(offsetof(VertexShader, bytecode), 4, offsetof(VertexShader, codeLen))
	);
	writers[ASSET_TYPE_PIXELSHADER] = AssetWriter(sizeof(PixelShader));
	writers[ASSET_TYPE_PIXELSHADER].addProp(
		new AssetProperty(offsetof(PixelShader, bytecode), 4, offsetof(PixelShader, codeLen))
	);
	writers[ASSET_TYPE_VERTEXDECL] = AssetWriter(sizeof(VertexDecl)); // no props here

	// Stringtable
	writers[ASSET_TYPE_STRINGTABLE] = AssetWriter(sizeof(StringTable));
	AssetProperty* stData = new AssetProperty(offsetof(StringTable, data), 1);
	stData->setCustomWrite([](void*asset, BUFFER* buf) {
		StringTable* data = (StringTable*)asset;
		for(int ri=0; ri<data->rows; ri++)
		{
			for(int ci=0; ci<data->columns; ci++)
			{
				buf->write(-1, 1);
				buf->write((int)data->data[((ri * data->columns) + ci) * 2 + 1], 1);
			}
		}
		for(int ri=0; ri<data->rows; ri++)
		{
			for(int ci=0; ci<data->columns; ci++)
			{
				buf->write(data->data[((ri * data->columns) + ci) * 2], strlen(data->data[((ri * data->columns) + ci) * 2]) + 1, 1);
			}
		}
		return true;
	});
	writers[ASSET_TYPE_STRINGTABLE].addProp(stData);

	return writers;
}