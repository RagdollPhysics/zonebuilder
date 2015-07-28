#include "StdInc.h"
#include "Tool.h"

void writeVehicleDef(zoneInfo_t* info, ZStream* buf, VehicleDef* data)
{
	int physPreset = NULL, turretWeapon = NULL, compassFriendlyIcon = NULL, compassEnemyIcon = NULL;
	if (HAS_FIELD(data, vehiclePhysics.physPreset))
		physPreset = requireAsset(info, ASSET_TYPE_PHYSPRESET, data->vehiclePhysics.physPreset->name, buf);
	if (HAS_FIELD(data, turretWeapon))
		turretWeapon = requireAsset(info, ASSET_TYPE_WEAPON, data->turretWeapon->name, buf);
	if (HAS_FIELD(data, compassFriendlyIcon))
		compassFriendlyIcon = requireAsset(info, ASSET_TYPE_MATERIAL, data->compassFriendlyIcon->name, buf);
	if (HAS_FIELD(data, compassEnemyIcon))
		compassEnemyIcon = requireAsset(info, ASSET_TYPE_MATERIAL, data->compassEnemyIcon->name, buf);

	WRITE_ASSET(data, VehicleDef);

	WRITE_NAME(data);

	if (HAS_FIELD(data, useHintString))
	{
		WRITE_STRING(data, useHintString);
	}

	if (HAS_FIELD(data, vehiclePhysics.physPresetName))
	{
		WRITE_STRING(data, vehiclePhysics.physPresetName);
	}

	if (HAS_FIELD(data, vehiclePhysics.physPreset))
	{
		dest->vehiclePhysics.physPreset = (PhysPreset*)physPreset;
	}

	if (HAS_FIELD(data, vehiclePhysics.accelGraphName))
	{
		WRITE_STRING(data, vehiclePhysics.accelGraphName);
	}

	if (HAS_FIELD(data, turretWeaponName))
	{
		WRITE_STRING(data, turretWeaponName);
	}

	if (HAS_FIELD(data, turretWeapon))
	{
		dest->turretWeapon = (WeaponVariantDef*)turretWeapon;
	}

	if (HAS_FIELD(data, turretSpinSnd.name))
	{
		WRITE_STRING(data, turretSpinSnd.name);
	}

	if (HAS_FIELD(data, turretStopSnd.name))
	{
		WRITE_STRING(data, turretStopSnd.name);
	}

	if (HAS_FIELD(data, compassFriendlyIcon))
	{
		dest->compassFriendlyIcon = (Material*)compassFriendlyIcon;
	}

	if (HAS_FIELD(data, compassEnemyIcon))
	{
		dest->compassEnemyIcon = (Material*)compassEnemyIcon;
	}

	if (HAS_FIELD(data, lowIdleSound.name))
	{
		WRITE_STRING(data, lowIdleSound.name);
	}

	if (HAS_FIELD(data, highIdleSound.name))
	{
		WRITE_STRING(data, highIdleSound.name);
	}

	if (HAS_FIELD(data, lowEngineSound.name))
	{
		WRITE_STRING(data, lowEngineSound.name);
	}

	if (HAS_FIELD(data, highEngineSound.name))
	{
		WRITE_STRING(data, highEngineSound.name);
	}

	if (HAS_FIELD(data, engineStartUpSnd.name))
	{
		WRITE_STRING(data, engineStartUpSnd.name);
	}

	if (HAS_FIELD(data, engineShutdownSnd.name))
	{
		WRITE_STRING(data, engineShutdownSnd.name);
	}

	if (HAS_FIELD(data, engineIdleSnd.name))
	{
		WRITE_STRING(data, engineIdleSnd.name);
	}

	if (HAS_FIELD(data, engineSustainSnd.name))
	{
		WRITE_STRING(data, engineSustainSnd.name);
	}

	if (HAS_FIELD(data, engineRampUpSnd.name))
	{
		WRITE_STRING(data, engineRampUpSnd.name);
	}

	if (HAS_FIELD(data, engineRampDownSnd.name))
	{
		WRITE_STRING(data, engineRampDownSnd.name);
	}

	if (HAS_FIELD(data, suspensionSoftSnd.name))
	{
		WRITE_STRING(data, suspensionSoftSnd.name);
	}

	if (HAS_FIELD(data, suspensionHardSnd.name))
	{
		WRITE_STRING(data, suspensionHardSnd.name);
	}

	if (HAS_FIELD(data, collisionSnd.name))
	{
		WRITE_STRING(data, collisionSnd.name);
	}

	if (HAS_FIELD(data, speedSnd.name))
	{
		WRITE_STRING(data, speedSnd.name);
	}

	if (HAS_FIELD(data, surfaceSndPrefix))
	{
		WRITE_STRING(data, surfaceSndPrefix);
	}

	for (int i = 0; i < 31; i++)
	{
		if (!data->surfaceSounds[i].name) continue;

		buf->write(data->surfaceSounds[i].name, strlen(data->surfaceSounds[i].name) + 1, 1);
		dest->surfaceSounds[i].name = (char*)-1;
	}

}

void * addVehicleDef(zoneInfo_t* info, const char* name, char* data, int dataLen)
{
	if (dataLen > 0)  { Com_Error(false, "No custom Vehicles yet.");  return NULL; }
	return data;
}
