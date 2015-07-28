#include "StdInc.h"
#pragma once

enum VehicleType : int
{
	VEH_WHEELS_4 = 0x0,
	VEH_TANK = 0x1,
	VEH_PLANE = 0x2,
	VEH_BOAT = 0x3,
	VEH_ARTILLERY = 0x4,
	VEH_HELICOPTER = 0x5,
	VEH_SNOWMOBILE = 0x6,
	VEH_TYPE_COUNT = 0x7,
};

enum VehicleAxleType
{
	VEH_AXLE_FRONT = 0x0,
	VEH_AXLE_REAR = 0x1,
	VEH_AXLE_ALL = 0x2,
	VEH_AXLE_COUNT = 0x3,
};

struct VehiclePhysDef
{
	int physicsEnabled;
	const char *physPresetName;
	PhysPreset *physPreset;
	const char *accelGraphName;
	VehicleAxleType steeringAxle;
	VehicleAxleType powerAxle;
	VehicleAxleType brakingAxle;
	float topSpeed;
	float reverseSpeed;
	float maxVelocity;
	float maxPitch;
	float maxRoll;
	float suspensionTravelFront;
	float suspensionTravelRear;
	float suspensionStrengthFront;
	float suspensionDampingFront;
	float suspensionStrengthRear;
	float suspensionDampingRear;
	float frictionBraking;
	float frictionCoasting;
	float frictionTopSpeed;
	float frictionSide;
	float frictionSideRear;
	float velocityDependentSlip;
	float rollStability;
	float rollResistance;
	float pitchResistance;
	float yawResistance;
	float uprightStrengthPitch;
	float uprightStrengthRoll;
	float targetAirPitch;
	float airYawTorque;
	float airPitchTorque;
	float minimumMomentumForCollision;
	float collisionLaunchForceScale;
	float wreckedMassScale;
	float wreckedBodyFriction;
	float minimumJoltForNotify;
	float slipThreshholdFront;
	float slipThreshholdRear;
	float slipFricScaleFront;
	float slipFricScaleRear;
	float slipFricRateFront;
	float slipFricRateRear;
	float slipYawTorque;
};

struct VehicleDef
{
	const char *name;
	VehicleType type;
	const char *useHintString;
	int health;
	int quadBarrel;
	float texScrollScale;
	float maxSpeed;
	float accel;
	float rotRate;
	float rotAccel;
	float maxBodyPitch;
	float maxBodyRoll;
	float fakeBodyAccelPitch;
	float fakeBodyAccelRoll;
	float fakeBodyVelPitch;
	float fakeBodyVelRoll;
	float fakeBodySideVelPitch;
	float fakeBodySidePitchStrength;
	float fakeBodyRollStrength;
	float fakeBodyPitchDampening;
	float fakeBodyRollDampening;
	float fakeBodyBoatRockingAmplitude;
	float fakeBodyBoatRockingPeriod;
	float fakeBodyBoatRockingRotationPeriod;
	float fakeBodyBoatRockingFadeoutSpeed;
	float boatBouncingMinForce;
	float boatBouncingMaxForce;
	float boatBouncingRate;
	float boatBouncingFadeinSpeed;
	float boatBouncingFadeoutSteeringAngle;
	float collisionDamage;
	float collisionSpeed;
	float killcamZDist;
	float killcamBackDist;
	float killcamUpDist;
	int playerProtected;
	int bulletDamage;
	int armorPiercingDamage;
	int grenadeDamage;
	int projectileDamage;
	int projectileSplashDamage;
	int heavyExplosiveDamage;
	VehiclePhysDef vehiclePhysics;
	float boostDuration;
	float boostRechargeTime;
	float boostAcceleration;
	float supensionTravel;
	float maxSteeringAngle;
	float steeringLerp;
	float minSteeringScale;
	float minSteeringSpeed;
	int camLookEnabled;
	float camLerp;
	float camPitchInfluence;
	float camRollInfluence;
	float camFovIncrease;
	float camFovOffset;
	float camFovSpeed;
	const char *turretWeaponName;
	WeaponVariantDef *turretWeapon;
	float turretHorizSpanLeft;
	float turretHorizSpanRight;
	float turretVertSpanUp;
	float turretVertSpanDown;
	float turretRotRate;
	snd_alias_list_name turretSpinSnd;
	snd_alias_list_name turretStopSnd;
	int trophyEnabled;
	float trophyRadius;
	float trophyInactiveRadius;
	int trophyAmmoCount;
	float trophyReloadTime;
	__int16 trophyTags[4];
	Material *compassFriendlyIcon;
	Material *compassEnemyIcon;
	int compassIconWidth;
	int compassIconHeight;
	snd_alias_list_name lowIdleSound;
	snd_alias_list_name highIdleSound;
	snd_alias_list_name lowEngineSound;
	snd_alias_list_name highEngineSound;
	float engineSndSpeed;
	snd_alias_list_name engineStartUpSnd;
	int engineStartUpLength;
	snd_alias_list_name engineShutdownSnd;
	snd_alias_list_name engineIdleSnd;
	snd_alias_list_name engineSustainSnd;
	snd_alias_list_name engineRampUpSnd;
	int engineRampUpLength;
	snd_alias_list_name engineRampDownSnd;
	int engineRampDownLength;
	snd_alias_list_name suspensionSoftSnd;
	float suspensionSoftCompression;
	snd_alias_list_name suspensionHardSnd;
	float suspensionHardConpression;
	snd_alias_list_name collisionSnd;
	float collisionBlendSpeed;
	snd_alias_list_name speedSnd;
	float speedSndBlendSpeed;
	char *surfaceSndPrefix;
	snd_alias_list_name surfaceSounds[0x1F];
	float surfaceSndBlendSpeed;
	float slideVolume;
	float slideBlendSpeed;
	float inAirPitch;
};