#include "StdInc.h"
#pragma once

struct vec2_t
{
	float x, y;
};

enum weapFireType_t : int
{
	WEAPON_FIRETYPE_FULLAUTO = 0x0,
	WEAPON_FIRETYPE_SINGLESHOT = 0x1,
	WEAPON_FIRETYPE_BURSTFIRE2 = 0x2,
	WEAPON_FIRETYPE_BURSTFIRE3 = 0x3,
	WEAPON_FIRETYPE_BURSTFIRE4 = 0x4,
	WEAPON_FIRETYPE_DOUBLE_BARREL = 0x5,
	WEAPON_FIRETYPE_MAX
};

enum weapInventoryType_t : int
{
	WEAPINVENTORY_PRIMARY = 0,
	WEAPINVENTORY_OFFHAND = 1,
	WEAPINVENTORY_ITEM = 2,
	WEAPINVENTORY_ALTMODE = 3,
	WEAPINVENTORY_EXCLUSIVE = 4,
	WEAPINVENTORY_SCAVENGER = 5,
	WEAPINVENTORY_MAX
};

enum PenetrateType
{
	PENETRATE_TYPE_NONE = 0x0,
	PENETRATE_TYPE_SMALL = 0x1,
	PENETRATE_TYPE_MEDIUM = 0x2,
	PENETRATE_TYPE_LARGE = 0x3,
	PENETRATE_TYPE_COUNT = 0x4
};

enum activeReticleType_t : int
{
	VEH_ACTIVE_RETICLE_NONE = 0,
	VEH_ACTIVE_RETICLE_PIP_ON_A_STICK = 1,
	VEH_ACTIVE_RETICLE_BOUNCING_DIAMOND = 2,
	VEH_ACTIVE_RETICLE_MAX
};

enum weapType_t : int
{
	WEAPTYPE_BULLET = 0,
	WEAPTYPE_GRENADE = 1,
	WEAPTYPE_PROJECTILE = 2,
	WEAPTYPE_RIOTSHIELD = 3,
	WEAPTYPE_MAX
};

enum weapClass_t : int
{
	WEAPCLASS_RIFLE = 0,
	WEAPCLASS_SNIPER = 1,
	WEAPCLASS_MG = 2,
	WEAPCLASS_SMG = 3,
	WEAPCLASS_SPREAD = 4,
	WEAPCLASS_PISTOL = 5,
	WEAPCLASS_GRENADE = 6,
	WEAPCLASS_ROCKETLAUNCHER = 7,
	WEAPCLASS_TURRET = 8,
	WEAPCLASS_THROWINGKNIFE = 9,
	WEAPCLASS_NON_PLAYER = 10,
	WEAPCLASS_ITEM = 11,
	WEAPCLASS_MAX
};

enum OffhandClass : int
{
	OFFHAND_CLASS_NONE = 0,
	OFFHAND_CLASS_FRAG_GRENADE = 1,
	OFFHAND_CLASS_SMOKE_GRENADE = 2,
	OFFHAND_CLASS_FLASH_GRENADE = 3,
	OFFHAND_CLASS_MAX
};

enum playerAnimType_t : int
{
	PLAER_ANIM_TYPE_NONE = 0x0,
	PLAER_ANIM_TYPE_OTHER = 0x1,
	PLAER_ANIM_TYPE_PISTOL = 0x2,
	PLAER_ANIM_TYPE_SMG = 0x3,
	PLAER_ANIM_TYPE_AUTORIFLE = 0x4,
	PLAER_ANIM_TYPE_MG = 0x5,
	PLAER_ANIM_TYPE_SNIPER = 0x6,
	PLAER_ANIM_TYPE_ROCKETLAUNCHER = 0x7,
	PLAER_ANIM_TYPE_EXPLOSIVE = 0x8,
	PLAER_ANIM_TYPE_GRENADE = 0x9,
	PLAER_ANIM_TYPE_TURRET = 0xA,
	PLAER_ANIM_TYPE_C4 = 0xB,
	PLAER_ANIM_TYPE_M203 = 0xC,
	PLAER_ANIM_TYPE_HOLD = 0xD,
	PLAER_ANIM_TYPE_BRIEFCASE = 0xE,
	PLAER_ANIM_TYPE_RIOTSHIELD = 0xF,
	PLAER_ANIM_TYPE_LAPTOP = 0x10,
	PLAER_ANIM_TYPE_THROWINGKNIFE = 0x11
};

enum weapProjExplosion_t
{
	WEAPPROJEXP_GRENADE = 0x0,
	WEAPPROJEXP_ROCKET = 0x1,
	WEAPPROJEXP_FLASHBANG = 0x2,
	WEAPPROJEXP_NONE = 0x3,
	WEAPPROJEXP_DUD = 0x4,
	WEAPPROJEXP_SMOKE = 0x5,
	WEAPPROJEXP_HEAVY = 0x6,
	WEAPPROJEXP_NUM = 0x7
};

enum WeapStickinessType
{
	WEAPSTICKINESS_NONE = 0x0,
	WEAPSTICKINESS_ALL = 0x1,
	WEAPSTICKINESS_ALL_ORIENT = 0x2,
	WEAPSTICKINESS_GROUND = 0x3,
	WEAPSTICKINESS_GROUND_WITH_YAW = 0x4,
	WEAPSTICKINESS_KNIFE = 0x5,
	WEAPSTICKINESS_COUNT = 0x6
};

enum weaponIconRatioType_t
{
	WEAPON_ICON_RATIO_1TO1 = 0x0,
	WEAPON_ICON_RATIO_2TO1 = 0x1,
	WEAPON_ICON_RATIO_4TO1 = 0x2,
	WEAPON_ICON_RATIO_COUNT = 0x3
};

enum ammoCounterClipType_t
{
	AMMO_COUNTER_CLIP_NONE = 0x0,
	AMMO_COUNTER_CLIP_MAGAZINE = 0x1,
	AMMO_COUNTER_CLIP_SHORTMAGAZINE = 0x2,
	AMMO_COUNTER_CLIP_SHOTGUN = 0x3,
	AMMO_COUNTER_CLIP_ROCKET = 0x4,
	AMMO_COUNTER_CLIP_BELTFED = 0x5,
	AMMO_COUNTER_CLIP_ALTWEAPON = 0x6,
	AMMO_COUNTER_CLIP_COUNT = 0x7
};

enum weapOverlayReticle_t
{
	WEAPOVERLAYRETICLE_NONE = 0x0,
	WEAPOVERLAYRETICLE_CROSSHAIR = 0x1,
	WEAPOVERLAYRETICLE_NUM = 0x2
};

enum weapOverlayInterface_t
{
	WEAPOVERLAYINTERFACE_NONE = 0x0,
	WEAPOVERLAYINTERFACE_JAVELIN = 0x1,
	WEAPOVERLAYINTERFACE_TURRETSCOPE = 0x2,
	WEAPOVERLAYINTERFACECOUNT = 0x3
};

enum weapStance_t
{
	WEAPSTANCE_STAND = 0x0,
	WEAPSTANCE_DUCK = 0x1,
	WEAPSTANCE_PRONE = 0x2,
	WEAPSTANCE_NUM = 0x3
};

enum ImpactType
{
	IMPACT_TYPE_NONE = 0,
	IMPACT_TYPE_BULLET_SMALL = 1,
	IMPACT_TYPE_BULLET_LARGE = 2,
	IMPACT_TYPE_BULLET_AP = 3,
	IMPACT_TYPE_SHOTGUN_FMJ = 4,
	IMPACT_TYPE_SHOTGUN = 5,
	IMPACT_TYPE_GRENADE_BOUNCE = 7,
	IMPACT_TYPE_GRENADE_EXPLODE = 8,
	IMPACT_TYPE_ROCKET_EXPLODE = 9,
	IMPACT_TYPE_PROJECTILE_DUD = 10,
	IMPACT_TYPE_MAX
};

enum guidedMissileType_t
{
	MISSILE_GUIDANCE_NONE = 0x0,
	MISSILE_GUIDANCE_SIDEWINDER = 0x1,
	MISSILE_GUIDANCE_HELLFIRE = 0x2,
	MISSILE_GUIDANCE_JAVELIN = 0x3,
	MISSILE_GUIDANCE_MAX
};

enum surfaceNames_t
{
	bark,
	brick,
	carpet,
	cloth,
	concrete,
	dirt,
	flesh,
	foliage,
	glass,
	grass,
	gravel,
	ice,
	metal,
	mud,
	paper,
	plaster,
	rock,
	sand,
	snow,
	water,
	wood,
	asphalt,
	ceramic,
	plastic,
	rubber,
	cushion,
	fruit,
	paintedmetal,
	riotshield,
	slush,
	opaqueglass
};

union snd_alias_list_name
{
	const char * name;
	SoundAliasList * asset;
};

struct WeaponDef
{
	char * szInternalName;
	XModel * *gunXModel;           //Count = 16
	XModel * handXModel;
	const char * * szXAnimsR;        //Count = 37
	const char * * szXAnimsL;        //Count = 37
	char * szModeName;
	short *noteTrackSoundMap[2]; // Count = 16
	short *noteTrackRumbleMap[2]; // Count = 16
	playerAnimType_t playerAnimType;
	weapType_t weaponType;
	weapClass_t weaponClass;
	PenetrateType penetrateType;
	weapInventoryType_t inventoryType;
	weapFireType_t fireType;
	OffhandClass offhandClass;
	weapStance_t stance;
	FxEffectDef * viewFlashEffect;
	FxEffectDef * worldFlashEffect;
	snd_alias_list_name sounds[48];
	FxEffectDef * viewShellEjectEffect;
	FxEffectDef * worldShellEjectEffect;
	FxEffectDef * viewLastShotEjectEffect;
	FxEffectDef * worldLastShotEjectEffect;
	Material * reticleCenter;
	Material * reticleSide;
	int iReticleCenterSize;
	int iReticleSideSize;
	int iReticleMinOfs;
	activeReticleType_t activeReticleType;
	float vStandMove[3];
	float vStandRot[3];
	float vStrafeMove[3];
	float vStrafeRot[3];
	float vDuckedOfs[3];
	float vDuckedMove[3];
	float vDuckedRot[3];
	float vProneOfs[3];
	float vProneMove[3];
	float vProneRot[3];
	float fPosMoveRate;
	float fPosProneMoveRate;
	float fStandMoveMinSpeed;
	float fDuckedMoveMinSpeed;
	float fProneMoveMinSpeed;
	float fPosRotRate;
	float fPosProneRotRate;
	float fStandRotMinSpeed;
	float fDuckedRotMinSpeed;
	float fProneRotMinSpeed;
	XModel * * worldModel;
	XModel * worldClipModel;
	XModel * rocketModel;
	XModel * knifeModel;
	XModel * worldKnifeModel;
	Material * hudIcon;
	weaponIconRatioType_t hudIconRatio;
	Material * pickupIcon;
	weaponIconRatioType_t pickupIconRatio;
	Material * ammoCounterIcon;
	weaponIconRatioType_t ammoCounterIconRatio;
	ammoCounterClipType_t ammoCounterClip;
	int iStartAmmo;
	const char *szAmmoName;
	int iAmmoIndex;
	const char *szClipName;
	int iClipIndex;
	int iMaxAmmo;
	int shotCount;
	const char *szSharedAmmoCapName;
	int iSharedAmmoCapIndex;
	int iSharedAmmoCap;
	int damage;
	int playerDamage;
	int iMeleeDamage;
	int iDamageType;
	int iFireDelay;
	int iMeleeDelay;
	int meleeChargeDelay;
	int iDetonateDelay;
	int iRechamberTime;
	int iRechamberOneHanded;
	int iRechamberBoltTime;
	int iHoldFireTime;
	int iDetonateTime;
	int iMeleeTime;
	int meleeChargeTime;
	int iReloadTime;
	int reloadShowRocketTime;
	int iReloadEmptyTime;
	int iReloadAddTime;
	int iReloadStartTime;
	int iReloadStartAddTime;
	int iReloadEndTime;
	int iDropTime;
	int iRaiseTime;
	int iAltDropTime;
	int quickDropTime;
	int quickRaiseTime;
	int iFirstRaiseTime;
	int iEmptyRaiseTime;
	int iEmptyDropTime;
	int sprintInTime;
	int sprintLoopTime;
	int sprintOutTime;
	int stunnedTimeBegin;
	int stunnedTimeLoop;
	int stunnedTimeEnd;
	int nightVisionWearTime;
	int nightVisionWearTimeFadeOutEnd;
	int nightVisionWearTimePowerUp;
	int nightVisionRemoveTime;
	int nightVisionRemoveTimePowerDown;
	int nightVisionRemoveTimeFadeInStart;
	int fuseTime;
	int aifuseTime;
	float autoAimRange;
	float aimAssistRange;
	float aimAssistRangeAds;
	float aimPadding;
	float enemyCrosshairRange;
	float moveSpeedScale;
	float adsMoveSpeedScale;
	float sprintDurationScale;
	float adsZoomInFrac;
	float adsZoomOutFrac;
	Material * AdsOverlayShader;
	Material * AdsOverlayShaderLowRes;
	Material * AdsOverlayShaderEMP;
	Material * AdsOverlayShaderEMPLowRes;
	weapOverlayReticle_t adsOverlayReticle;
	weapOverlayInterface_t adsOverlayInterface;
	float adsOverlayWidth;
	float adsOverlayHeight;
	float adsOverlayWidthSplitscreen;
	float adsOverlayHeightSplitscreen;
	float fAdsBobFactor;
	float fAdsViewBobMult;
	float fHipSpreadStandMin;
	float fHipSpreadDuckedMin;
	float fHipSpreadProneMin;
	float hipSpreadStandMax;
	float hipSpreadDuckedMax;
	float hipSpreadProneMax;
	float fHipSpreadDecayRate;
	float fHipSpreadFireAdd;
	float fHipSpreadTurnAdd;
	float fHipSpreadMoveAdd;
	float fHipSpreadDuckedDecay;
	float fHipSpreadProneDecay;
	float fHipReticleSidePos;
	float fAdsIdleAmount;
	float fHipIdleAmount;
	float adsIdleSpeed;
	float hipIdleSpeed;
	float fIdleCrouchFactor;
	float fIdleProneFactor;
	float fGunMaxPitch;
	float fGunMaxYaw;
	float swayMaxAngle;
	float swayLerpSpeed;
	float swayPitchScale;
	float swayYawScale;
	float swayHorizScale;
	float swayVertScale;
	float swayShellShockScale;
	float adsSwayMaxAngle;
	float adsSwayLerpSpeed;
	float adsSwayPitchScale;
	float adsSwayYawScale;
	float adsSwayHorizScale;
	float adsSwayVertScale;
	float adsViewErrorMin;
	float adsViewErrorMax;
	PhysCollmap * collisions;
	float dualWieldViewModelOffset;
	weaponIconRatioType_t killIconRatio;
	int iReloadAmmoAdd;
	int iReloadStartAdd;
	int iDropAmmoMin;
	int ammoDropClipPercentMin;
	int ammoDropClipPercentMax;
	int iExplosionRadius;
	int iExplosionRadiusMin;
	int iExplosionInnerDamage;
	int iExplosionOuterDamage;
	float damageConeAngle;
	float bulletExplDmgMult;
	float bulletExplRadiusMult;
	int iProjectileSpeed;
	int iProjectileSpeedUp;
	int iProjectileSpeedForward;
	int iProjectileActivateDist;
	float projLifetime;
	float timeToAccelerate;
	float projectileCurvature;
	XModel * projectileModel;
	weapProjExplosion_t projExplosiveType;
	FxEffectDef * projExplosionEffect;
	FxEffectDef * projDudEffect;
	snd_alias_list_name projExplosionSound;
	snd_alias_list_name projDudSound;
	WeapStickinessType stickiness;
	float lowAmmoWarningThreshold;
	float ricochetChance;
	float * parallelBounce;            //Refer to surfaceNames_t
	float * perpendicularBounce;         //Refer to surfaceNames_t
	FxEffectDef * projTrailEffect;
	FxEffectDef * projBeaconEffect;
	float vProjectileColor[3];
	guidedMissileType_t guidedMissileType;
	float maxSteeringAccel;
	float projIgnitionDelay;
	FxEffectDef * projIgnitionEffect;
	snd_alias_list_name projIgnitionSound;
	float fAdsAimPitch;
	float fAdsCrosshairInFrac;
	float fAdsCrosshairOutFrac;
	int adsGunKickReducedKickBullets;
	float adsGunKickReducedKickPercent;
	float fAdsGunKickPitchMin;
	float fAdsGunKickPitchMax;
	float fAdsGunKickYawMin;
	float fAdsGunKickYawMax;
	float fAdsGunKickAccel;
	float fAdsGunKickSpeedMax;
	float fAdsGunKickSpeedDecay;
	float fAdsGunKickStaticDecay;
	float fAdsViewKickPitchMin;
	float fAdsViewKickPitchMax;
	float fAdsViewKickYawMin;
	float fAdsViewKickYawMax;
	float fAdsViewScatterMin;
	float fAdsViewScatterMax;
	float fAdsSpread;
	int hipGunKickReducedKickBullets;
	float hipGunKickReducedKickPercent;
	float fHipGunKickPitchMin;
	float fHipGunKickPitchMax;
	float fHipGunKickYawMin;
	float fHipGunKickYawMax;
	float fHipGunKickAccel;
	float fHipGunKickSpeedMax;
	float fHipGunKickSpeedDecay;
	float fHipGunKickStaticDecay;
	float fHipViewKickPitchMin;
	float fHipViewKickPitchMax;
	float fHipViewKickYawMin;
	float fHipViewKickYawMax;
	float fHipViewScatterMin;
	float fHipViewScatterMax;
	float fightDist;
	float maxDist;
	const char *accuracyGraphName[2];
	vec2_t * accuracyGraphKnots;
	vec2_t * originalAccuracyGraphKnots;
	short accuracyGraphKnotCount;
	short originalAccuracyGraphKnotCount;
	int iPositionReloadTransTime;
	float leftArc;
	float rightArc;
	float topArc;
	float bottomArc;
	float accuracy;
	float aiSpread;
	float playerSpread;
	float minVertTurnSpeed;
	float minHorTurnSpeed;
	float maxVertTurnSpeed;
	float maxHorTurnSpeed;
	float pitchConvergenceTime;
	float yawConvergenceTime;
	float suppressTime;
	float maxRange;
	float fAnimHorRotateInc;
	float fPlayerPositionDist;
	const char *szUseHintString;
	const char *dropHintString;
	int iUseHintStringIndex;
	int dropHintStringIndex;
	float horizViewJitter;
	float vertViewJitter;
	float scanSpeed;
	float scanAccel;
	int scanPauseTime;
	const char *szScript;
	float fOOPosAnimLength[2];
	int minDamage;
	int minPlayerDamage;
	float maxDamageRange;
	float minDamageRange;
	float destabilizationRateTime;
	float destabilizationCurvatureMax;
	int destabilizeDistance;
	float * locationDamageMultipliers;   //1Refer to bodyParts_t
	const char *fireRumble;
	const char *meleeImpactRumble;
	Tracer * tracer;
	int turretScopeZoomRate;
	int turretScopeZoomMin;
	int turretScopeZoomMax;
	int turretOverheatUpRate;
	int turretOverheatDownRate;
	int turretOverheatPenalty;
	snd_alias_list_name turretOverheatSound;
	FxEffectDef *turretOverheatEffect;
	char *turretBarrelSpinRumble;
	int turretBarrelSpinUpTime;
	int turretBarrelSpinDownTime;
	int turretBarrelSpinSpeed;
	snd_alias_list_name turretBarrelSpinMaxSnd;
	snd_alias_list_name turretBarrelSpinUpSnds[4];
	snd_alias_list_name turretBarrelSpinDownSnds[4];
	snd_alias_list_name missileConeSoundAlias;
	snd_alias_list_name missileConeSoundAliasAtBase;
	float missileConeSoundRadiusAtTop;
	float missileConeSoundRadiusAtBase;
	float missileConeSoundHeight;
	float missileConeSoundOriginOffset;
	float missileConeSoundVolumescaleAtCore;
	float missileConeSoundVolumescaleAtEdge;
	float missileConeSoundVolumescaleCoreSize;
	float missileConeSoundPitchAtTop;
	float missileConeSoundPitchAtBottom;
	float missileConeSoundPitchTopSize;
	float missileConeSoundPitchBottomSize;
	float missileConeSoundCrossfadeTopSize;
	float missileConeSoundCrossfadeBottomSize;
	bool shareAmmo;
	bool lockonSupported;
	bool requireLockonToFire;
	bool bigExplosion;
	bool noAdsWhenMagEmpty;
	bool avoidDropCleanup;
	bool inheritsPerks;
	bool crosshairColorChange;
	bool rifleBullet;
	bool armorPiercing;
	bool boltAction;
	bool aimDownSight;
	bool rechamberWhileAds;
	bool bBulletExplosiveDamage;
	bool cookOffHold;
	bool clipOnly;
	bool noAmmoPickup;
	bool adsFire;
	bool cancelAutoHolsterWhenEmpty;
	bool disableSwitchToWhenEmpty;
	bool suppressAmmoReserveDisplay;
	bool laserSightDuringNightvision;
	bool markableViewmodel;
	bool noDualWield;
	bool flipKillIcon;
	bool noPartialReload;
	bool segmentedReload;
	bool blocksProne;
	bool silenced;
	bool isRollingGrenade;
	bool projExplosionEffectForceNormalUp;
	bool projImpactExplode;
	bool stickToPlayers;
	bool hasDetonator;
	bool disableFiring;
	bool timedDetonation;
	bool rotate;
	bool holdButtonToThrow;
	bool freezeMovementWhenFiring;
	bool thermalScope;
	bool altModeSameWeapon;
	bool turretBarrelSpinEnabled;
	bool missileConeSoundEnabled;
	bool missileConeSoundPitchshiftEnabled;
	bool missileConeSoundCrossfadeEnabled;
	bool offhandHoldIsCancelable;
};

struct WeaponVariantDef
{
	char * name;
	WeaponDef * WeaponDef;
	char * displayName;
	short *hideTags; // Count = 32
	const char * * szXAnims;        //Count = 37
	float fAdsZoomFov;
	int iAdsTransInTime;
	int iAdsTransOutTime;
	int iClipSize;
	ImpactType impactType;
	int iFireTime;
	weaponIconRatioType_t dpadIconRatio;
	float fPenetrateMultiplier;
	float fAdsViewKickCenterSpeed;
	float fHipViewKickCenterSpeed;
	const char * altWeaponName;
	unsigned int altWeaponIndex;
	int iAltRaiseTime;
	Material * killIcon;
	Material * dpadIcon;
	int unknown8;
	int iFirstRaiseTime;
	int iDropAmmoMax;
	float adsDofStart;
	float adsDofEnd;
	short accuracyGraphKnotCount;
	short originalAccuracyGraphKnotCount;
	vec2_t *accuracyGraphKnots;
	vec2_t *originalAccuracyGraphKnots;
	bool motionTracker;
	bool enhanced;
	bool dpadIconShowsAmmo;
};