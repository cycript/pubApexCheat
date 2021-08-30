#include "Threads.h"
#include "Database.h"
#include <Windows.h>
#include <chrono>
#include <Dwmapi.h> 
#pragma comment(lib, "dwmapi.lib")



static Vector3 RCSEx_vecOldAngle;
static Vector3 RCSEx_vecViewAngle;
static Vector3 RCSEx_vecFinalAngle;

//Threads::Variables::Get().bActive
static uintptr_t aimEntity = 0;
int EntID = -1;
float MinFov = 6000.f;
bool lock = false;

static int globalbone = 2;


void RCS(Vector3 vecTargetViewAngle)
{
	uintptr_t pLocalPlayer = ApexLegends::GetLocalPlayer();
	if (pLocalPlayer)
	{
		Vector3 vecAimPunch = ApexLegends::GetLocalAimPunch();
		if (GetAsyncKeyState(VK_LBUTTON))
		{
			RCSEx_vecViewAngle.X = vecTargetViewAngle.X + RCSEx_vecOldAngle.X;
			RCSEx_vecViewAngle.Y = vecTargetViewAngle.Y + RCSEx_vecOldAngle.Y;
			RCSEx_vecFinalAngle.X = vecTargetViewAngle.X - (vecAimPunch.X);
			RCSEx_vecFinalAngle.Y = vecTargetViewAngle.Y - (vecAimPunch.Y);
			Vector3 vecFinalAngle = Maths::FinalizeAngle(RCSEx_vecFinalAngle);
			SetViewAngle(vecFinalAngle);
			RCSEx_vecOldAngle = RCSEx_vecFinalAngle;
		}
		else
		{
			RCSEx_vecOldAngle = { 0.0f, 0.0f, 0.0f };
		}
	}
}
void FlushPlayerCache()
{
	for (int i = 0; i < 60; i++)
	{
		APEXVars.visiblePlayerInfo[i] = 0.f;
		APEXVars.Visible[i] = false;
		APEXVars.EntityFov[i] = 0;
	}
	Sleep(5);

}
void Threads::Itemglow()
{
	while (true)
	{
		if (CFG.bGlowItems) {
			uintptr_t pLocalPlayer = ApexLegends::GetLocalPlayer();
			if (pLocalPlayer)
			{
				for (int i = 60; i <= 10000; i++)
				{
					uintptr_t pEntityBase = ApexLegends::GetEntityBase(i);
					if (pEntityBase)
					{
						if (ApexLegends::isEntityAnItem(pEntityBase)) {
							//printf("got here \n");
							if (Maths::CalculateDistance(ApexLegends::GetEntityPosition(pLocalPlayer), ApexLegends::GetEntityPosition(pEntityBase)) < ((CFG.MaxDistance / 3) * 40))
								ApexLegends::EnableItemGlow(pEntityBase);
						}
					}
					Sleep(1);
				}
			}
		}
		CRC();
		Sleep(2000);
		if (CFG.bRandomBones)
		{
			CFG.bone = CFG.BoneIds[(rand() % (5 - 1 + 1) + 1) - 1];
			globalbone = CFG.bone;
		}
	}
}
uintptr_t GetBestAimEntity(uintptr_t pLocalPlayer)
{
	MinFov = CFG.Fov;
	EntID = -1;
	for (int i = 0; i <= 60; i++) {

		if (CFG.bVischeck)
		{
			if (!APEXVars.Visible[i])
			{
				continue;
			}
		}
		uintptr_t pEntity = ApexLegends::GetEntityBase(i);
		if (pEntity && ApexLegends::IsPlayer(pEntity) && !ApexLegends::isDowned(pEntity) && ApexLegends::IsAlive(pEntity) && ApexLegends::IsEnemy(pEntity)) {
			float flDistance = Maths::CalculateDistance(ApexLegends::GetEntityPosition(pLocalPlayer), ApexLegends::GetEntityPosition(pEntity));
			if (flDistance > CFG.MaxDistance * 40) continue;
			Vector vecLocalBonePosition = ApexLegends::GetEntityBonePos(pLocalPlayer, 8);
			Vector vecEntityBonePositionFoV = ApexLegends::GetEntityBonePos(pEntity, 8);
			auto vecLocalViewAngle = ApexLegends::GetViewAngle(pLocalPlayer);
			float flCalculatedFov = Maths::GetFov(vecLocalViewAngle, Maths::FinalQAngle(Maths::CalcAngle(vecLocalBonePosition, vecEntityBonePositionFoV)));
			if (flCalculatedFov < MinFov)
			{

				MinFov = flCalculatedFov;
				EntID = i;
			}
		}
	}
	if (MinFov > CFG.Fov) return 0;
	if (EntID >= 0)
		return ApexLegends::GetEntityBase(EntID);
	return 0;
}
void Threads::ApexLegendsAimThread()
{
	while (CFG.bActive)
	{
		if (CFG.bAimbot) {
			uintptr_t pLocalPlayer = ApexLegends::GetLocalPlayer();
			if (pLocalPlayer) {
				if (GetAsyncKeyState(CFG.dwAimKey))
				{
					uintptr_t pEnemy = GetBestAimEntity(pLocalPlayer);
					if (pEnemy != 0 && pEnemy != pLocalPlayer)
					{
						int LocalBone = CFG.bone;
						int id = EntID;
						float flDistance = Maths::CalculateDistance(ApexLegends::GetEntityPosition(pLocalPlayer), ApexLegends::GetEntityPosition(pEnemy));
						if (flDistance > CFG.MaxDistance * 40) continue;
						float maxFOV = CFG.Fov;
						if (CFG.bClosestBone)
						{
							for (int i = 0; i < 100; i++)
							{
								auto vecLocalBonePosition = ApexLegends::GetEntityBonePos(pLocalPlayer, 8);
								Vector vecEntityBonePositionFoV = ApexLegends::GetEntityBonePos(pEnemy, i);
								auto vecLocalViewAngle = ApexLegends::GetViewAngle(pLocalPlayer);
								float flBoneFoV = Maths::GetFov(vecLocalViewAngle, Maths::FinalQAngle(Maths::CalcAngle(vecLocalBonePosition, vecEntityBonePositionFoV)));
								if (flBoneFoV < maxFOV)
								{
									maxFOV = flBoneFoV;
									LocalBone = i;
								}
							}
						}
						while (true)
						{
							Vector vecLocalBonePosition = ApexLegends::GetEntityBonePos(pLocalPlayer, 8);
							Vector vecEntityBonePositionFoV = ApexLegends::GetEntityBonePos(pEnemy, LocalBone);
							auto vecLocalViewAngle = ApexLegends::GetViewAngle(pLocalPlayer);
							float flCalculatedFov = Maths::GetFov(vecLocalViewAngle, Maths::FinalQAngle(Maths::CalcAngle(vecLocalBonePosition, vecEntityBonePositionFoV)));
							float localSmooth = CFG.flSmooth;
							if (flCalculatedFov > CFG.Fov)
								break;

							// ranged smooth
							// fov based smooth
							
							if (CFG.Safebot)
							{
								if (flCalculatedFov < (1 - ((flDistance / 100) / 10))) continue;
							}
							
							if (CFG.RandomizeSmoothness)
							{
								 localSmooth = (CFG.MinSmooth + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (CFG.flSmooth - CFG.MinSmooth)))) * 1.2;
							}
							if (CFG.DyanamicSmooth)
							{
								if (flCalculatedFov > 1)
								{
									localSmooth += 37.f;
									localSmooth = localSmooth / (flCalculatedFov / 4);
								}
							}
							if (CFG.bRandomBones) 
							{
								LocalBone = globalbone;
							}
							auto absVelocity = ApexLegends::GetEntityAbsVelocity(pEnemy);
							absVelocity *= CFG.flSmooth / 40.f ;
							Vector vecVelocity = { absVelocity.X, absVelocity.Y, absVelocity.Z };
							QAngle vecAimAngle = Maths::CalculatePredictionAngle(pLocalPlayer, ApexLegends::GetViewAngle(pLocalPlayer), ApexLegends::GetEntitySwayAngle(pLocalPlayer), ApexLegends::GetEntityBonePos(pEnemy, LocalBone), vecVelocity, localSmooth * 17.f );
							if (vecAimAngle.x != 0 && vecAimAngle.y != 0)
							{
								ApexLegends::SetViewQAngle(vecAimAngle);
							}
							if (!GetAsyncKeyState(CFG.dwAimKey)) {
								FlushPlayerCache();
								break;
							}
							if (CFG.bVischeck)
							{
								if (!APEXVars.Visible[id])
								{
									FlushPlayerCache();
									break;
								}
							}
						}
						//Sleep(100);
					}
				}
				else if (GetAsyncKeyState(VK_LBUTTON) && CFG.bLeftClickAim)
				{
					uintptr_t pEnemy = GetBestAimEntity(pLocalPlayer);
					if (pEnemy != 0 && pEnemy != pLocalPlayer)
					{
						int LocalBone = CFG.bone;
						int id = EntID;
						float flDistance = Maths::CalculateDistance(ApexLegends::GetEntityPosition(pLocalPlayer), ApexLegends::GetEntityPosition(pEnemy));
						if (flDistance > CFG.MaxDistance * 40) continue;
						float maxFOV = CFG.Fov;
						if (CFG.bClosestBone)
						{
							for (int i = 0; i < 100; i++)
							{
								auto vecLocalBonePosition = ApexLegends::GetEntityBonePos(pLocalPlayer, 8);
								Vector vecEntityBonePositionFoV = ApexLegends::GetEntityBonePos(pEnemy, i);
								auto vecLocalViewAngle = ApexLegends::GetViewAngle(pLocalPlayer);
								float flBoneFoV = Maths::GetFov(vecLocalViewAngle, Maths::CalcAngle(vecLocalBonePosition, vecEntityBonePositionFoV));
								if (flBoneFoV < maxFOV)
								{
									maxFOV = flBoneFoV;
									LocalBone = i;
								}
							}
						}
						while (true)
						{
							Vector vecLocalBonePosition = ApexLegends::GetEntityBonePos(pLocalPlayer, 8);
							Vector vecEntityBonePositionFoV = ApexLegends::GetEntityBonePos(pEnemy, LocalBone);
							auto vecLocalViewAngle = ApexLegends::GetViewAngle(pLocalPlayer);
							float flCalculatedFov = Maths::GetFov(vecLocalViewAngle, Maths::CalcAngle(vecLocalBonePosition, vecEntityBonePositionFoV));
							float localSmooth = CFG.flSmooth;
							if (flCalculatedFov > CFG.Fov)
								break;
							// ranged smooth
							// fov based smooth
							if (CFG.Safebot)
							{
								if (flCalculatedFov < (1 - ((flDistance / 100) / 10))) continue;
							}

							if (CFG.RandomizeSmoothness)
							{
								localSmooth = (CFG.MinSmooth + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (CFG.flSmooth - CFG.MinSmooth)))) * 1.2;
							}
							if (CFG.DyanamicSmooth)
							{
								if (flCalculatedFov > 1)
								{
									localSmooth += 37.f;
									localSmooth = localSmooth / (flCalculatedFov / 4);
								}
							}
							if (CFG.bRandomBones)
							{
								LocalBone = globalbone;
							}
							auto absVelocity = ApexLegends::GetEntityAbsVelocity(pEnemy);
							absVelocity *= CFG.flSmooth / 40.f;
							Vector vecVelocity = { absVelocity.X, absVelocity.Y, absVelocity.Z };
							QAngle vecAimAngle = Maths::CalculatePredictionAngle(pLocalPlayer, ApexLegends::GetViewAngle(pLocalPlayer), ApexLegends::GetEntitySwayAngle(pLocalPlayer), ApexLegends::GetEntityBonePos(pEnemy, CFG.bone), vecVelocity, localSmooth * 17.f);
							if (vecAimAngle.x != 0 && vecAimAngle.y != 0)
							{
						
								//vecAimAngle.x = static_cast<int>(vecAimAngle.x);
								//vecAimAngle.y = static_cast<int>(vecAimAngle.y);
								ApexLegends::SetViewQAngle(vecAimAngle);
							}
							if (!GetAsyncKeyState(VK_LBUTTON))
							{
								//FlushPlayerCache();
								break;
							}
							if (CFG.bVischeck)
							{
								if (!APEXVars.Visible[id])
								{
									//	FlushPlayerCache();
									break;
								}
							}
						}
						//Sleep(100);
					}
				}
			}
			
		}
		Sleep(1);
	}
}

void Threads::Glow()
{
	while (true)
	{
		uintptr_t pLocalPlayer = ApexLegends::GetLocalPlayer();
		MinFov = CFG.Fov;
		EntID = -1;
		if (pLocalPlayer) {
			for (int i = 0; i <= 60; i++)
			{

				APEXVars.EntityFov[i] = 900.f;
				uintptr_t pEntityBase = ApexLegends::GetEntityBase(i);
				double flCurrentVisTime = -1.f;

				if (pEntityBase && pEntityBase != NULL)
				{
					if ((pEntityBase == pLocalPlayer))
					{
						APEXVars.localname = ApexLegends::GetPlayerName(i);
						//printf("LocalName : %s  \n",APEXVars.localname);
					}
					APEXVars.PlayerNames[i] = ApexLegends::GetPlayerName(i);
					if (ApexLegends::IsPlayer(pEntityBase))
					{
						bool isEnemy =  ApexLegends::IsEnemy(pEntityBase);
						if (isEnemy) {
							if (Maths::CalculateDistance(ApexLegends::GetEntityPosition(pLocalPlayer), ApexLegends::GetEntityPosition(pEntityBase)) < (CFG.MaxDistance * 40.f)) {
								if (CFG.bGlowPlayers)
									ApexLegends::GlowESP(pEntityBase);
							}
						}
						if (pEntityBase == pLocalPlayer || !isEnemy || !ApexLegends::IsAlive(pEntityBase)) {
							APEXVars.EntityFov[i] = 999.f;
							APEXVars.Visible[i] = false;
							continue;
						}

						// IF ENTITY IS NOT TEAMMATE AND NOT LOCALPLAYER
						flCurrentVisTime = ApexLegends::LastVisibleTime(pEntityBase);
						if (flCurrentVisTime > APEXVars.visiblePlayerInfo[i] || flCurrentVisTime < 0.f && APEXVars.visiblePlayerInfo[i] > 0.f)
						{
							APEXVars.Visible[i] = true;
						}
						else
						{
							APEXVars.Visible[i] = false;
						}
						APEXVars.PlayersHealth[i] = Memory::ReadProcessMemory<int>(APEXVars.dwProcessId,pEntityBase + APEXOffsets.OFFSET_HEALTH);
						APEXVars.PlayersArmor[i] = Memory::ReadProcessMemory<int>(APEXVars.dwProcessId, pEntityBase + APEXOffsets.OFFSET_SHIELD);
						APEXVars.MaxPlayerArmor[i] = Memory::ReadProcessMemory<int>(APEXVars.dwProcessId, pEntityBase + APEXOffsets.OFFSET_MAX_SHIELD);
						Vector vecLocalBonePosition = ApexLegends::GetEntityBonePos(pLocalPlayer, 8);
						Vector vecEntityBonePositionFoV = ApexLegends::GetEntityBonePos(pEntityBase, 8);
						auto vecLocalViewAngle = ApexLegends::GetViewAngle(pLocalPlayer);
						float flCalculatedFov = Maths::GetFov(vecLocalViewAngle, Maths::FinalQAngle(Maths::CalcAngle(vecLocalBonePosition, vecEntityBonePositionFoV)));
						APEXVars.EntityFov[i] = flCalculatedFov;
					}
				}
			//	Sleep(1);
				APEXVars.visiblePlayerInfo[i] = flCurrentVisTime;
			}
		}
		Sleep(50);
	
	}
}

void Threads::CRC()
{
	
DWORD dwCurrentProcessId = GetCurrentProcessId();
	if (!HWIDVars.bInitialized)
	{
		Database::SQL::Account::Ban("HWIDUNINITIALIZED");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	if (Utilities::IsProcessBeingDebugged64(dwCurrentProcessId))
	{
		Database::SQL::Account::Ban("PEDEBUG");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	if (!Database::SQL::IsInitialized())
	{
		Database::SQL::Account::Ban("CREDUNINITIALIZED");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	if (Database::SQL::Account::GetBanState())
	{
		Database::SQL::Account::Ban("SVBAN");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	if (Utilities::GetProcessIdByContainingString(L"cheatengine"))
	{
		Database::SQL::Account::Ban("UMCELOAD");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	if (Utilities::GetProcessIdByContainingString(L"ida64"))
	{
		Database::SQL::Account::Ban("UMIDALOAD");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	if (Utilities::GetProcessIdByContainingString(L"ida32"))
	{
		Database::SQL::Account::Ban("UMIDALOAD");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	if (Utilities::GetProcessIdByContainingString(L"olly"))
	{
		Database::SQL::Account::Ban("UMOLLYDBGLOAD");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	if (Utilities::GetProcessIdByContainingString(L"dbg"))
	{
		Database::SQL::Account::Ban("UMDBGLOAD");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	if (Utilities::GetProcessIdByContainingString(L"WindowsKernelExplorer"))
	{
		Database::SQL::Account::Ban("UMWKELOAD");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	if (Utilities::GetProcessIdByContainingString(L"fiddler"))
	{
		Database::SQL::Account::Ban("UMFIDDLERLOAD");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	if (Utilities::GetProcessIdByContainingString(L"telerik"))
	{
		Database::SQL::Account::Ban("UMTELERIKLOAD");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	if (Utilities::GetProcessIdByContainingString(L"charles"))
	{
		Database::SQL::Account::Ban("UMCHARLESLOAD");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	if (Utilities::GetProcessIdByContainingString(L"burp"))
	{
		Database::SQL::Account::Ban("UMBURPLOAD");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	if (Utilities::GetProcessIdByContainingString(L"burpsuite"))
	{
		Database::SQL::Account::Ban("UMBURPLOAD");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	if (Utilities::GetProcessIdByContainingString(L"manualmap"))
	{
		Database::SQL::Account::Ban("UMMMLOAD");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	if (Utilities::GetProcessIdByContainingString(L"spoof"))
	{
		Database::SQL::Account::Ban("UMSPOOFERLOAD");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	if (Utilities::GetProcessIdByContainingString(L"hacker"))
	{
		Database::SQL::Account::Ban("UMHCKRLOAD");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	if (Utilities::GetProcessIdByContainingString(L"kdmapper"))
	{
		Database::SQL::Account::Ban("UMKDMLOAD");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	if (Utilities::GetProcessIdByContainingString(L"drvmap"))
	{
		Database::SQL::Account::Ban("UMDRVMLOAD");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	if (Utilities::GetProcessIdByContainingString(L"capcom"))
	{
		Database::SQL::Account::Ban("UMCPCMLOAD");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	if (Utilities::GetProcessIdByContainingString(L"mmap"))
	{
		Database::SQL::Account::Ban("UMMMLOAD");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	/*if (Utilities::GetProcessIdByContainingString(L"inj"))
	{
		Database::SQL::Account::Ban("UMINJLOAD");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	if (Utilities::GetProcessIdByContainingString(L"inje"))
	{
		Database::SQL::Account::Ban("UMINJLOAD");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	if (Utilities::GetProcessIdByContainingString(L"injector"))
	{
		Database::SQL::Account::Ban("UMINJLOAD");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}*/
	if (Utilities::GetProcessIdByContainingString(L"ProcessHacker"))
	{
		Database::SQL::Account::Ban("UMPROHKLOAD");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	if (Utilities::GetKernelModuleAddress("dbk64.sys") != 0)
	{
		Database::SQL::Account::Ban("KEDBKCELOAD");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	if (Utilities::GetKernelModuleAddress("WindowsKernelExplorer.sys") != 0)
	{
		Database::SQL::Account::Ban("KEWKECELOAD");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	if (Utilities::GetKernelModuleAddress("kprocesshacker.sys") != 0)
	{
		Database::SQL::Account::Ban("KEPRCHKRCELOAD");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	if (Utilities::GetKernelModuleAddress("kdmapper.sys") != 0)
	{
		Database::SQL::Account::Ban("KEKDMLOAD");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	if (Utilities::GetKernelModuleAddress("capcom.sys") != 0)
	{
		Database::SQL::Account::Ban("KECPCMLOAD");
		mhyprotrose::TerminateProcess(dwCurrentProcessId);
	}
	//Sleep(1000);
}