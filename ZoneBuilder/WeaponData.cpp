#include "StdInc.h"
#include "Tool.h"

void sndAliasToName(snd_alias_list_name* alias)
{
	if ((*alias).asset == NULL) return;
	(*alias).name = (*alias).asset->name;
}

void doRequires(WeaponVariantDef* def)
{

}

void writeWeaponDef(zoneInfo_t* info, ZStream* buf, WeaponDef* def)
{

}

void writeWeaponVariantDef(zoneInfo_t* info, ZStream* buf, WeaponVariantDef* data)
{
	WeaponVariantDef* def = (WeaponVariantDef*)buf->at();
	buf->write(data, sizeof(WeaponVariantDef), 1);

	
}

typedef WeaponVariantDef* (__cdecl * BG_LoadWeaponDef_LoadObj_t)(const char* filename);
BG_LoadWeaponDef_LoadObj_t BG_LoadWeaponDef_LoadObj = (BG_LoadWeaponDef_LoadObj_t)0x57B5F0;

void * addWeaponVariantDef(zoneInfo_t* info, const char* name, char* data, size_t dataLen)
{
	WeaponVariantDef* ret = (WeaponVariantDef*)data;
	// so we have to laod a file but we need a filename (which is given to us in data)
	// load it using the builtin function
	if (dataLen != 0)
	{
		ret = BG_LoadWeaponDef_LoadObj(data);
	}

	// now add strings to our list
	// hidetags
	for (int i = 0; i < 32; i++)
	{
		if (ret->hideTags[i] == NULL) break; // no more tags
		ret->hideTags[i] = addScriptString(info, SL_ConvertToString(ret->hideTags[i]));
	}
	// notetrackSounds
	for (int i = 0; i < 16; i++)
	{
		if (ret->WeaponDef->noteTrackSoundMap[0][i] == NULL) break; // no more tags
		ret->WeaponDef->noteTrackSoundMap[0][i] = addScriptString(info, SL_ConvertToString(ret->WeaponDef->noteTrackSoundMap[0][i]));
	}
	for (int i = 0; i < 16; i++)
	{
		if (ret->WeaponDef->noteTrackSoundMap[1][i] == NULL) break; // no more tags
		ret->WeaponDef->noteTrackSoundMap[1][i] = addScriptString(info, SL_ConvertToString(ret->WeaponDef->noteTrackSoundMap[1][i]));
	}

	// notetrackRumble
	for (int i = 0; i < 16; i++)
	{
		if (ret->WeaponDef->noteTrackRumbleMap[0][i] == NULL) break; // no more tags
		ret->WeaponDef->noteTrackRumbleMap[0][i] = addScriptString(info, SL_ConvertToString(ret->WeaponDef->noteTrackRumbleMap[0][i]));
	}
	for (int i = 0; i < 16; i++)
	{
		if (ret->WeaponDef->noteTrackSoundMap[1][i] == NULL) break; // no more tags
		ret->WeaponDef->noteTrackRumbleMap[1][i] = addScriptString(info, SL_ConvertToString(ret->WeaponDef->noteTrackRumbleMap[1][i]));
	}

	// now require all sub-assets
	// materials
#define save_material(mat) if (mat) addAsset(info, ASSET_TYPE_MATERIAL, mat->name, addMaterial(info, mat->name, (char*)mat, 0));
	save_material(ret->killIcon);
	save_material(ret->dpadIcon);
	save_material(ret->WeaponDef->reticleCenter);
	save_material(ret->WeaponDef->reticleSide);
	save_material(ret->WeaponDef->hudIcon);
	save_material(ret->WeaponDef->pickupIcon);
	save_material(ret->WeaponDef->ammoCounterIcon);
	save_material(ret->WeaponDef->AdsOverlayShader);
	save_material(ret->WeaponDef->AdsOverlayShaderLowRes);
	save_material(ret->WeaponDef->AdsOverlayShaderEMP);
	save_material(ret->WeaponDef->AdsOverlayShaderEMPLowRes);
#undef save_material

	// xmodel
#define save_model(model) if (model) addAsset(info, ASSET_TYPE_XMODEL, model->name, addXModel(info, model->name, (char*)model, 0));
	for (int i = 0; i < 16; i++)
	{
		save_model(ret->WeaponDef->gunXModel[i]);
	}
	save_model(ret->WeaponDef->handXModel);

	for (int i = 0; i < 16; i++)
	{
		save_model(ret->WeaponDef->worldModel[i]);
	}
	save_model(ret->WeaponDef->worldClipModel);
	save_model(ret->WeaponDef->rocketModel);
	save_model(ret->WeaponDef->knifeModel);
	save_model(ret->WeaponDef->worldKnifeModel);
	save_model(ret->WeaponDef->projectileModel);
#undef save_model

	// fx
	// null these for now because I'm not certain effect writing works
	ret->WeaponDef->viewFlashEffect = NULL;
	ret->WeaponDef->worldFlashEffect = NULL;
	ret->WeaponDef->viewShellEjectEffect = NULL;
	ret->WeaponDef->worldShellEjectEffect = NULL;
	ret->WeaponDef->viewLastShotEjectEffect = NULL;
	ret->WeaponDef->worldLastShotEjectEffect = NULL;
	ret->WeaponDef->projExplosionEffect = NULL;
	ret->WeaponDef->projDudEffect = NULL;
	ret->WeaponDef->projTrailEffect = NULL;
	ret->WeaponDef->projBeaconEffect = NULL;
	ret->WeaponDef->projIgnitionEffect = NULL;
	ret->WeaponDef->turretOverheatEffect = NULL;

	return ret;
}
