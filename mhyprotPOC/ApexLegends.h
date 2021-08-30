#pragma once
#ifndef APEXLEGENDS_H

#include <iostream>
#include <Windows.h>
#include "Structs.h"
#include "Utilities.h"
#include "Singleton.h"
#include "vector.h"
#include "Memory.h"

enum BoneIDs {
	Head = 8,
	Neck = 7,
	UpperChest = 5,
	Chest =3,
	Stomach =2,
};

inline namespace ApexLegends
{
	struct MatrixA
	{
		float matrix[16];
	};
	struct ClientClass {
		uint64_t pCreateFn;
		uint64_t pCreateEventFn;
		uint64_t pNetworkName;
		uint64_t pRecvTable;
		uint64_t pNext;
		uint32_t ClassID;
		uint32_t ClassSize;
	};
	class Variables : public Singleton<Variables>
	{
	public:
		const char* chProcessName = "r5apex.exe";
		const wchar_t* wcProcessName = L"r5apex.exe";
		DWORD dwProcessId;
		//VMMDLL_MAP_MODULEENTRY modApex;
		uintptr_t pBaseAddress;
		double visiblePlayerInfo[61] = { };
		float EntityFov[61];
		bool Visible[61];
		string PlayerNames[61];
		int PlayersHealth[61];
		int PlayersArmor[61];
		int MaxPlayerArmor[61];
		int localid = 0;
		string localname;
	};

#define APEXVars ApexLegends::Variables::Get()

	class Offsets : public Singleton<Offsets>
	{
	public:
		DWORD ENTITY_SIZE_DEF = 0x42E0;
		DWORD dwLocalPLayer = 0x1cc0ce8;
		DWORD dwEntityList = 0x1911448;
		DWORD m_iTeamNum = 0x448;
		DWORD m_iHealth = 0x438;
		DWORD m_iName = 0x589;
		DWORD m_iSignifierName = 0x0580;
		DWORD m_flVisible = 0x1ACC;
		DWORD m_shotCount = 0x181c;
		DWORD GLOW_ENABLE = 0x350; //Enable //NO
		DWORD GLOW_CONTEXT = 0x03C8; //Context //OK
		DWORD GLOW_LIFE_TIME = 0x3A4; //Life Time //OK
		DWORD GLOW_DISTANCE = 0x3B4; //Distance //OK
		DWORD GLOW_TYPE = 0x2C4; //Type //NO
		DWORD GLOW_COLOR = 0x1D0; //Color //OK
		DWORD GLOW_VISIBLE_TYPE = 0x3D0; //Visible Type
		DWORD GLOW_FADE = 0x388; //Fade //NO
		DWORD OFFSET_VIEWANGLES = 0x2518;
		DWORD OFFSET_ORIGIN = 0x14C;
		DWORD OFFSET_BONES = 0xF38;
		DWORD OFFSET_AIMPUNCH = 0x2430;
		DWORD OFFSET_CAMERAPOS = 0x1ED0;
		DWORD OFFSET_ENTITYLIST = 0x18EDA78; // Only Origin
		DWORD OFFSET_WEAPON = 0x1a6c;//player + weapon
		DWORD OFFSET_BULLET_SPEED = 0x1ED0;
		DWORD OFFSET_BULLET_SCALE = 0x1ED8;
		DWORD OFFSET_ZOOM_FOV = 0x1678 + 0xb8; //m_playerData + m_curZoomFOV
		DWORD OFFSET_ABS_VELOCITY = 0x140; // enemy velocity
		DWORD OFFSET_BREATH_ANGLES = (OFFSET_VIEWANGLES - 0x10);
		DWORD OFFSET_BLEED_OUT_STATE = 0x26c8;
		DWORD OFFSET_ITEM_GLOW = 0x2c0;
		DWORD OFFSET_VIEWRENDER = 0x41e3588;
		DWORD OFFSET_MATRIX   =     0x1b3bd0;
		DWORD OFFSET_LIFE_STATE = 0x798;
		DWORD OFFSET_NAME_LIST  = 0x8382450;
		DWORD OFFSET_HEALTH = 0x438;
		DWORD OFFSET_SHIELD = 0x170;	//m_shieldHealth
		DWORD OFFSET_MAX_SHIELD = 0x174;
	};

#define APEXOffsets ApexLegends::Offsets::Get()

	uintptr_t GetLocalPlayer();
	uintptr_t GetEntityBase(DWORD dwEntityIndex);
	bool IsPlayer(uintptr_t pEntityBase);
	bool IsEnemy(uintptr_t pEntityBase);
	bool isDowned(uintptr_t pEntityBase);
	bool isEntityAnItem(uintptr_t pEntityBase);
	bool IsAlive(uintptr_t pEntityBase);
	void EnableItemGlow(uintptr_t pEntityBase);
	double LastVisibleTime(uintptr_t pEntityBase);
	string GetPlayerName(int  EntityID);
	Vector3 GetLocalAimPunch();
	Vector3 GetEntityViewAngle(uintptr_t pEntityBase);
	QAngle GetViewAngle(uintptr_t pEntityBase);
	Vector3 GetSwayAngle(uintptr_t pEntityBase);
	QAngle GetEntitySwayAngle(uintptr_t pEntityBase);
	Vector3 GetLocalPosition();
	Vector3 GetEntityAbsVelocity(uintptr_t pEntityBase);
	Vector3 GetEntityPosition(uintptr_t pEntityBase);
	Vector GetEntityPos(uintptr_t pEntityBase);
	Vector GetEntityAbsVelo(uintptr_t pEntityBase);
	Vector3 GetEntityBonePosition(uintptr_t pEntityBase, uint32_t dwBoneId);
	Vector GetEntityBonePos(uintptr_t pEntityBase, uint32_t dwBoneId);
	Vector3 WorldToScreen(Vector3 StartPos, float targetWidth, float targetHeight);
	void SetViewAngle(Vector3 vecViewAngle);
	void SetViewQAngle(QAngle vecViewAngle);
	void GlowESP(uintptr_t pEntityBase);
	bool W2S(Vector from, float* m_vMatrix, int targetWidth, int targetHeight, Vector& to);
}

#endif  !APEXLEGENDS_H
