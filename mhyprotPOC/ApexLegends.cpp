#include "ApexLegends.h"




char* get_class_name(uint64_t entity_ptr)
{
	char out_str[33];
	uint64_t client_networkable_vtable = Memory::ReadProcessMemory<uint64_t>(APEXVars.dwProcessId, + (entity_ptr + 8 * 3));
	uint64_t get_client_class = Memory::ReadProcessMemory<uint64_t>(APEXVars.dwProcessId,  (client_networkable_vtable + 8 * 3));
	uint32_t disp = Memory::ReadProcessMemory<uint32_t>(APEXVars.dwProcessId,  (get_client_class + 3));
	const uint64_t client_class_ptr = get_client_class + disp + 7;
	ClientClass client_class = Memory::ReadProcessMemory<ClientClass>(APEXVars.dwProcessId, (client_class_ptr));
	if (Memory::ReadProcessMemory(APEXVars.dwProcessId, client_class.pNetworkName, &out_str, size_t(33))) {
	}
	return out_str;
}
uintptr_t ApexLegends::GetLocalPlayer()
{
	return  Memory::ReadProcessMemory<uintptr_t>(APEXVars.dwProcessId, APEXVars.pBaseAddress + APEXOffsets.dwLocalPLayer);
}

uintptr_t ApexLegends::GetEntityBase(DWORD dwEntityIndex)
{
	return  Memory::ReadProcessMemory<uintptr_t>(APEXVars.dwProcessId, (APEXVars.pBaseAddress + APEXOffsets.dwEntityList) + (dwEntityIndex << 5));
}

bool ApexLegends::IsPlayer(uintptr_t pEntityBase)
{
	return ( Memory::ReadProcessMemory<DWORD64>(APEXVars.dwProcessId, pEntityBase + APEXOffsets.m_iName) == 125780153691248);
}

bool ApexLegends::IsEnemy(uintptr_t pEntityBase)
{
	//return true;
	return ( Memory::ReadProcessMemory<uintptr_t>(APEXVars.dwProcessId, GetLocalPlayer() + APEXOffsets.m_iTeamNum) !=  Memory::ReadProcessMemory<uintptr_t>(APEXVars.dwProcessId, pEntityBase + APEXOffsets.m_iTeamNum));
}
bool ApexLegends::IsAlive(uintptr_t pEntityBase) 
{
	return Memory::ReadProcessMemory<int>(APEXVars.dwProcessId, pEntityBase + APEXOffsets.OFFSET_LIFE_STATE) == 0;
}
string ApexLegends::GetPlayerName(int  EntityID )
{
	EntityID--;
	EntityID *= 0x10;
	uint64_t name_ptr = 0;
	name_ptr =  Memory::ReadProcessMemory<uint64_t>(APEXVars.dwProcessId,APEXVars.pBaseAddress + APEXOffsets.OFFSET_NAME_LIST + EntityID);
	char name[32];
	Memory::ReadProcessMemory(APEXVars.dwProcessId, name_ptr, &name, sizeof(name));
	return string(name);
}
bool ApexLegends::isDowned(uintptr_t pEntityBase) {

	return  Memory::ReadProcessMemory<int>(APEXVars.dwProcessId, pEntityBase + APEXOffsets.OFFSET_BLEED_OUT_STATE) > 0;
}

bool ApexLegends::isEntityAnItem(uintptr_t pEntityBase) {

	char* class_name = get_class_name(pEntityBase);
	return strncmp(class_name, "CPropSurvival", 13) == 0;
}
void ApexLegends::EnableItemGlow(uintptr_t pEntityBase) {

	/*Memory::WriteProcessMemory<int>(APEXVars.dwProcessId, pEntityBase + 0x262, 16256);
	Memory::WriteProcessMemory<int>(APEXVars.dwProcessId, pEntityBase + 0x2dc, 1193322764);
	Memory::WriteProcessMemory<int>(APEXVars.dwProcessId, pEntityBase + 0x3c8, 7);
	Memory::WriteProcessMemory<int>(APEXVars.dwProcessId, pEntityBase + 0x3d0, 2);*/
	Memory::WriteProcessMemory<int>(APEXVars.dwProcessId,pEntityBase + APEXOffsets.OFFSET_ITEM_GLOW, 1363184265);
}
double ApexLegends::LastVisibleTime(uintptr_t pEntityBase)
{
	return  Memory::ReadProcessMemory<double>(APEXVars.dwProcessId, pEntityBase + APEXOffsets.m_flVisible);
}

Vector3 ApexLegends::GetLocalAimPunch()
{
	return  Memory::ReadProcessMemory<Vector3>(APEXVars.dwProcessId, GetLocalPlayer() + APEXOffsets.OFFSET_AIMPUNCH);
}
Vector3 ApexLegends::GetEntityViewAngle(uintptr_t pEntityBase)
{
	return  Memory::ReadProcessMemory<Vector3>(APEXVars.dwProcessId, pEntityBase + APEXOffsets.OFFSET_VIEWANGLES);
}
QAngle ApexLegends::GetViewAngle(uintptr_t pEntityBase)
{
	return  Memory::ReadProcessMemory<QAngle>(APEXVars.dwProcessId, pEntityBase + APEXOffsets.OFFSET_VIEWANGLES);

}
Vector3 ApexLegends::GetSwayAngle(uintptr_t pEntityBase)
{
	return  Memory::ReadProcessMemory<Vector3>(APEXVars.dwProcessId, pEntityBase + APEXOffsets.OFFSET_BREATH_ANGLES);
}
QAngle ApexLegends::GetEntitySwayAngle(uintptr_t pEntityBase)
{
	return  Memory::ReadProcessMemory<QAngle>(APEXVars.dwProcessId, pEntityBase + APEXOffsets.OFFSET_BREATH_ANGLES);
}
Vector3 ApexLegends::GetLocalPosition()
{
	return  Memory::ReadProcessMemory<Vector3>(APEXVars.dwProcessId, GetLocalPlayer() + APEXOffsets.OFFSET_CAMERAPOS);
}
Vector3 ApexLegends::GetEntityAbsVelocity(uintptr_t pEntityBase)
{
	return  Memory::ReadProcessMemory<Vector3>(APEXVars.dwProcessId, pEntityBase + APEXOffsets.OFFSET_ABS_VELOCITY);
}
Vector ApexLegends::GetEntityAbsVelo(uintptr_t pEntityBase)
{
	return  Memory::ReadProcessMemory<Vector>(APEXVars.dwProcessId, pEntityBase + APEXOffsets.OFFSET_ABS_VELOCITY);
}
Vector3 ApexLegends::GetEntityPosition(uintptr_t pEntityBase)
{
	auto pos = Memory::ReadProcessMemory<Vector3>(APEXVars.dwProcessId, pEntityBase + APEXOffsets.OFFSET_ORIGIN);
	return  pos;
}
Vector ApexLegends::GetEntityPos(uintptr_t pEntityBase)
{
	auto pos = Memory::ReadProcessMemory<Vector>(APEXVars.dwProcessId, pEntityBase + APEXOffsets.OFFSET_ORIGIN);
	return pos;
}
Vector3 GetEntityBasePosition(uintptr_t pEntityBase)
{
	auto pos = Memory::ReadProcessMemory<Vector3>(APEXVars.dwProcessId, pEntityBase + APEXOffsets.OFFSET_ORIGIN);
	return  pos;
}
Vector3 ApexLegends::GetEntityBonePosition(uintptr_t pEntityBase, uint32_t dwBoneId)
{
	uintptr_t pBone =  Memory::ReadProcessMemory<uintptr_t>(APEXVars.dwProcessId, pEntityBase + APEXOffsets.OFFSET_BONES);
	matrix3x4_t matBonePosition = Memory::ReadProcessMemory<matrix3x4_t>(APEXVars.dwProcessId, pBone + (dwBoneId * 0x30));
	Vector3 vecBoneBosition = { matBonePosition[0][3], matBonePosition[1][3], matBonePosition[2][3] };
	return GetEntityPosition(pEntityBase) + vecBoneBosition;
}
Vector ApexLegends::GetEntityBonePos(uintptr_t pEntityBase, uint32_t dwBoneId)
{
	uintptr_t pBone =  Memory::ReadProcessMemory<uintptr_t>(APEXVars.dwProcessId, pEntityBase + APEXOffsets.OFFSET_BONES);

	Vector EntityHead = Vector();
	Vector3 BasePosition = GetEntityBasePosition(pEntityBase);
	EntityHead.x =  Memory::ReadProcessMemory<float>(APEXVars.dwProcessId,(pBone + 0xCC + (dwBoneId * 0x30))) + BasePosition.X;
	EntityHead.y =  Memory::ReadProcessMemory<float>(APEXVars.dwProcessId, (pBone + 0xDC + (dwBoneId * 0x30))) + BasePosition.Y;
	EntityHead.z =  Memory::ReadProcessMemory<float>(APEXVars.dwProcessId, (pBone + 0xEC + (dwBoneId * 0x30))) + BasePosition.Z;
	return EntityHead;
}

void ApexLegends::SetViewAngle(Vector3 vecViewAngle)
{
	Memory::WriteProcessMemory<Vector3>(APEXVars.dwProcessId, GetLocalPlayer() + APEXOffsets.OFFSET_VIEWANGLES, vecViewAngle);
}

void ApexLegends::SetViewQAngle(QAngle vecViewAngle)
{
	Memory::WriteProcessMemory<QAngle>(APEXVars.dwProcessId, GetLocalPlayer() + APEXOffsets.OFFSET_VIEWANGLES, vecViewAngle);
}
void ApexLegends::GlowESP(uintptr_t pEntityBase)
{	
	Memory::WriteProcessMemory<int>(APEXVars.dwProcessId, pEntityBase + 0x262, 16256);
	Memory::WriteProcessMemory<int>(APEXVars.dwProcessId, pEntityBase + 0x2dc, 1193322764);
	Memory::WriteProcessMemory<int>(APEXVars.dwProcessId, pEntityBase + 0x3c8, 7);
	Memory::WriteProcessMemory<int>(APEXVars.dwProcessId, pEntityBase + 0x3d0, 2);
}



Vector3 ApexLegends::WorldToScreen(Vector3 from, float targetWidth, float targetHeight)
{
	Vector3 to = Vector3();

	uint64_t viewRenderer = Memory::ReadProcessMemory<uint64_t>(APEXVars.dwProcessId,APEXVars.pBaseAddress +APEXOffsets.OFFSET_VIEWRENDER);
	uint64_t viewMatriX = Memory::ReadProcessMemory<uint64_t>(APEXVars.dwProcessId, viewRenderer + APEXOffsets.OFFSET_MATRIX);
	MatrixA m = Memory::ReadProcessMemory<MatrixA>(APEXVars.dwProcessId, viewMatriX);
	auto m_vMatriX = m.matrix;

	float w = m_vMatriX[12] * from.X + m_vMatriX[13] * from.Y + m_vMatriX[14] * from.Z + m_vMatriX[15];

	if (w < 0.01f) return Vector3();

	to.X = m_vMatriX[0] * from.X + m_vMatriX[1] * from.Y + m_vMatriX[2] * from.Z + m_vMatriX[3];
	to.Y = m_vMatriX[4] * from.X + m_vMatriX[5] * from.Y + m_vMatriX[6] * from.Z + m_vMatriX[7];

	float invw = 1.0f / w;
	to.X *= invw;
	to.Y *= invw;

	float X = targetWidth / 2; 
	float Y = targetHeight / 2;

	X += 0.5f * to.X * targetWidth + 0.5f;
	Y -= 0.5f * to.Y * targetHeight + 0.5f;

	to.X = X;
	to.Y = Y;
	to.Z = 0;
	return to;
}

bool ApexLegends::W2S(Vector from, float* m_vMatrix, int targetWidth, int targetHeight, Vector& to)
{
	float w = m_vMatrix[12] * from.x + m_vMatrix[13] * from.y + m_vMatrix[14] * from.z + m_vMatrix[15];

	if (w < 0.01f) return false;

	to.x = m_vMatrix[0] * from.x + m_vMatrix[1] * from.y + m_vMatrix[2] * from.z + m_vMatrix[3];
	to.y = m_vMatrix[4] * from.x + m_vMatrix[5] * from.y + m_vMatrix[6] * from.z + m_vMatrix[7];

	float invw = 1.0f / w;
	to.x *= invw;
	to.y *= invw;

	float x = targetWidth / 2;
	float y = targetHeight / 2;

	x += 0.5 * to.x * targetWidth + 0.5;
	y -= 0.5 * to.y * targetHeight + 0.5;

	to.x = x;
	to.y = y;
	to.z = 0;
	return true;
}