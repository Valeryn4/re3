#pragma once

#include "audio_enums.h"
#include "AudioCollision.h"
#include "PolRadio.h"

class tSound
{
public:
	int32 m_nEntityIndex;
	int32 m_nCounter;
	int32 m_nSampleIndex;
	uint8 m_nBankIndex;
	bool8 m_bIs2D;
	int32 m_nReleasingVolumeModificator;
	uint32 m_nFrequency;
	uint8 m_nVolume;
	float m_fDistance;
	int32 m_nLoopCount;
#ifndef GTA_PS2
	int32 m_nLoopStart;
	int32 m_nLoopEnd;
#endif
#ifdef EXTERNAL_3D_SOUND
	uint8 m_nEmittingVolume;
#endif
	float m_fSpeedMultiplier;
	float m_fSoundIntensity;
	bool8 m_bReleasingSoundFlag;
	CVector m_vecPos;
	bool8 m_bReverbFlag;
	uint8 m_nLoopsRemaining;
	bool8 m_bRequireReflection; // Used for oneshots
	uint8 m_nOffset;
	int32 m_nReleasingVolumeDivider;
	bool8 m_bIsProcessed;
	bool8 m_bLoopEnded;
	int32 m_nCalculatedVolume;
	int8 m_nVolumeChange;
};

VALIDATE_SIZE(tSound, 92);

class CPhysical;
class CAutomobile;

class tAudioEntity
{
public:
	eAudioType m_nType;
	void *m_pEntity;
	bool8 m_bIsUsed;
	bool8 m_bStatus;
	int16 m_awAudioEvent[NUM_AUDIOENTITY_EVENTS];
	float m_afVolume[NUM_AUDIOENTITY_EVENTS];
	uint8 m_AudioEvents;
};

VALIDATE_SIZE(tAudioEntity, 40);

class tPedComment
{
public:
	int32 m_nSampleIndex;
	int32 m_nEntityIndex;
	CVector m_vecPos;
	float m_fDistance;
	uint8 m_bVolume;
	int8 m_nProcess;
};

VALIDATE_SIZE(tPedComment, 28);

class cPedComments
{
public:
	tPedComment m_asPedComments[NUM_PED_COMMENTS_BANKS][NUM_PED_COMMENTS_SLOTS];
	uint8 m_nIndexMap[NUM_PED_COMMENTS_BANKS][NUM_PED_COMMENTS_SLOTS];
	uint8 m_nCommentsInBank[NUM_PED_COMMENTS_BANKS];
	uint8 m_nActiveBank;

	cPedComments()
	{
		for (int i = 0; i < NUM_PED_COMMENTS_SLOTS; i++)
			for (int j = 0; j < NUM_PED_COMMENTS_BANKS; j++) {
				m_asPedComments[j][i].m_nProcess = -1;
				m_nIndexMap[j][i] = NUM_PED_COMMENTS_SLOTS;
			}

		for (int i = 0; i < NUM_PED_COMMENTS_BANKS; i++)
			m_nCommentsInBank[i] = 0;
		m_nActiveBank = 0;
	}
	void Add(tPedComment *com);
	void Process();
};

VALIDATE_SIZE(cPedComments, 1164);

class CEntity;

class cMissionAudio
{
public:
	CVector m_vecPos;
	bool8 m_bPredefinedProperties;
	int32 m_nSampleIndex;
	uint8 m_nLoadingStatus;
	uint8 m_nPlayStatus;
	bool8 m_bIsPlaying;
	int32 m_nMissionAudioCounter;
	bool8 m_bIsPlayed;
};
VALIDATE_SIZE(cMissionAudio, 32);

// name made up
class cAudioScriptObjectManager
{
public:
	int32 m_anScriptObjectEntityIndices[NUM_SCRIPT_MAX_ENTITIES];
	int32 m_nScriptObjectEntityTotal;

	cAudioScriptObjectManager() { m_nScriptObjectEntityTotal = 0; }
	~cAudioScriptObjectManager() { m_nScriptObjectEntityTotal = 0; }
};


class cTransmission;
class CPlane;
class CVehicle;
class CPed;

class cPedParams
{
public:
	bool8 m_bDistanceCalculated;
	float m_fDistance;
	CPed *m_pPed;

	cPedParams()
	{
		m_bDistanceCalculated = false;
		m_fDistance = 0.0f;
		m_pPed = nil;
	}
};

class cVehicleParams
{
public:
	bool8 m_bDistanceCalculated;
	float m_fDistance;
	CVehicle *m_pVehicle;
	cTransmission *m_pTransmission;
	int32 m_nIndex;
	float m_fVelocityChange;

	cVehicleParams()
	{
		m_bDistanceCalculated = false;
		m_fDistance = 0.0f;
		m_pVehicle = nil;
		m_pTransmission = nil;
		m_nIndex = 0;
		m_fVelocityChange = 0.0f;
	}
};

VALIDATE_SIZE(cVehicleParams, 0x18);

enum {
	/*
	REFLECTION_YMAX = 0, top
	REFLECTION_YMIN = 1, bottom
	REFLECTION_XMIN = 2, left
	REFLECTION_XMAX = 3, right
	REFLECTION_ZMAX = 4,
	*/

	REFLECTION_TOP = 0,
	REFLECTION_BOTTOM,
	REFLECTION_LEFT,
	REFLECTION_RIGHT,
	REFLECTION_UP,
	MAX_REFLECTIONS,
};

enum PLAY_STATUS { PLAY_STATUS_STOPPED = 0, PLAY_STATUS_PLAYING, PLAY_STATUS_FINISHED };
enum LOADING_STATUS { LOADING_STATUS_NOT_LOADED = 0, LOADING_STATUS_LOADED, LOADING_STATUS_FAILED };

class cAudioManager
{
public:
	bool8 m_bIsInitialised;
	bool8 m_bReverb; // unused
	bool8 m_bFifthFrameFlag;
	uint8 m_nActiveSamples;
	uint8 field_4; // unused
	bool8 m_bDynamicAcousticModelingStatus;
	float m_fSpeedOfSound;
	bool8 m_bTimerJustReset;
	int32 m_nTimer;
	tSound m_sQueueSample;
	uint8 m_nActiveSampleQueue;
	tSound m_asSamples[NUM_SOUNDS_SAMPLES_BANKS][NUM_CHANNELS_GENERIC];
	uint8 m_abSampleQueueIndexTable[NUM_SOUNDS_SAMPLES_BANKS][NUM_CHANNELS_GENERIC];
	uint8 m_SampleRequestQueuesStatus[NUM_SOUNDS_SAMPLES_BANKS];
	tSound m_asActiveSamples[NUM_CHANNELS_GENERIC];
	tAudioEntity m_asAudioEntities[NUM_AUDIOENTITIES];
	int32 m_anAudioEntityIndices[NUM_AUDIOENTITIES];
	int32 m_nAudioEntitiesTotal;
#ifdef GTA_PC
	CVector m_avecReflectionsPos[NUM_AUDIO_REFLECTIONS];
	float m_afReflectionsDistances[NUM_AUDIO_REFLECTIONS];
#endif
	cAudioScriptObjectManager m_sAudioScriptObjectManager;
	cPedComments m_sPedComments;
	int32 m_nFireAudioEntity;
	int32 m_nWaterCannonEntity;
	int32 m_nPoliceChannelEntity;
	cPoliceRadioQueue m_sPoliceRadioQueue;
	int32 m_nFrontEndEntity;
	int32 m_nCollisionEntity;
	cAudioCollisionManager m_sCollisionManager;
	int32 m_nProjectileEntity;
	int32 m_nBridgeEntity;
	cMissionAudio m_sMissionAudio;
	int32 m_anRandomTable[5];
	uint8 m_nTimeSpent;
	uint8 m_nUserPause;
	uint8 m_nPreviousUserPause;
	uint32 m_FrameCounter;

	cAudioManager();
	~cAudioManager();

	void Initialise();
	void Terminate();
	void Service();
	int32 CreateEntity(eAudioType type, void *entity);
	void DestroyEntity(int32 id);
	bool8 GetEntityStatus(int32 id);
	void SetEntityStatus(int32 id, bool8 status);
	void *GetEntityPointer(int32 id);
	void PlayOneShot(int32 index, uint16 sound, float vol);
	void SetEffectsMasterVolume(uint8 volume);
	void SetMusicMasterVolume(uint8 volume);
	void SetEffectsFadeVol(uint8 volume);
	void SetMusicFadeVol(uint8 volume);
	void SetMonoMode(bool8 mono);
	void ResetTimers(uint32 time);
	void DestroyAllGameCreatedEntities();

#ifdef GTA_PC
	uint8 GetNum3DProvidersAvailable();
	char *Get3DProviderName(uint8 id);
	int8 GetCurrent3DProviderIndex();
	int8 SetCurrent3DProvider(uint8 which);
	void SetSpeakerConfig(int32 conf);
	bool8 IsMP3RadioChannelAvailable();
	void ReleaseDigitalHandle();
	void ReacquireDigitalHandle();
	void SetDynamicAcousticModelingStatus(bool8 status);
	bool8 CheckForAnAudioFileOnCD();
	char GetCDAudioDriveLetter();
	bool8 IsAudioInitialised();
#endif

	void ServiceSoundEffects();
	uint32 FL(float f); // not used
	uint8 ComputeVolume(uint8 emittingVolume, float soundIntensity, float distance);
	void TranslateEntity(Const CVector *v1, CVector *v2);
	int32 ComputePan(float, CVector *);
	uint32 ComputeDopplerEffectedFrequency(uint32 oldFreq, float position1, float position2, float speedMultiplier); // inlined on PS2
	int32 RandomDisplacement(uint32 seed);
	void InterrogateAudioEntities(); // inlined on PS2
	void AddSampleToRequestedQueue();
	void AddDetailsToRequestedOrderList(uint8 sample); // inlined on PS2
#ifdef GTA_PC
	void AddReflectionsToRequestedQueue();
	void UpdateReflections();
#endif
	void AddReleasingSounds();
	void ProcessActiveQueues();
	void ClearRequestedQueue(); // inlined on PS2
	void ClearActiveSamples();
	void GenerateIntegerRandomNumberTable(); // inlined on PS2
	void LoadBankIfNecessary(uint8 bank); // this is used only on PS2 but technically not a platform code

#ifdef EXTERNAL_3D_SOUND // actually must have been && AUDIO_MSS as well
	void AdjustSamplesVolume();
	uint8 ComputeEmittingVolume(uint8 emittingVolume, float intensity, float dist);
#endif

	// audio logic
	void PreInitialiseGameSpecificSetup();
	void PostInitialiseGameSpecificSetup();
	void PreTerminateGameSpecificShutdown();
	void PostTerminateGameSpecificShutdown();
	void ResetAudioLogicTimers(uint32 timer);
	void ProcessReverb();
	float GetDistanceSquared(const CVector &v);
	void CalculateDistance(bool8 &condition, float dist);
	void ProcessSpecial();
	void ProcessEntity(int32 sound);
	void ProcessPhysical(int32 id);

	// vehicles
	void ProcessVehicle(CVehicle *vehicle);
	void ProcessRainOnVehicle(cVehicleParams &params);
	bool8 ProcessReverseGear(cVehicleParams &params);
	void ProcessModelCarEngine(cVehicleParams &params);
	bool8 ProcessVehicleRoadNoise(cVehicleParams &params);
	bool8 ProcessWetRoadNoise(cVehicleParams &params);
	void ProcessVehicleEngine(cVehicleParams &params);
	void UpdateGasPedalAudio(CAutomobile *automobile); // inlined on PS2
	void PlayerJustGotInCar();
	void PlayerJustLeftCar();
	void AddPlayerCarSample(uint8 emittingVolume, uint32 freq, uint32 sample, uint8 bank, uint8 counter, bool8 notLooping);
	void ProcessCesna(cVehicleParams &params);
	void ProcessPlayersVehicleEngine(cVehicleParams &params, CAutomobile *automobile);
	bool8 ProcessVehicleSkidding(cVehicleParams &params);
	float GetVehicleDriveWheelSkidValue(uint8 wheel, CAutomobile *automobile, cTransmission *transmission, float velocityChange);
	float GetVehicleNonDriveWheelSkidValue(uint8 wheel, CAutomobile *automobile, cTransmission *transmission, float velocityChange); // inlined on PS2
	void ProcessVehicleHorn(cVehicleParams &params);
	bool8 UsesSiren(uint32 model); // inlined on PS2
	bool8 UsesSirenSwitching(uint32 model); // inlined on PS2
	bool8 ProcessVehicleSirenOrAlarm(cVehicleParams &params);
	bool8 UsesReverseWarning(uint32 model); // inlined on PS2
	bool8 ProcessVehicleReverseWarning(cVehicleParams &params);
	bool8 ProcessVehicleDoors(cVehicleParams &params);
	bool8 ProcessAirBrakes(cVehicleParams &params);
	bool8 HasAirBrakes(uint32 model); // inlined on PS2
	bool8 ProcessEngineDamage(cVehicleParams &params);
	bool8 ProcessCarBombTick(cVehicleParams &params);
	void ProcessVehicleOneShots(cVehicleParams &params);
	bool8 ProcessTrainNoise(cVehicleParams &params);
	bool8 ProcessBoatEngine(cVehicleParams &params);
	bool8 ProcessBoatMovingOverWater(cVehicleParams &params);
	bool8 ProcessHelicopter(cVehicleParams &params);
	void ProcessPlane(cVehicleParams &params); // inlined on PS2
	void ProcessJumbo(cVehicleParams &params);
	void ProcessJumboTaxi(); // inlined on PS2
	void ProcessJumboAccel(CPlane *plane);
	void ProcessJumboTakeOff(CPlane *plane); // inlined on PS2
	void ProcessJumboFlying();               // inlined on PS2
	void ProcessJumboLanding(CPlane *plane); // inlined on PS2
	void ProcessJumboDecel(CPlane *plane);   // inlined on PS2
	bool8 SetupJumboTaxiSound(uint8 vol);
	bool8 SetupJumboWhineSound(uint8 emittingVol, uint32 freq);
	bool8 SetupJumboEngineSound(uint8 vol, uint32 freq);
	bool8 SetupJumboFlySound(uint8 emittingVol);
	bool8 SetupJumboRumbleSound(uint8 emittingVol);
	int32 GetJumboTaxiFreq(); // inlined on PS2

	// peds
	void ProcessPed(CPhysical *ped); // inlined on PS2
	void ProcessPedHeadphones(cPedParams &params);
	void ProcessPedOneShots(cPedParams &params);

	// ped comments
	void SetupPedComments(cPedParams &params, uint16 sound);
	int32 GetPedCommentSfx(CPed *ped, uint16 sound);
	void GetPhrase(uint32 &phrase, uint32 &prevPhrase, uint32 sample, uint32 maxOffset); // inlined on PS2
	uint32 GetPlayerTalkSfx(uint16 sound);                                               // inlined on PS2
	uint32 GetCopTalkSfx(uint16 sound);
	uint32 GetSwatTalkSfx(uint16 sound);
	uint32 GetFBITalkSfx(uint16 sound);
	uint32 GetArmyTalkSfx(uint16 sound);
	uint32 GetMedicTalkSfx(uint16 sound);
	uint32 GetFiremanTalkSfx(uint16 sound); // inlined on PS2
	uint32 GetBusinessMaleOldTalkSfx(uint16 sound);
	uint32 GetBusinessMaleYoungTalkSfx(uint16 sound, uint32 model);
	uint32 GetMafiaTalkSfx(uint16 sound);
	uint32 GetTriadTalkSfx(uint16 sound);
	uint32 GetDiabloTalkSfx(uint16 sound);
	uint32 GetYakuzaTalkSfx(uint16 sound);
	uint32 GetYardieTalkSfx(uint16 sound);
	uint32 GetColumbianTalkSfx(uint16 sound);
	uint32 GetHoodTalkSfx(uint16 sound);
	uint32 GetBlackCriminalTalkSfx(uint16 sound);
	uint32 GetWhiteCriminalTalkSfx(uint16 sound);
	uint32 GetCasualMaleOldTalkSfx(uint16 sound);
	uint32 GetCasualMaleYoungTalkSfx(uint16 sound);
	uint32 GetBlackCasualFemaleTalkSfx(uint16 sound);
	uint32 GetWhiteCasualFemaleTalkSfx(uint16 sound);
	uint32 GetFemaleNo3TalkSfx(uint16 sound);
	uint32 GetWhiteBusinessFemaleTalkSfx(uint16 sound, uint32 model);
	uint32 GetBlackFatFemaleTalkSfx(uint16 sound);
	uint32 GetWhiteFatMaleTalkSfx(uint16 sound);
	uint32 GetBlackFatMaleTalkSfx(uint16 sound);
	uint32 GetWhiteFatFemaleTalkSfx(uint16 sound);
	uint32 GetBlackFemaleProstituteTalkSfx(uint16 sound);
	uint32 GetWhiteFemaleProstituteTalkSfx(uint16 sound);
	uint32 GetBlackProjectMaleTalkSfx(uint16 sound, uint32 model);
	uint32 GetBlackProjectFemaleOldTalkSfx(uint16 sound);
	uint32 GetBlackProjectFemaleYoungTalkSfx(uint16 sound);
	uint32 GetChinatownMaleOldTalkSfx(uint16 sound);
	uint32 GetChinatownMaleYoungTalkSfx(uint16 sound);
	uint32 GetChinatownFemaleOldTalkSfx(uint16 sound);
	uint32 GetChinatownFemaleYoungTalkSfx(uint16 sound);
	uint32 GetLittleItalyMaleTalkSfx(uint16 sound);
	uint32 GetLittleItalyFemaleOldTalkSfx(uint16 sound);
	uint32 GetLittleItalyFemaleYoungTalkSfx(uint16 sound);
	uint32 GetWhiteDockerMaleTalkSfx(uint16 sound);
	uint32 GetBlackDockerMaleTalkSfx(uint16 sound);
	uint32 GetScumMaleTalkSfx(uint16 sound);
	uint32 GetScumFemaleTalkSfx(uint16 sound);
	uint32 GetWhiteWorkerMaleTalkSfx(uint16 sound);
	uint32 GetBlackWorkerMaleTalkSfx(uint16 sound);
	uint32 GetBlackBusinessFemaleTalkSfx(uint16 sound);
	uint32 GetSupermodelMaleTalkSfx(uint16 sound);
	uint32 GetSupermodelFemaleTalkSfx(uint16 sound);
	uint32 GetStewardMaleTalkSfx(uint16 sound);
	uint32 GetStewardFemaleTalkSfx(uint16 sound);
	uint32 GetFanMaleTalkSfx(uint16 sound, uint32 model);
	uint32 GetFanFemaleTalkSfx(uint16 sound);
	uint32 GetHospitalMaleTalkSfx(uint16 sound);
	uint32 GetHospitalFemaleTalkSfx(uint16 sound); // inlined on PS2
	uint32 GetWhiteConstructionWorkerTalkSfx(uint16 sound);
	uint32 GetBlackConstructionWorkerTalkSfx(uint16 sound);
	uint32 GetShopperFemaleTalkSfx(uint16 sound, uint32 model);
	uint32 GetStudentMaleTalkSfx(uint16 sound);
	uint32 GetStudentFemaleTalkSfx(uint16 sound);

	uint32 GetSpecialCharacterTalkSfx(uint32 modelIndex, uint16 sound);
	uint32 GetEightBallTalkSfx(uint16 sound); // inlined on PS2
	uint32 GetSalvatoreTalkSfx(uint16 sound); // inlined on PS2
	uint32 GetMistyTalkSfx(uint16 sound);
	uint32 GetOldJapTalkSfx(uint16 sound); // inlined on PS2
	uint32 GetCatalinaTalkSfx(uint16 sound); // inlined on PS2
	uint32 GetBomberTalkSfx(uint16 sound);   // inlined on PS2
	uint32 GetSecurityGuardTalkSfx(uint16 sound);
	uint32 GetChunkyTalkSfx(uint16 sound); // inlined on PS2

	uint32 GetAsianTaxiDriverTalkSfx(uint16 sound); // inlined on PS2
	uint32 GetPimpTalkSfx(uint16 sound);
	uint32 GetNormalMaleTalkSfx(uint16 sound);
	uint32 GetGenericMaleTalkSfx(uint16 sound);
	uint32 GetGenericFemaleTalkSfx(uint16 sound);

	// particles
	void ProcessExplosions(int32 explosion);
	void ProcessFires(int32 entity);
	void ProcessWaterCannon(int32);

	// script objects
	void ProcessScriptObject(int32 id); // inlined on PS2
	void ProcessOneShotScriptObject(uint8 sound);
	void ProcessLoopingScriptObject(uint8 sound);
	void ProcessPornCinema(uint8 sound);
	void ProcessWorkShopScriptObject(uint8 sound);
	void ProcessSawMillScriptObject(uint8 sound);
	void ProcessLaunderetteScriptObject(uint8 sound);
	void ProcessShopScriptObject(uint8 sound);
	void ProcessAirportScriptObject(uint8 sound);
	void ProcessCinemaScriptObject(uint8 sound);
	void ProcessDocksScriptObject(uint8 sound);
	void ProcessHomeScriptObject(uint8 sound);
	void ProcessPoliceCellBeatingScriptObject(uint8 sound);

	// misc
	void ProcessWeather(int32 id);
	void ProcessFrontEnd();
	void ProcessCrane();
	void ProcessProjectiles();
	void ProcessGarages();
	void ProcessFireHydrant();

	// bridge
	void ProcessBridge(); // inlined on PS2
	void ProcessBridgeWarning();
	void ProcessBridgeMotor();
	void ProcessBridgeOneShots();

	// mission audio
	bool8 MissionScriptAudioUsesPoliceChannel(uint32 soundMission);
	void PreloadMissionAudio(Const char *name);
	uint8 GetMissionAudioLoadingStatus();
	void SetMissionAudioLocation(float x, float y, float z);
	void PlayLoadedMissionAudio();
	bool8 IsMissionAudioSampleFinished();
	bool8 IsMissionAudioSamplePlaying() { return m_sMissionAudio.m_nPlayStatus == PLAY_STATUS_PLAYING; }
	bool8 ShouldDuckMissionAudio() { return IsMissionAudioSamplePlaying(); }
	void ClearMissionAudio();
	void ProcessMissionAudio();

	// police radio
	void InitialisePoliceRadioZones();
	void InitialisePoliceRadio();
	void ResetPoliceRadio();
	void SetMissionScriptPoliceAudio(uint32 sfx);
	int8 GetMissionScriptPoliceAudioPlayingStatus();
	void DoPoliceRadioCrackle();
	void ServicePoliceRadio();
	void ServicePoliceRadioChannel(uint8 wantedLevel);
	bool8 SetupCrimeReport();
	void SetupSuspectLastSeenReport();
	void ReportCrime(eCrimeType crime, const CVector &pos);
	void PlaySuspectLastSeen(float x, float y, float z);
	void AgeCrimes(); // inlined on PS2
		
	// collision stuff
	void ReportCollision(CEntity *entity1, CEntity *entity2, uint8 surface1, uint8 surface2, float collisionPower, float intensity2);
	void ServiceCollisions();
	void SetUpOneShotCollisionSound(const cAudioCollision &col);
	void SetUpLoopingCollisionSound(const cAudioCollision &col, uint8 counter);
	uint32 SetLoopingCollisionRequestedSfxFreqAndGetVol(const cAudioCollision &audioCollision);
	float GetCollisionOneShotRatio(uint32 a, float b);
	float GetCollisionLoopingRatio(uint32 a, uint32 b, float c); // not used
	float GetCollisionRatio(float a, float b, float c, float d); // inlined on PS2
};

/*
   Manual loop points are not on PS2 so let's have these macros to avoid massive ifndefs.
   Setting these manually was pointless anyway since they never change from sdt values.
   What were they thinking?
*/
#ifndef GTA_PS2
#define RESET_LOOP_OFFSETS \
	m_sQueueSample.m_nLoopStart = 0; \
	m_sQueueSample.m_nLoopEnd = -1;
#define SET_LOOP_OFFSETS(sample) \
	m_sQueueSample.m_nLoopStart = SampleManager.GetSampleLoopStartOffset(sample); \
	m_sQueueSample.m_nLoopEnd = SampleManager.GetSampleLoopEndOffset(sample);
#else
#define RESET_LOOP_OFFSETS
#define SET_LOOP_OFFSETS(sample)
#endif
#ifdef EXTERNAL_3D_SOUND
#define SET_EMITTING_VOLUME(vol) m_sQueueSample.m_nEmittingVolume = vol
#else
#define SET_EMITTING_VOLUME(vol)
#endif

#if defined(AUDIO_MSS) && !defined(PS2_AUDIO_CHANNELS)
static_assert(sizeof(cAudioManager) == 19220, "cAudioManager: error");
#endif

extern cAudioManager AudioManager;
