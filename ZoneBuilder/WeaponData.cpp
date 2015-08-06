#include "StdInc.h"
#include "Tool.h"

void doRequires(zoneInfo_t* info, ZStream* buf, WeaponVariantDef* def)
{
#define save_material(mat) if(mat) mat = (Material*)requireAsset(info, ASSET_TYPE_MATERIAL, (char*)mat->name, buf);
	save_material(def->killIcon);
	save_material(def->dpadIcon);
	save_material(def->WeaponDef->reticleCenter);
	save_material(def->WeaponDef->reticleSide);
	save_material(def->WeaponDef->hudIcon);
	save_material(def->WeaponDef->pickupIcon);
	save_material(def->WeaponDef->ammoCounterIcon);
	save_material(def->WeaponDef->AdsOverlayShader);
	save_material(def->WeaponDef->AdsOverlayShaderLowRes);
	save_material(def->WeaponDef->AdsOverlayShaderEMP);
	save_material(def->WeaponDef->AdsOverlayShaderEMPLowRes);
#undef save_material

	// xmodel
#define save_model(mat) if(mat) mat = (XModel*)requireAsset(info, ASSET_TYPE_XMODEL, (char*)mat->name, buf);

	for (int i = 0; i < 16; i++)
	{
		save_model(def->WeaponDef->gunXModel[i]);
	}

	save_model(def->WeaponDef->handXModel);

	for (int i = 0; i < 16; i++)
	{
		save_model(def->WeaponDef->worldModel[i]);
	}

	save_model(def->WeaponDef->worldClipModel);
	save_model(def->WeaponDef->rocketModel);
	save_model(def->WeaponDef->knifeModel);
	save_model(def->WeaponDef->worldKnifeModel);
	save_model(def->WeaponDef->projectileModel);
#undef save_model

	if (def->WeaponDef->collisions) def->WeaponDef->collisions = (PhysGeomList*)requireAsset(info, ASSET_TYPE_PHYS_COLLMAP, (char*)def->WeaponDef->collisions->name, buf);
	if (def->WeaponDef->tracer) def->WeaponDef->tracer = (Tracer*)requireAsset(info, ASSET_TYPE_TRACER, (char*)def->WeaponDef->tracer->name, buf);
/*
#define save_fx(fx) if(fx) fx = (FxEffectDef*)requireAsset(info, ASSET_TYPE_FX, (char*)fx->name, buf);
	save_fx(def->WeaponDef->viewFlashEffect);
	save_fx(def->WeaponDef->worldFlashEffect);
	save_fx(def->WeaponDef->viewShellEjectEffect);
	save_fx(def->WeaponDef->worldShellEjectEffect);
	save_fx(def->WeaponDef->viewLastShotEjectEffect);
	save_fx(def->WeaponDef->worldLastShotEjectEffect);
	save_fx(def->WeaponDef->projExplosionEffect);
	save_fx(def->WeaponDef->projDudEffect);
	save_fx(def->WeaponDef->projTrailEffect);
	save_fx(def->WeaponDef->projBeaconEffect);
	save_fx(def->WeaponDef->projIgnitionEffect);
	save_fx(def->WeaponDef->turretOverheatEffect);
#undef save_fx
*/
}

void writeWeaponDef(zoneInfo_t* info, ZStream* buf, WeaponDef* data)
{
	WeaponDef* def = (WeaponDef*)buf->at();
	buf->write(data, sizeof(WeaponDef), 1);

	buf->write(data->szInternalName, strlen(data->szInternalName) + 1, 1);
	def->szInternalName = (char*)-1;

	if (data->gunXModel) // OffsetToPointer
	{
		buf->align(ALIGN_TO_4);
		// data in the array is already stored primed
		buf->write(data->gunXModel, sizeof(XModel*), 16);
		def->gunXModel = (XModel**)-1;
	}

	// already stored and primed by the require function
	// handXModel

	if (data->szXAnimsR) // OffsetToPointer
	{
		buf->align(ALIGN_TO_4);
		int* ptrs = (int*)buf->at();
		buf->write(3, -1, 37);

		for (int i = 0; i < 37; i++)
		{
			if (!data->szXAnimsR[i]) 
			{
				ptrs[i] = 0; continue;
			} // there is no text

			buf->write(data->szXAnimsR[i], strlen(data->szXAnimsR[i]) + 1, 1);
		}

		def->szXAnimsR = (const char**)-1;
	}

	if (data->szXAnimsL) // OffsetToPointer
	{
		buf->align(ALIGN_TO_4);
		int* ptrs = (int*)buf->at();
		buf->write(3, -1, 37);

		for (int i = 0; i < 37; i++)
		{
			if (!data->szXAnimsL[i]) 
			{
				ptrs[i] = 0; continue;
			} // there is no text

			buf->write(data->szXAnimsL[i], strlen(data->szXAnimsL[i]) + 1, 1);
		}

		def->szXAnimsL = (const char**)-1;
	}

	if (data->szModeName) 
	{
		buf->write(data->szModeName, strlen(data->szModeName) + 1, 1);
		def->szModeName = (char*)-1;
	}

	if (data->noteTrackSoundMap[0]) // OffsetToPointer
	{
		buf->align(ALIGN_TO_2);
		buf->write(data->noteTrackSoundMap[0], 2, 16);
		def->noteTrackSoundMap[0] = (short*)-1;
	}

	if (data->noteTrackSoundMap[1]) // OffsetToPointer
	{
		buf->align(ALIGN_TO_2);
		buf->write(data->noteTrackSoundMap[0], 2, 16);
		def->noteTrackSoundMap[1] = (short*)-1;
	}

	if (data->noteTrackRumbleMap[0]) // OffsetToPointer
	{
		buf->align(ALIGN_TO_2);
		buf->write(data->noteTrackRumbleMap[0], 2, 16);
		def->noteTrackRumbleMap[0] = (short*)-1;
	}

	if (data->noteTrackRumbleMap[1]) // OffsetToPointer
	{
		buf->align(ALIGN_TO_2);
		buf->write(data->noteTrackRumbleMap[0], 2, 16);
		def->noteTrackRumbleMap[1] = (short*)-1;
	}

	// already stored and primed by the require function
	// viewFlashEffect
	// worldFlashEffect

	for (int i = 0; i < 47; i++)
	{
		if (!data->sounds[i].name) continue;

		buf->write(3, -1, 1);
		buf->write(data->sounds[i].name, strlen(data->sounds[i].name) + 1, 1);
		def->sounds[i].name = (char*)-1;
	}

	if (data->bounceSound) // OffsetToPointer
	{
		buf->align(ALIGN_TO_4);
		int* ptrs = (int*)buf->at();
		buf->write(3, -1, 37);

		for (int i = 0; i < 37; i++)
		{
			if (!data->bounceSound[i].name) 
			{
				ptrs[i] = 0; continue;
			} // there is no text

			buf->write(3, -1, 1);
			buf->write(data->bounceSound[i].name, strlen(data->bounceSound[i].name) + 1, 1);
		}

		def->bounceSound = (snd_alias_list_name*)-1;
	}

	// already stored and primed by the require function
	// viewShellEjectEffect
	// worldShellEjectEffect
	// viewShellLastShotEjectEffect
	// worldShellLastShotEjectEffect
	// reticleCenter
	// reticleSide

	if (data->worldModel) // OffsetToPointer
	{
		buf->align(ALIGN_TO_4);
		// data in the array is already stored primed
		buf->write(data->worldModel, sizeof(XModel*), 16);
		def->worldModel = (XModel**)-1;
	}

	// already stored and primed by the require function
	// worldClipModel
	// rocketModel
	// knifeModel
	// worldKnifeModel
	// hudIcon
	// pickupIcon
	// ammoCounterIcon

	if (data->szAmmoName)
	{
		buf->write(data->szAmmoName, strlen(data->szAmmoName) + 1, 1);
		def->szAmmoName = (char*)-1;
	}

	if (data->szClipName)
	{
		buf->write(data->szClipName, strlen(data->szClipName) + 1, 1);
		def->szClipName = (char*)-1;
	}

	if (data->szSharedAmmoCapName)
	{
		buf->write(data->szSharedAmmoCapName, strlen(data->szSharedAmmoCapName) + 1, 1);
		def->szSharedAmmoCapName = (char*)-1;
	}

	// already stored and primed by the require function
	// AdsOverlayShader
	// AdsOverlayShaderLowRes
	// AdsOverlayShaderEMP
	// AdsOverlayShaderEMPLowRes
	// collisions
	// projectileModel
	// projExplosionEffect
	// projDudEffect

	if (data->projExplosionSound.name)
	{
		buf->write(3, -1, 1);
		buf->write(data->projExplosionSound.name, strlen(data->projExplosionSound.name) + 1, 1);
		def->projExplosionSound.name = (char*)-1;
	}

	if (data->projDudSound.name)
	{
		buf->write(3, -1, 1);
		buf->write(data->projDudSound.name, strlen(data->projDudSound.name) + 1, 1);
		def->projDudSound.name = (char*)-1;
	}

	if (data->parallelBounce) // OffsetToPointer
	{
		buf->align(ALIGN_TO_4);
		buf->write(data->parallelBounce, sizeof(float), 31);
		def->parallelBounce = (float*)-1;
	}

	if (data->perpendicularBounce) // OffsetToPointer
	{
		buf->align(ALIGN_TO_4);
		buf->write(data->perpendicularBounce, sizeof(float), 31);
		def->perpendicularBounce = (float*)-1;
	}

	// already stored and primed by the require function
	// projTrailEffect
	// projBeaconEffect
	// projIgnitionEffect

	if (data->projIgnitionSound.name)
	{
		buf->write(3, -1, 1);
		buf->write(data->projIgnitionSound.name, strlen(data->projIgnitionSound.name) + 1, 1);
		def->projIgnitionSound.name = (char*)-1;
	}

	if (data->accuracyGraphName[0])
	{
		buf->write(data->accuracyGraphName[1], strlen(data->accuracyGraphName[0]) + 1, 1);
		def->accuracyGraphName[0] = (char*)-1;
	}

	if (data->accuracyGraphKnots) // OffsetToPointer
	{
		buf->align(ALIGN_TO_2);
		buf->write(data->accuracyGraphKnots, sizeof(vec2_t), data->accuracyGraphKnotCount);
		def->accuracyGraphKnots = (vec2_t*)-1;
	}

	if (data->accuracyGraphName[1])
	{
		buf->write(data->accuracyGraphName[1], strlen(data->accuracyGraphName[1]) + 1, 1);
		def->accuracyGraphName[1] = (char*)-1;
	}

	if (data->originalAccuracyGraphKnots) // OffsetToPointer
	{
		buf->align(ALIGN_TO_2);
		buf->write(data->originalAccuracyGraphKnots, sizeof(vec2_t), data->originalAccuracyGraphKnotCount);
		def->originalAccuracyGraphKnots = (vec2_t*)-1;
	}

	if (data->szUseHintString)
	{
		buf->write(data->szUseHintString, strlen(data->szUseHintString) + 1, 1);
		def->szUseHintString = (char*)-1;
	}

	if (data->dropHintString)
	{
		buf->write(data->dropHintString, strlen(data->dropHintString) + 1, 1);
		def->dropHintString = (char*)-1;
	}

	if (data->szScript)
	{
		buf->write(data->szScript, strlen(data->szScript) + 1, 1);
		def->szScript = (char*)-1;
	}

	if (data->locationDamageMultipliers) // OffsetToPointer
	{
		buf->align(ALIGN_TO_4);
		buf->write(data->locationDamageMultipliers, sizeof(float), 20);
		def->locationDamageMultipliers = (float*)-1;
	}

	if (data->fireRumble)
	{
		buf->write(data->fireRumble, strlen(data->fireRumble) + 1, 1);
		def->fireRumble = (char*)-1;
	}

	if (data->meleeImpactRumble)
	{
		buf->write(data->meleeImpactRumble, strlen(data->meleeImpactRumble) + 1, 1);
		def->meleeImpactRumble = (char*)-1;
	}

	// already stored and primed by the require function
	// tracer

	if (data->turretOverheatSound.name)
	{
		buf->write(3, -1, 1);
		buf->write(data->turretOverheatSound.name, strlen(data->turretOverheatSound.name) + 1, 1);
		def->turretOverheatSound.name = (char*)-1;
	}

	// already stored and primed by the require function
	// turretOverheatEffect

	if (data->turretBarrelSpinRumble)
	{
		buf->write(data->turretBarrelSpinRumble, strlen(data->turretBarrelSpinRumble) + 1, 1);
		def->turretBarrelSpinRumble = (char*)-1;
	}

	if (data->turretBarrelSpinMaxSnd.name)
	{
		buf->write(3, -1, 1);
		buf->write(data->turretBarrelSpinMaxSnd.name, strlen(data->turretBarrelSpinMaxSnd.name) + 1, 1);
		def->turretBarrelSpinMaxSnd.name = (char*)-1;
	}

	for (int i = 0; i < 4; i++)
	{
		if (!data->turretBarrelSpinUpSnds[i].name) continue;

		buf->write(3, -1, 1);
		buf->write(data->turretBarrelSpinUpSnds[i].name, strlen(data->turretBarrelSpinUpSnds[i].name) + 1, 1);
		def->turretBarrelSpinUpSnds[i].name = (char*)-1;
	}

	for (int i = 0; i < 4; i++)
	{
		if (!data->turretBarrelSpinDownSnds[i].name) continue;

		buf->write(3, -1, 1);
		buf->write(data->turretBarrelSpinDownSnds[i].name, strlen(data->turretBarrelSpinDownSnds[i].name) + 1, 1);
		def->turretBarrelSpinDownSnds[i].name = (char*)-1;
	}

	if (data->missileConeSoundAlias.name)
	{
		buf->write(3, -1, 1);
		buf->write(data->missileConeSoundAlias.name, strlen(data->missileConeSoundAlias.name) + 1, 1);
		def->missileConeSoundAlias.name = (char*)-1;
	}

	if (data->missileConeSoundAliasAtBase.name)
	{
		buf->write(3, -1, 1);
		buf->write(data->missileConeSoundAliasAtBase.name, strlen(data->missileConeSoundAliasAtBase.name) + 1, 1);
		def->missileConeSoundAliasAtBase.name = (char*)-1;
	}

}

void writeWeaponVariantDef(zoneInfo_t* info, ZStream* buf, WeaponVariantDef* data)
{
	doRequires(info, buf, data);

	WeaponVariantDef* def = (WeaponVariantDef*)buf->at();
	buf->write(data, sizeof(WeaponVariantDef), 1);

	buf->write(data->name, strlen(data->name) + 1, 1);
	def->name = (char*)-1;

	if (data->WeaponDef) // OffsetToPointer
	{
		buf->align(ALIGN_TO_4);
		writeWeaponDef(info, buf, data->WeaponDef);
		def->WeaponDef = (WeaponDef*)-1;
	}

	buf->write(data->displayName, strlen(data->displayName) + 1, 1);
	def->displayName = (char*)-1;

	if (data->hideTags) // OffsetToPointer
	{
		buf->align(ALIGN_TO_2);
		buf->write(data->hideTags, 2, 32);
		def->hideTags = (short*)-1;
	}

	if (data->szXAnims) // OffsetToPointer
	{
		buf->align(ALIGN_TO_4);
		int* ptrs = (int*)buf->at();
		buf->write(3, -1, 37);

		for (int i = 0; i < 37; i++)
		{
			if (!data->szXAnims[i]) 
			{
				ptrs[i] = 0; continue;
			} // there is no text

			buf->write(data->szXAnims[i], strlen(data->szXAnims[i]) + 1, 1);
		}

		def->szXAnims = (const char**)-1;
	}

	buf->write(data->altWeaponName, strlen(data->altWeaponName) + 1, 1);
	def->altWeaponName = (char*)-1;

	// already stored and primed by the require function
	// killIcon
	// dpadIcon

	if (data->accuracyGraphKnots) // OffsetToPointer
	{
		buf->align(ALIGN_TO_4);
		buf->write(data->accuracyGraphKnots, 8, data->accuracyGraphKnotCount);
		def->accuracyGraphKnots = (vec2_t*)-1;
	}

	if (data->originalAccuracyGraphKnots) // OffsetToPointer
	{
		buf->align(ALIGN_TO_4);
		buf->write(data->originalAccuracyGraphKnots, 8, data->originalAccuracyGraphKnotCount);
		def->originalAccuracyGraphKnots = (vec2_t*)-1;
	}
}

typedef WeaponVariantDef* (__cdecl * BG_LoadWeaponDef_LoadObj_t)(const char* filename);
BG_LoadWeaponDef_LoadObj_t BG_LoadWeaponDef_LoadObj = (BG_LoadWeaponDef_LoadObj_t)0x57B5F0;

void * addWeaponVariantDef(zoneInfo_t* info, const char* name, char* data, int dataLen)
{
	WeaponVariantDef* ret;
	if (dataLen > 0) ret = BG_LoadWeaponDef_LoadObj(data);
	else ret = (WeaponVariantDef*)data;

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
#define save_material(mat) if (mat) addAsset(info, ASSET_TYPE_MATERIAL, mat->name, addMaterial(info, mat->name, (char*)mat, -1));
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
#define save_model(model) if (model) addAsset(info, ASSET_TYPE_XMODEL, model->name, addXModel(info, model->name, (char*)model, -1));
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

	if (ret->WeaponDef->collisions)
	{
		addAsset(info, ASSET_TYPE_PHYS_COLLMAP, ret->WeaponDef->collisions->name,
		addPhysCollmap(info, ret->WeaponDef->collisions->name, (char*)ret->WeaponDef->collisions, -1));
	}

	if (ret->WeaponDef->tracer)
	{
		addAsset(info, ASSET_TYPE_TRACER, ret->WeaponDef->tracer->name,
		addTracer(info, ret->WeaponDef->tracer->name, (char*)ret->WeaponDef->tracer, -1));
	}

	// fx
	// null these for now because I'm not certain effect writing works

#define save_fx(model) if (model) addAsset(info, ASSET_TYPE_FX, model->name, addFxEffectDef(info, model->name, (char*)model, -1));
	/*
	save_fx(ret->WeaponDef->viewFlashEffect);
	save_fx(ret->WeaponDef->worldFlashEffect);
	save_fx(ret->WeaponDef->viewShellEjectEffect);
	save_fx(ret->WeaponDef->worldShellEjectEffect);
	save_fx(ret->WeaponDef->viewLastShotEjectEffect);
	save_fx(ret->WeaponDef->worldLastShotEjectEffect);
	save_fx(ret->WeaponDef->projExplosionEffect);
	save_fx(ret->WeaponDef->projDudEffect);
	save_fx(ret->WeaponDef->projTrailEffect);
	save_fx(ret->WeaponDef->projBeaconEffect);
	save_fx(ret->WeaponDef->projIgnitionEffect);
	save_fx(ret->WeaponDef->turretOverheatEffect);
	*/
#undef save_fx
	
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
