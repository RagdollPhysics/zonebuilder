#include "StdInc.h"
#include "Tool.h"



void writeXAnim(zoneInfo_t* info, BUFFER* buf, XAnim* data)
{
	WRITE_ASSET(data, XAnim);
	WRITE_NAME(data);

	WRITE_FIELD(data, tagnames,	short, tagCount);
	WRITE_FIELD(data, notetracks, Notetrack, notetrackCount);
	WRITE_FIELD(data, dataByte,	char, dataByteCount);
	WRITE_FIELD(data, dataShort, short,	dataShortCount);
	WRITE_FIELD(data, dataInt,	int, dataIntCount);
	WRITE_FIELD(data, randomDataShort, short, randomDataShortCount);
	WRITE_FIELD(data, randomDataByte, char, randomDataByteCount);
	WRITE_FIELD(data, randomDataInt, int, randomDataIntCount);
	WRITE_FIELD_ON_SIZE(data, indicies, char, indexcount);
}

void * addXAnim(zoneInfo_t* info, const char* name, char* data, size_t dataLen)
{
	if(dataLen == 0)
	{
		XAnim* a = (XAnim*)data;
		// fix these if we are dumping a pre loaded anim
		if(a->delta) 
		{
			Com_Error(true, "Cannot do anims that have delta parts!!!!");
		}
		for(int i=0; i<a->tagCount; i++)
		{
			a->tagnames[i] = addScriptString(info, SL_ConvertToString(a->tagnames[i]));
		}
		for(int i=0; i<a->notetrackCount; i++)
		{
			a->notetracks[i].name = addScriptString(info, SL_ConvertToString(a->notetracks[i].name));
		}
		return a;
	}
	BUFFER* buf = new BUFFER(data, dataLen);

	XAnim* anim = new XAnim;
	int numTags;
	int numNotetracks;
	buf->read(&numTags, 4, 1);
	buf->read(&numNotetracks, 4, 1);

	// read in the script strings
	char str[128];
	short* tagnames = new short[numTags];
	for(int i=0; i<numTags; i++)
	{
		buf->readstr(str, 128);
		tagnames[i] = addScriptString(info, str);
	}

	// read in the notetracks
	Notetrack* notetracks = new Notetrack[numNotetracks];
	for(int i=0; i<numNotetracks; i++)
	{
		buf->readstr(str, 128);
		notetracks[i].name = addScriptString(info, str);
	}

	// copy to the new struct
	memcpy(anim, buf->at(), sizeof(XAnim));
	anim->tagnames = tagnames;
	anim->notetracks = notetracks;

	buf->seek(sizeof(XAnim), SEEK_CUR);
	buf->seek(strlen(buf->at()) + 1, SEEK_CUR);
	buf->seek(numTags * 2, SEEK_CUR); // skip the data to maintain compatibility
	Notetrack * tracks = (Notetrack*)buf->at();
	for(int i=0; i<numNotetracks; i++)
	{
		anim->notetracks[i].frame = tracks[i].frame;
	}
	buf->seek(numNotetracks * 8, SEEK_CUR); // skip the data to maintain compatibility

	// load data into the struct
	anim->name = strdup(name);

	if(anim->dataByte) {
		anim->dataByte = new char[anim->dataByteCount];
		memcpy(anim->dataByte, buf->at(), anim->dataByteCount);
		buf->seek(anim->dataByteCount, SEEK_CUR);
	}

	if(anim->dataShort) {
		anim->dataShort = new short[anim->dataShortCount];
		memcpy(anim->dataShort, buf->at(), anim->dataShortCount * 2);
		buf->seek(anim->dataShortCount * 2, SEEK_CUR);
	}

	if(anim->dataInt) {
		anim->dataInt = new int[anim->dataIntCount];
		memcpy(anim->dataInt, buf->at(), anim->dataIntCount * 4);
		buf->seek(anim->dataIntCount * 4, SEEK_CUR);
	}

	if(anim->randomDataShort) {
		anim->randomDataShort = new short[anim->randomDataShortCount];
		memcpy(anim->randomDataShort, buf->at(), anim->randomDataShortCount * 2);
		buf->seek(anim->randomDataShortCount * 2, SEEK_CUR);
	}

	if(anim->randomDataByte) {
		anim->randomDataByte = new char[anim->randomDataByteCount];
		memcpy(anim->randomDataByte, buf->at(), anim->randomDataByteCount);
		buf->seek(anim->randomDataByteCount, SEEK_CUR);
	}

	if(anim->randomDataInt) {
		anim->randomDataInt = new int[anim->randomDataIntCount];
		memcpy(anim->randomDataInt, buf->at(), anim->randomDataIntCount * 4);
		buf->seek(anim->randomDataIntCount * 4, SEEK_CUR);
	}

	if(anim->indicies) {
		if(anim->indexcount < 256)
		{
			anim->indicies = new char[anim->indexcount];
			memcpy(anim->indicies, buf->at(), anim->indexcount);
		} 
		else
		{
			anim->indicies = new char[anim->indexcount * 2];
			memcpy(anim->indicies, buf->at(), anim->indexcount * 2);
		}
	}
	return anim;
}