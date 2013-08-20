#include "StdInc.h"

unsigned int pad = 0xFFFFFFFF;
unsigned int zero = 0;

BUFFER* writeZone(zoneInfo_t * info)
{
	BUFFER* buf = new BUFFER(0x8000);
	buf->seek(40, SEEK_SET);
	
	buf->write(&info->scriptStringCount, 4, 1);
	if(info->scriptStringCount > 0) buf->write(&pad, 4, 1);
	else buf->write(&zero, 4, 1);
	buf->write(&info->assetCount, 4, 1);
	buf->write(&pad, 4, 1);
	
	for(int i=0; i<info->scriptStringCount; i++)
	{
		buf->write(&pad, 4, 1);
	}

	for(int i=0; i<info->scriptStringCount; i++)
	{
		buf->write((void*)info->scriptStrings[i].c_str(), info->scriptStrings[i].length() + 1, 1);
	}

	for(int i=0; i<info->assetCount; i++)
	{
		buf->write(&info->assets[i].type, 4, 1);
		buf->write(&pad, 4, 1);
	}

	for(int i=0; i<info->assetCount; i++)
	{
		info->assets[i].offset = buf->tell(); // store offset
		buf->write(info->assets[i].data, info->assets[i].length, 1);
		int end = buf->tell();

		map<int, int> fixups = *(info->assets[i].fixups); // do fixups
		for(map<int,int>::iterator it = fixups.begin(); it != fixups.end(); it++)
		{
			buf->seek(info->assets[i].offset + it->first, SEEK_SET);
			buf->write(info->assets[it->second].offset | 0xF0000000, 4);
		}
		buf->seek(end, SEEK_SET);
	}

	buf->resize(-1); // should be maxsize

	// YAY... now we get to compute XZoneMemory sizes!
	// got some nice values courtesy of IW4Tool
	int sizes[10];
	sizes[0] = buf->getsize() - 39; // data length
	sizes[1] = 0;
	sizes[2] = (int)(buf->getsize() * 0.4);
	sizes[3] = 0;
	sizes[4] = 0;
	sizes[5] = (int)(buf->getsize() * 1.3);
	sizes[6] = 0;
	sizes[7] = 0;
	sizes[8] = 0; // 1.2 * something.....
	sizes[9] = 0; // 1.2 * something.....

	buf->seek(0, SEEK_SET);
	buf->write(sizes, 40, 1);

	return buf;
}

typedef struct
{

} XHeader_t;