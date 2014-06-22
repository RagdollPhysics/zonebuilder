#include "StdInc.h"
#include "Tool.h"
#include <functional>

class AssetProperty
{
public:
	AssetProperty() { };
	AssetProperty(int off, int sz, int offNum=-1) : size(sz), offset(off), offsetOfNum(offNum) { 
			doCustomWrite = [](void* a, BUFFER* b) { return false; };
			getCountCustom = [](void* a) { return -1; };
		};
	void addChild(AssetProperty* prop) { children.push_back(prop); };
	void setCustomWrite(function<bool(void*, BUFFER*)> func) { doCustomWrite = func; };
	int size;
	int offset;
	int offsetOfNum;
	list<AssetProperty*> children;
	function<bool(void*, BUFFER*)> doCustomWrite;
	function<int(void*)> getCountCustom;
};

class AssetWriter
{
public:
	AssetWriter() { };
	AssetWriter(int s) : _size(s) { };
	void addProp(AssetProperty* p) { _prop.push_back(p); };
	void write(void* asset, BUFFER* buf);

private:
	int _size;
	list<AssetProperty*> _prop;
};

AssetWriter* createAssetWriters();