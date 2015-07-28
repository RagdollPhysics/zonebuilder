#include "StdInc.h"
#include "Tool.h"

void writeMenuList(zoneInfo_t* info, ZStream* buf, MenuList* data)
{

}

void* addMenu(zoneInfo_t* info, const char* name, char* data, int dataLen)
{
	if (dataLen > 0) { Com_Error(false, "No custom menus yet."); return NULL; }
	return data;
}

void * addMenuList(zoneInfo_t* info, const char* name, char* data, int dataLen)
{
	if (dataLen > 0) { Com_Error(false, "No custom menulists yet."); return NULL; }

	MenuList* list = (MenuList*)data;
	for (int i = 0; list->menuCount; i++)
	{
		addAsset(info, ASSET_TYPE_MENU, list->menus[i]->window.name, addMenu(info, list->menus[i]->window.name, (char*)list->menus[i], -1));
	}

	return data;
}