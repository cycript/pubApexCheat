#pragma once

#include "MD5.h"
#include "Memory.h"
#include "Singleton.h"
#include "KeStructs.h"
#include "GAC_Driver.h"
#include "mhyprot_Driver.h"
#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>
#include <mhyprotrose.h>

#define INRANGE(x,a,b)   (x >= a && x <= b)
#define GET_BYTE( x )    (GET_BITS(x[0]) << 4 | GET_BITS(x[1]))
#define GET_BITS( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getBits( x )		(INRANGE(x,'0','9') ? (x - '0') : ((x&(~0x20)) - 'A' + 0xa))
#define getByte( x )		(getBits(x[0]) << 4 | getBits(x[1]))

using namespace std;

namespace Driver
{
	static const char* chMhyprotDriverDirectory = "C:/Windows/system32/drivers/mhyprot2.sys";
	static const char* chMhyprotDriverServiceName = "mhyprot2";
	static const char* chGACDriverDirectory = "C:/Windows/system32/drivers/GAC_DE.sys";
	static const char* chGACDriverServiceName = "GAC_DE";

	bool CreateFileFromMemory(const std::string& strDesiredFilePath, const char* chAddress, size_t szSize);
	bool WriteMihoyoDriverMemoryIntoFile();
	bool WriteGACDriverMemoryIntoFile();
	bool DeleteDriverFile(const char* chDriverDirectory);
	bool CreateAndStartDriverService(const char* chServiceName, const char* chDriverDirectory);
	bool StopAndDeleteDriverService(const char* chServiceName, const char* chDriverDirectory);
	bool ServiceIsRunning(const char* chServiceName);
	void StopAndDeleteAllDrivers();
}

namespace Utilities
{
	class Variables : public Singleton<Variables>
	{
	public:
		////https://www.vergiliusproject.com/kernels/x64/Windows%2010%20%7C%202016/2009%2020H2%20(October%202020%20Update)/_EPROCESS
		uintptr_t pSystemBaseOffset = 0x0;
		uintptr_t pImageFileNameOffset = 0x5A8; // EPROCESS->
		uintptr_t pUniqueProcessIdOffset = 0x440; // EPROCESS->
		uintptr_t pSectionBaseOffset = 0x520; // EPROCESS->
		uintptr_t pActiveProcessLinkOffset = 0x448; // EPROCESS->
		uintptr_t pPEBOffset = 0x550; // EPROCESS ->
	};
	#define UtilVars Utilities::Variables::Get()
	void GetDesktopResolution(int& horizontal, int& vertical);
	PCHAR MD5(PCHAR chText);
	string StrReplace(std::string str, const std::string& from, const std::string& to);
	bool ReadFileToMemory(const std::string& strFilePath, std::vector<uint8_t>* pOutBuffer);
	bool ConvertFileToByteArray(const char* chFileName, const char* chFileDestination);
	bool UpdateEPROCESSOffsets();
	uintptr_t TranslateLinearAddress(uint64_t pDirectoryTableBaseAddress, uint64_t pVirtualAddress);
	uintptr_t SFGetEPROCESS(int dwProcessId);
	uintptr_t GetEPROCESS(int dwProcessId);
	uintptr_t GetKernelModuleAddress(const std::string& strModuleName);
	uintptr_t GetProcessBaseAddress(int dwProcessId);
	DWORD GetProcessIdByName(const wchar_t* chProcessName);
	DWORD GetProcessIdByContainingString(const wchar_t* wcProcessName);
	bool IsProcessBeingDebugged64(int dwProcessId);
	ULONG GetModuleBaseAddress32(int dwProcessId, const wchar_t* wcModuleName, DWORD* pdwSize, DWORD dwMaxCount = 1024);
	uintptr_t GetModuleBaseAddress64(int dwProcessId, const wchar_t* wcModuleName, DWORD* pdwSize);
	uintptr_t SearchSignature(DWORD dwProcessId, uintptr_t pStartAddress, SIZE_T szLength, BYTE* bPattern, const char* chMask);
	uintptr_t FindPattern(DWORD dwProcessId, uintptr_t pModuleAddress, SIZE_T szModuleSize, BYTE* bPattern, const char* chMask);
	uintptr_t ExtractOffsetByPattern(DWORD dwProcessId, uintptr_t pModuleAddress, SIZE_T szModuleSize, ULONG ulAdditionalOffset, ULONG ulExtra, const char* chPattern, const char* chMask, BOOLEAN bRelative);
	OSVERSIONINFOEXW GetOSVersionInfo();
};