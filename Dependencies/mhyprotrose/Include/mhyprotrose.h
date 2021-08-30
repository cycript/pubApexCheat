#pragma once

#include <Windows.h>
#include <fstream>
#include <vector>

typedef struct _MHYPROT_THREAD_INFORMATION
{
	uint64_t pKernelAddress;
	uint64_t pStartAddress;
	bool bReserve1;
} MHYPROT_THREAD_INFORMATION, * PMHYPROT_THREAD_INFORMATION;

namespace mhyprotrose
{
	extern bool MhyDebugInitialize();
	extern void MhyDebugUnload();
	extern uint32_t GetSystemUpTime();
	extern bool TerminateProcess(const uint32_t dwProcessId);
	extern bool MhyReadKernelMemory(const uint64_t& pAddress, void* pBuffer, const size_t& szDataSize);
	template<class T> T MhyReadKernelMemory(const uint64_t& pAddress)
	{
		T tBuffer;
		MhyReadKernelMemory(pAddress, &tBuffer, sizeof(T));
		return tBuffer;
	}
	extern bool MhyReadProcessMemory(const uint32_t& dwProcessId, const uint64_t& pAddress, void* pBuffer, const size_t& szDataSize);
	template<class T> T MhyReadProcessMemory(const uint32_t& dwProcessId, const uint64_t& pAddress)
	{
		T tBuffer;
		MhyReadProcessMemory(dwProcessId, pAddress, &tBuffer, sizeof(T));
		return tBuffer;
	}
	extern bool MhyWriteProcessMemory(const uint32_t& dwProcessId, const uint64_t& pAddress, void* pBuffer, const size_t& szDataSize);
	template<class T> bool MhyWriteProcessMemory(const uint32_t& dwProcessId, const uint64_t& pAddress, const T& tBuffer)
	{
		return MhyWriteProcessMemory(dwProcessId, pAddress, (void*)&tBuffer, sizeof(T));
	}
	extern bool GetProcessModuleList(const uint32_t& dwProcessId, const uint32_t dwMaxCount, std::vector< std::pair<std::wstring, std::wstring> >& arrResult, void** pPayloadContext, size_t* szPayloadContextSize);
	extern bool GetProcessThreadList(const uint32_t& dwProcessId, const uint32_t& dwParentProcessId, std::vector<MHYPROT_THREAD_INFORMATION>& arrResult);
}