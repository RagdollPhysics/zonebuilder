#include "StdInc.h"
#include "Tool.h"

void writeXAnimDeltaParts(zoneInfo_t* info, ZStream* buf, XAnim* parts)
{
	XAnimDeltaPart* data = parts->delta;
	XAnimDeltaPart* dest = (XAnimDeltaPart*)buf->at();
	buf->write(data, sizeof(XAnimDeltaPart), 1);

	if (data->trans)
	{
		buf->write(data->trans, 4, 1); // not full struct
		if (data->trans->size)
		{
			XAnimPartTransFrames* frames = (XAnimPartTransFrames*)buf->at();
			buf->write(&data->trans->u, 0x1C, 1); // not full struct
			if (parts->framecount > 0x100)
			{
				buf->write(&data->trans->u.frames.indices, sizeof(short), data->trans->size + 1);
			}
			else
			{
				buf->write(&data->trans->u.frames.indices, sizeof(char), data->trans->size + 1);
			}

			if (frames->frames._1)
			{
				if (data->trans->smallTrans)
					buf->write(data->trans->u.frames.frames._1, sizeof(char) * 3, data->trans->size + 1);
				else
					buf->write(data->trans->u.frames.frames._2, sizeof(short) * 3, data->trans->size + 1);
			}
		}
		else
		{
			buf->write(data->trans->u.frame0, sizeof(float), 3);
		}
		dest->trans = (XAnimPartTrans*)-1;
	}

	if (data->quat2)
	{
		buf->write(data->quat2, 4, 1); // not full struct
		if (data->quat2->size)
		{
			if (parts->framecount > 0x100)
			{
				buf->write(&data->quat2->u.frames.indices, sizeof(short), data->quat2->size + 1);
			}
			else
			{
				buf->write(&data->quat2->u.frames.indices, sizeof(char), data->quat2->size + 1);
			}

			if (data->quat2->u.frames.frames)
			{
				buf->write(data->quat2->u.frames.frames, sizeof(short) * 2, data->quat2->size + 1);
			}
		}
		else
		{
			buf->write(data->quat2->u.frame0, sizeof(short) * 2, 1);
		}
		dest->quat2 = (XAnimDeltaPartQuat2*)-1;
	}

	if (data->quat)
	{
		buf->write(data->quat, 4, 1);
		if (data->quat->size)
		{
			if (parts->framecount > 0x100)
			{
				buf->write(&data->quat->u.frames.indices, sizeof(short), data->quat->size + 1);
			}
			else
			{
				buf->write(&data->quat->u.frames.indices, sizeof(char), data->quat->size + 1);
			}

			if (data->quat->u.frames.frames)
			{
				buf->write(data->quat->u.frames.frames, sizeof(short) * 2, data->quat->size + 1);
			}
		}
		else
		{
			buf->write(data->quat->u.frame0, sizeof(short) * 4, 1);
		}
		dest->quat = (XAnimDeltaPartQuat*)-1;
	}
}

void writeXAnim(zoneInfo_t* info, ZStream* buf, XAnim* data)
{
	WRITE_ASSET(data, XAnim);
	WRITE_NAME(data);

	WRITE_FIELD(data, tagnames,	short, tagCount);
	WRITE_FIELD(data, notetracks, XAnimNotifyInfo, notetrackCount);
	if (data->delta)
	{
		writeXAnimDeltaParts(info, buf, data);
		dest->delta = (XAnimDeltaPart*)-1;
	}
	WRITE_FIELD(data, dataByte,	char, dataByteCount);
	WRITE_FIELD(data, dataShort, short,	dataShortCount);
	WRITE_FIELD(data, dataInt,	int, dataIntCount);
	WRITE_FIELD(data, randomDataShort, short, randomDataShortCount);
	WRITE_FIELD(data, randomDataByte, char, randomDataByteCount);
	WRITE_FIELD(data, randomDataInt, int, randomDataIntCount);
	if (data->framecount > 0x100)
	{
		buf->write(&data->indicies->data, sizeof(short), data->indexcount);
	}
	else
	{
		buf->write(&data->indicies->data, sizeof(char), data->indexcount);
	}
	//WRITE_FIELD_ON_SIZE(data, indicies->data, char, indexcount);
}

void * addXAnim(zoneInfo_t* info, const char* name, char* data, size_t dataLen)
{
	if(dataLen == 0)
	{
		XAnim* a = (XAnim*)data;
		// fix these if we are dumping a pre loaded anim
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
	XAnimNotifyInfo* notetracks = new XAnimNotifyInfo[numNotetracks];

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

	XAnimNotifyInfo * tracks = (XAnimNotifyInfo*)buf->at();

	for(int i=0; i<numNotetracks; i++)
	{
		anim->notetracks[i].time = tracks[i].time;
	}

	buf->seek(numNotetracks * 8, SEEK_CUR); // skip the data to maintain compatibility

	// load data into the struct
	anim->name = strdup(name);

	if(anim->dataByte) 
	{
		anim->dataByte = new char[anim->dataByteCount];
		memcpy(anim->dataByte, buf->at(), anim->dataByteCount);
		buf->seek(anim->dataByteCount, SEEK_CUR);
	}

	if(anim->dataShort)
	{
		anim->dataShort = new short[anim->dataShortCount];
		memcpy(anim->dataShort, buf->at(), anim->dataShortCount * 2);
		buf->seek(anim->dataShortCount * 2, SEEK_CUR);
	}

	if(anim->dataInt) 
	{
		anim->dataInt = new int[anim->dataIntCount];
		memcpy(anim->dataInt, buf->at(), anim->dataIntCount * 4);
		buf->seek(anim->dataIntCount * 4, SEEK_CUR);
	}

	if(anim->randomDataShort) 
	{
		anim->randomDataShort = new short[anim->randomDataShortCount];
		memcpy(anim->randomDataShort, buf->at(), anim->randomDataShortCount * 2);
		buf->seek(anim->randomDataShortCount * 2, SEEK_CUR);
	}

	if(anim->randomDataByte) 
	{
		anim->randomDataByte = new char[anim->randomDataByteCount];
		memcpy(anim->randomDataByte, buf->at(), anim->randomDataByteCount);
		buf->seek(anim->randomDataByteCount, SEEK_CUR);
	}

	if(anim->randomDataInt) 
	{
		anim->randomDataInt = new int[anim->randomDataIntCount];
		memcpy(anim->randomDataInt, buf->at(), anim->randomDataIntCount * 4);
		buf->seek(anim->randomDataIntCount * 4, SEEK_CUR);
	}

	if(anim->indicies) 
	{
		if(anim->indexcount < 256)
		{
			anim->indicies = new XAnimIndices[anim->indexcount];
			memcpy(anim->indicies->data, buf->at(), anim->indexcount);
		} 
		else
		{
			anim->indicies = new XAnimIndices[anim->indexcount * 2];
			memcpy(anim->indicies->data, buf->at(), anim->indexcount * 2);
		}
	}

	return anim;
}