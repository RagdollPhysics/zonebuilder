#include "StdInc.h"
int ffVersion = 277;

char header[] = {'I', 'W', 'f', 'f', 'u', '1', '0', '0', 
	ffVersion, ffVersion >> 8, ffVersion >> 16, ffVersion >> 24, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

int inputFiles = 0;
string *input;
string output;

void doArgs(int argc, char* argv[])
{
	if(argc < 2) Com_Error(true, "Not enough Parameters!");
	output = argv[1];
	input = new string[argc - 2];
	for(int i=2; i<argc; i++)
	{
		input[i-2] = argv[i];
		inputFiles++;
	}
}

void loadAsset(zoneInfo_t* info, int type, const char* filename, const char* name)
{
	if(containsAsset(info, type, name) > 0) return;
	if(GetFileAttributesA(filename) == INVALID_FILE_ATTRIBUTES) { Com_Error(false, "File %s does not exist!", filename); return; }
	Com_Debug("Loading Asset %s of type %s (%s)\n", name, getAssetStringForType(type), filename);
	FILE* fp = fopen(filename, "rb");
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	char* data = (char*)VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);//malloc(size);
	fseek(fp, 0, SEEK_SET);

	fread(data, 1, size, fp);
	fflush(fp);
	fclose(fp);
	switch(type)
	{
		case ASSET_TYPE_XANIM:
			addXAnim(info, name, data, size);
			break;
		case ASSET_TYPE_RAWFILE:
			addRawfile(info, name, data, size);
			break;
		case ASSET_TYPE_MATERIAL:
			addMaterial(info, name, data, size);
			break;
		case ASSET_TYPE_TECHSET:
			addTechset(info, name, data, size);
			break;
		case ASSET_TYPE_VERTEXDECL:
			addVertexDecl(info, name, data, size);
			break;
		case ASSET_TYPE_VERTEXSHADER:
			addVertexShader(info, name, data, size);
			break;
		case ASSET_TYPE_PIXELSHADER:
			addPixelShader(info, name, data, size);
			break;
		case ASSET_TYPE_XMODELSURFS:
			addXModelSurfs(info, name, data, size);
			break;
	}
}

void loadAssetsFromFile(zoneInfo_t* info, const char* file)
{
	if(GetFileAttributesA(file) == INVALID_FILE_ATTRIBUTES) { Com_Error(false, "\nFile %s does not exist!", file); return; }
	FILE* fp = fopen(file, "r");
	char line [1024];
	int lineNum = 1;
	while (fscanf(fp, "%s\n", &line) != EOF)
	{
		string s = string(line);
		int first = s.find(',');
		int second = s.substr(first + 1, string::npos).find(',');
		int type = getAssetTypeForString(s.substr(0, first).c_str());
		string realname = s.substr(first + 1, second);
		string filename = s.substr(first + second + 2, string::npos);
		loadAsset(info, type, filename.c_str(), realname.c_str());
		lineNum++;
	}

	fclose(fp);
}

int main(int argc, char* argv[])
{
	Com_Printf("Startup...\n");
	zoneInfo_t * info = getZoneInfo();
	doArgs(argc, argv);

	Com_Printf("Building Zone : %s\n", output.c_str());

	Com_Printf("Loading asets from source files: ");
	for(int i=0 ;i<inputFiles; i++)
	{
		Com_Printf("%s...\n", input[i].c_str());
		loadAssetsFromFile(info, input[i].c_str());
	}
	Com_Printf("Done\n");
	
	doLastAsset(info, output.c_str());

	Com_Printf("Writing Zone File.....\n");
	BUFFER* buf = writeZone(info);
#if _DEBUG
    FILE* fp = fopen("uncompressed_zone", "wb");
    buf->writetofile(fp);
    fclose(fp);
#endif
	Com_Printf("Compressing Zone File.....\n");
	BUFFER* compressed = buf->compressZlib();
	Com_Printf("Writing to Disk.....\n");
	CreateDirectoryA("zone", NULL);
	FILE* out = fopen(output.insert(0, "zone\\").append(".ff").c_str(), "wb");
	_setmode( _fileno( out ), _O_BINARY ); // it was fucking up zlib output
	FILETIME time;
	GetSystemTimeAsFileTime(&time);
	*((int*)&header[13]) = time.dwHighDateTime;
	*((int*)&header[17]) = time.dwLowDateTime;
	fwrite(header, 21, 1, out);
	compressed->writetofile(out);
	fclose(out);
	Com_Printf("Done!");

	if(IsDebuggerPresent()) {
		getchar();
	}
	return 0;
}
