#include "Utilities.h"

bool Driver::CreateFileFromMemory(const std::string& strDesiredFilePath, const char* chAddress, size_t szSize)
{
	std::ofstream file_ofstream(strDesiredFilePath.c_str(), std::ios_base::out | std::ios_base::binary);
	if (!file_ofstream.write(chAddress, szSize))
	{
		file_ofstream.close();
		return false;
	}
	file_ofstream.close();
	return true;
}
void Utilities::GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	horizontal = desktop.right;
	vertical = desktop.bottom;
}


bool Driver::DeleteDriverFile(const char* chDriverDirectory)
{
	return DeleteFileA(chDriverDirectory);
}

bool Driver::CreateAndStartDriverService(const char* chServiceName, const char* chDriverDirectory)
{
	SC_HANDLE hSCM = OpenSCManager(nullptr, nullptr, SC_MANAGER_CREATE_SERVICE);
	if (hSCM)
	{
		SC_HANDLE hService = CreateServiceA(hSCM, chServiceName, chServiceName, SERVICE_START | SERVICE_STOP | DELETE, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_IGNORE, chDriverDirectory, nullptr, nullptr, nullptr, nullptr, nullptr);
		if (!hService)
		{
			hService = OpenServiceA(hSCM, chServiceName, SERVICE_START);
			if (!hService)
			{
				CloseServiceHandle(hService);
				return false;
			}
		}
		bool bResult = StartService(hService, 0, nullptr);
		CloseServiceHandle(hSCM);
		CloseServiceHandle(hService);
		return bResult;
	}
	return false;
}

bool Driver::StopAndDeleteDriverService(const char* chServiceName, const char* chDriverDirectory)
{
	SC_HANDLE hSCM = OpenSCManager(nullptr, nullptr, SC_MANAGER_CREATE_SERVICE);
	if (!hSCM)
	{
		return false;
	}
	SC_HANDLE hService = OpenServiceA(hSCM, chServiceName, SERVICE_STOP | DELETE);
	if (!hService)
	{
		CloseServiceHandle(hSCM);
		return false;
	}
	SERVICE_STATUS sStatus = { 0 };
	bool bResult = ControlService(hService, SERVICE_CONTROL_STOP, &sStatus) && DeleteService(hService);
	CloseServiceHandle(hService);
	CloseServiceHandle(hSCM);
	Sleep(2000);
	return DeleteDriverFile(chDriverDirectory) && bResult;
}

bool Driver::ServiceIsRunning(const char* chServiceName)
{
	SC_HANDLE hSCM = OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
	if (hSCM)
	{
		SC_HANDLE hService = OpenServiceA(hSCM, chServiceName, SERVICE_ALL_ACCESS);
		if (hService)
		{
			CloseServiceHandle(hSCM);
			CloseServiceHandle(hService);
			return true;
		}
	}
	return false;
}

void Driver::StopAndDeleteAllDrivers()
{
	if (Driver::ServiceIsRunning(Driver::chMhyprotDriverServiceName))
	{
		Driver::StopAndDeleteDriverService(Driver::chMhyprotDriverServiceName, Driver::chMhyprotDriverDirectory);
	}
	if (Driver::ServiceIsRunning(Driver::chGACDriverServiceName))
	{
		Driver::StopAndDeleteDriverService(Driver::chGACDriverServiceName, Driver::chGACDriverDirectory);
	}
}

PCHAR Utilities::MD5(PCHAR chText)
{
	static char chResult[33];
	RtlZeroMemory(chResult, sizeof(chResult));
	md5_state_t md5State;
	md5_byte_t md5Sum[16];
	md5_init(&md5State);
	md5_append(&md5State, (const md5_byte_t*)chText, strlen(chText));
	md5_finish(&md5State, md5Sum);
	httpMD5String(md5Sum, chResult);
	return chResult;
}

string Utilities::StrReplace(std::string str, const std::string& from, const std::string& to)
{
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
	{
		return str;
	}
	str.replace(start_pos, from.length(), to);
	return str;
}

bool Utilities::ReadFileToMemory(const std::string& strFilePath, std::vector<uint8_t>* pOutBuffer)
{
	std::ifstream file_ifstream(strFilePath, std::ios::binary);
	if (!file_ifstream)
	{
		return false;
	}
	pOutBuffer->assign((std::istreambuf_iterator<char>(file_ifstream)), std::istreambuf_iterator<char>());
	file_ifstream.close();
	return true;
}

bool Utilities::ConvertFileToByteArray(const char* chFileName, const char* chFileDestination)
{
	vector<uint8_t> raw_image = { 0 };
	if (ReadFileToMemory(chFileName, &raw_image))
	{
		ofstream myfile;
		myfile.open(chFileDestination);
		for (int i = 0; i < +raw_image.size(); i++)
		{
			myfile << "0x" << hex << uppercase << +raw_image[i] << ", ";
		}
		return true;
		myfile.close();
	}
	else
	{
		return false;
	}
}

bool Utilities::UpdateEPROCESSOffsets()
{
	OSVERSIONINFOEXW osVersionInfo = Utilities::GetOSVersionInfo();
	if (osVersionInfo.dwBuildNumber == 10240)
	{
		UtilVars.pImageFileNameOffset = 0x448;
		UtilVars.pUniqueProcessIdOffset = 0x2E8;
		UtilVars.pSectionBaseOffset = 0x3C0;
		UtilVars.pActiveProcessLinkOffset = 0x2F0;
		UtilVars.pPEBOffset = 0x3F8;
		return true;
	}
	else if (osVersionInfo.dwBuildNumber == 10586)
	{
		UtilVars.pImageFileNameOffset = 0x450;
		UtilVars.pUniqueProcessIdOffset = 0x2E8;
		UtilVars.pSectionBaseOffset = 0x3C0;
		UtilVars.pActiveProcessLinkOffset = 0x2F0;
		UtilVars.pPEBOffset = 0x3F8;
		return true;
	}
	else if (osVersionInfo.dwBuildNumber == 14393)
	{
		UtilVars.pImageFileNameOffset = 0x450;
		UtilVars.pUniqueProcessIdOffset = 0x2E8;
		UtilVars.pSectionBaseOffset = 0x3C0;
		UtilVars.pActiveProcessLinkOffset = 0x2F0;
		UtilVars.pPEBOffset = 0x3F8;
		return true;
	}
	else if (osVersionInfo.dwBuildNumber == 15063)
	{
		UtilVars.pImageFileNameOffset = 0x450;
		UtilVars.pUniqueProcessIdOffset = 0x2E0;
		UtilVars.pSectionBaseOffset = 0x3C0;
		UtilVars.pActiveProcessLinkOffset = 0x2E8;
		UtilVars.pPEBOffset = 0x3F8;
		return true;
	}
	else if (osVersionInfo.dwBuildNumber == 16299)
	{
		UtilVars.pImageFileNameOffset = 0x450;
		UtilVars.pUniqueProcessIdOffset = 0x2E0;
		UtilVars.pSectionBaseOffset = 0x3C0;
		UtilVars.pActiveProcessLinkOffset = 0x2E8;
		UtilVars.pPEBOffset = 0x3F8;
		return true;
	}
	else if (osVersionInfo.dwBuildNumber == 17134)
	{
		UtilVars.pImageFileNameOffset = 0x450;
		UtilVars.pUniqueProcessIdOffset = 0x2E0;
		UtilVars.pSectionBaseOffset = 0x3C0;
		UtilVars.pActiveProcessLinkOffset = 0x2E8;
		UtilVars.pPEBOffset = 0x3F8;
		return true;
	}
	else if (osVersionInfo.dwBuildNumber == 17763)
	{
		UtilVars.pImageFileNameOffset = 0x450;
		UtilVars.pUniqueProcessIdOffset = 0x2E0;
		UtilVars.pSectionBaseOffset = 0x3C0;
		UtilVars.pActiveProcessLinkOffset = 0x2E8;
		UtilVars.pPEBOffset = 0x3F8;
		return true;
	}
	else if (osVersionInfo.dwBuildNumber == 18362)
	{
		UtilVars.pImageFileNameOffset = 0x450;
		UtilVars.pUniqueProcessIdOffset = 0x2E8;
		UtilVars.pSectionBaseOffset = 0x3C8;
		UtilVars.pActiveProcessLinkOffset = 0x2F0;
		UtilVars.pPEBOffset = 0x3F8;
		return true;
	}
	else if (osVersionInfo.dwBuildNumber == 18363)
	{
		UtilVars.pImageFileNameOffset = 0x450;
		UtilVars.pUniqueProcessIdOffset = 0x2E8;
		UtilVars.pSectionBaseOffset = 0x3C8;
		UtilVars.pActiveProcessLinkOffset = 0x2F0;
		UtilVars.pPEBOffset = 0x3F8;
		return true;
	}
	else if (osVersionInfo.dwBuildNumber == 19041)
	{
		UtilVars.pImageFileNameOffset = 0x5A8;
		UtilVars.pUniqueProcessIdOffset = 0x440;
		UtilVars.pSectionBaseOffset = 0x520;
		UtilVars.pActiveProcessLinkOffset = 0x448;
		UtilVars.pPEBOffset = 0x550;
		return true;
	}
	else if (osVersionInfo.dwBuildNumber == 19042)
	{
		UtilVars.pImageFileNameOffset = 0x5A8;
		UtilVars.pUniqueProcessIdOffset = 0x440;
		UtilVars.pSectionBaseOffset = 0x520;
		UtilVars.pActiveProcessLinkOffset = 0x448;
		UtilVars.pPEBOffset = 0x550;
		return true;
	}
	else if (osVersionInfo.dwBuildNumber == 19043)
	{
		UtilVars.pImageFileNameOffset = 0x5A8;
		UtilVars.pUniqueProcessIdOffset = 0x440;
		UtilVars.pSectionBaseOffset = 0x520;
		UtilVars.pActiveProcessLinkOffset = 0x448;
		UtilVars.pPEBOffset = 0x550;
		return true;
	}
	return false;
}

uintptr_t Utilities::TranslateLinearAddress(uint64_t pDirectoryTableBaseAddress, uint64_t pVirtualAddress)
{
	uint16_t PML4 = (uint16_t)((pVirtualAddress >> 39) & 0x1FF);
	uint16_t pDirectoryPtr = (uint16_t)((pVirtualAddress >> 30) & 0x1FF);
	uint16_t pDirectory = (uint16_t)((pVirtualAddress >> 21) & 0x1FF);
	uint16_t pTable = (uint16_t)((pVirtualAddress >> 12) & 0x1FF);
	uint64_t PML4E = 0;
	Memory::ReadVirtualMemory(pDirectoryTableBaseAddress + (uint64_t)PML4 * sizeof(uint64_t), (uint8_t*)&PML4E, sizeof(PML4E));
	if (PML4E)
	{
		uint64_t PDPTE = 0;
		Memory::ReadVirtualMemory((PML4E & 0xFFFF1FFFFFF000) + (uint64_t)pDirectoryPtr * sizeof(uint64_t), (uint8_t*)&PDPTE, sizeof(PDPTE));
		if (PDPTE)
		{
			if ((PDPTE & (1 << 7)) != 0)
			{
				return (PDPTE & 0xFFFFFC0000000) + (pVirtualAddress & 0x3FFFFFFF);
			}
			uint64_t PDE = 0;
			Memory::ReadVirtualMemory((PDPTE & 0xFFFFFFFFFF000) + (uint64_t)pDirectory * sizeof(uint64_t), (uint8_t*)&PDE, sizeof(PDE));
			if (PDE)
			{
				if ((PDE & (1 << 7)) != 0)
				{
					return (PDE & 0xFFFFFFFE00000) + (pVirtualAddress & 0x1FFFFF);
				}
				uint64_t PTE = 0;
				Memory::ReadVirtualMemory((PDE & 0xFFFFFFFFFF000) + (uint64_t)pTable * sizeof(uint64_t), (uint8_t*)&PTE, sizeof(PTE));
				if (PTE)
				{
					return (PTE & 0xFFFFFFFFFF000) + (pVirtualAddress & 0xFFF);
				}
			}
		}
	}
	return (uintptr_t)0;
}

uintptr_t Utilities::SFGetEPROCESS(int dwProcessId)
{
	auto pHandleInfo = QueryInfo<SYSTEM_HANDLE_INFORMATION>(SystemHandleInformation);
	if (pHandleInfo.get())
	{
		for (size_t i = 0; i < pHandleInfo->HandleCount; i++)
		{
			if (dwProcessId == pHandleInfo->Handles[i].ProcessId && 7 == pHandleInfo->Handles[i].ObjectTypeNumber)
			{
				return reinterpret_cast<size_t>(pHandleInfo->Handles[i].Object);
			}
		}
	}
	return 0;
}

uintptr_t Utilities::GetEPROCESS(int dwProcessId)
{
	_LIST_ENTRY pActiveProcessLinks;
	Memory::ReadVirtualMemory(SFGetEPROCESS(4) + UtilVars.pActiveProcessLinkOffset, (uint8_t*)&pActiveProcessLinks, sizeof(pActiveProcessLinks));
	if (&pActiveProcessLinks)
	{
		while (true)
		{
			uint64_t pNextPid = 0;
			uint64_t pNextLink = (uint64_t)(pActiveProcessLinks.Flink);
			uint64_t pNext = pNextLink - UtilVars.pActiveProcessLinkOffset;
			if (pNext)
			{
				Memory::ReadVirtualMemory(pNext + UtilVars.pUniqueProcessIdOffset, (uint8_t*)&pNextPid, sizeof(pNextPid));
				Memory::ReadVirtualMemory(pNext + UtilVars.pActiveProcessLinkOffset, (uint8_t*)&pActiveProcessLinks, sizeof(pActiveProcessLinks));
				if (pNextPid == dwProcessId)
				{
					return pNext;
				}
				if (pNextPid == 4 || pNextPid == 0)
				{
					break;
				}
			}
		}
	}
	return 0;
}

uintptr_t Utilities::GetKernelModuleAddress(const std::string& strModuleName)
{
	void* pBuffer = nullptr;
	DWORD dwBufferSize = 0;
	NTSTATUS ntStatus = NtQuerySystemInformation(static_cast<SYSTEM_INFORMATION_CLASS>(SystemModuleInformation), pBuffer, dwBufferSize, &dwBufferSize);
	while (ntStatus == STATUS_INFO_LENGTH_MISMATCH)
	{
		VirtualFree(pBuffer, 0, MEM_RELEASE);
		pBuffer = VirtualAlloc(nullptr, dwBufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		ntStatus = NtQuerySystemInformation(static_cast<SYSTEM_INFORMATION_CLASS>(SystemModuleInformation), pBuffer, dwBufferSize, &dwBufferSize);
	}
	if (!NT_SUCCESS(ntStatus))
	{
		VirtualFree(pBuffer, 0, MEM_RELEASE);
		return 0;
	}
	PRTL_PROCESS_MODULES pModules = static_cast<PRTL_PROCESS_MODULES>(pBuffer);
	for (auto i = 0u; i < pModules->NumberOfModules; ++i)
	{
		const std::string strCurrentModuleName = std::string(reinterpret_cast<char*>(pModules->Modules[i].FullPathName) + pModules->Modules[i].OffsetToFileName);
		if (!_stricmp(strCurrentModuleName.c_str(), strModuleName.c_str()))
		{
			const uintptr_t pResult = reinterpret_cast<uint64_t>(pModules->Modules[i].ImageBase);
			VirtualFree(pBuffer, 0, MEM_RELEASE);
			return pResult;
		}
	}
	VirtualFree(pBuffer, 0, MEM_RELEASE);
	return 0;
}

uintptr_t Utilities::GetProcessBaseAddress(int dwProcessId)
{
	if (dwProcessId == 0) 
	{
		dwProcessId = GetKernelModuleAddress("ntoskrnl.exe");
	}
	uintptr_t pEPROCESS = GetEPROCESS(dwProcessId);
	uintptr_t pBaseAddress = 0;
	if (pEPROCESS)
	{
		Memory::ReadVirtualMemory(pEPROCESS + UtilVars.pSectionBaseOffset, (uint8_t*)&pBaseAddress, sizeof(pBaseAddress));
		return pBaseAddress;
	}
	return 0;
}

DWORD Utilities::GetProcessIdByName(const wchar_t* wcProcessName)
{
	PROCESSENTRY32 processEntry32;
	processEntry32.dwSize = sizeof(PROCESSENTRY32);
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Process32First(hSnapshot, &processEntry32) == TRUE)
	{
		while (Process32Next(hSnapshot, &processEntry32) == TRUE)
		{
			if (wcscmp(processEntry32.szExeFile, wcProcessName) == 0)
			{
				return processEntry32.th32ProcessID;
			}
		}
	}
	CloseHandle(hSnapshot);
	return 0;
}

DWORD Utilities::GetProcessIdByContainingString(const wchar_t* wcProcessName)
{
	PROCESSENTRY32 processEntry32;
	processEntry32.dwSize = sizeof(PROCESSENTRY32);
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Process32First(hSnapshot, &processEntry32) == TRUE)
	{
		while (Process32Next(hSnapshot, &processEntry32) == TRUE)
		{
			if (wcsstr(processEntry32.szExeFile, wcProcessName) != 0)
			{
				return processEntry32.th32ProcessID;
			}
		}
	}
	CloseHandle(hSnapshot);
	return 0;
}

bool Utilities::IsProcessBeingDebugged64(int dwProcessId)
{
	uintptr_t pPEB = Memory::ReadVirtualMemory<uintptr_t>(GetEPROCESS(dwProcessId) + UtilVars.pPEBOffset);
	if (pPEB)
	{
		UCHAR ucBeingDebugged = Memory::ReadProcessMemory<UCHAR>(dwProcessId, pPEB + 0x2);
		return (ucBeingDebugged == (UCHAR)1) || (IsDebuggerPresent());
	}
	return false;
}

ULONG Utilities::GetModuleBaseAddress32(int dwProcessId, const wchar_t* wcModuleName, DWORD* pdwSize, DWORD dwMaxCount)
{
	/*
	* TODO : 
	* Fix the unlimited loop using actual method instead of cringy max count
	* Refactor
	*/
	uintptr_t pPEB = Memory::ReadVirtualMemory<uintptr_t>(GetEPROCESS(dwProcessId) + UtilVars.pPEBOffset);
	if (pPEB)
	{
		uintptr_t pPEB32Ptr = pPEB + 0x1000;
		PEB32 pPEB32 = Memory::ReadProcessMemory<PEB32>(dwProcessId, pPEB32Ptr);
		if (&pPEB32)
		{
			PEB_LDR_DATA32 pPEBLDRData32 = Memory::ReadProcessMemory<PEB_LDR_DATA32>(dwProcessId, pPEB32.Ldr);
			if (&pPEBLDRData32)
			{
				ULONG pListHead = pPEB32.Ldr + 0x10;
				UCHAR ucImageName[8000];
				static int dwCount = 0;
				for (ULONG pCurrentList = pPEBLDRData32.InLoadOrderModuleList.Flink; pCurrentList != pListHead;)
				{
					LDR_DATA_TABLE_ENTRY32 pEntryData = Memory::ReadProcessMemory<LDR_DATA_TABLE_ENTRY32>(dwProcessId, pCurrentList);
					if (&pEntryData)
					{
						ULONG ulImageNamePtr = Memory::ReadProcessMemory<ULONG>(dwProcessId, pCurrentList + 0x30);
						Memory::ReadProcessMemory(dwProcessId, ulImageNamePtr, &ucImageName, 128);
						wchar_t* wcImageName = reinterpret_cast<wchar_t*>(ucImageName);
						if (wcscmp(wcImageName, wcModuleName) == 0)
						{
							if (pdwSize != nullptr)
								*pdwSize = (DWORD)pEntryData.SizeOfImage;
							return pEntryData.DllBase;
						}
						pCurrentList = (ULONG)pEntryData.InLoadOrderLinks.Flink;
						dwCount += 1;
						if (dwCount >= dwMaxCount)
							break;
					}
				}
			}
		}
	}
	return 0;
}

uintptr_t Utilities::GetModuleBaseAddress64(int dwProcessId, const wchar_t* wcModuleName, DWORD* pdwSize)
{
	/*
	* TODO :
	* Read the actual struct instead of pointer
	* Refactor
	*/
	uintptr_t pPEB = Memory::ReadVirtualMemory<uintptr_t>(GetEPROCESS(dwProcessId) + UtilVars.pPEBOffset);
	if (pPEB)
	{
		uintptr_t pLDRData = Memory::ReadProcessMemory<uintptr_t>(dwProcessId, pPEB + 0x18);
		if (pLDRData)
		{
			uintptr_t pListHead = pLDRData + 0x10;
			uintptr_t InLoadOrderModuleList = Memory::ReadProcessMemory<uintptr_t>(dwProcessId, pLDRData + 0x10);
			if (InLoadOrderModuleList)
			{
				UCHAR ucImageName[8000];
				static int dwCount = 0;
				for (uintptr_t pCurrentList = InLoadOrderModuleList; pCurrentList != pListHead;)
				{
					LDR_DATA_TABLE_ENTRY pEntryData = Memory::ReadProcessMemory<LDR_DATA_TABLE_ENTRY>(dwProcessId, pCurrentList);
					if (&pEntryData)
					{
						uintptr_t pImageNamePtr = Memory::ReadProcessMemory<uintptr_t>(dwProcessId, pCurrentList + 0x60);
						if (pImageNamePtr)
						{
							Memory::ReadProcessMemory(dwProcessId, pImageNamePtr, &ucImageName, 128);
							wchar_t* wcImageName = reinterpret_cast<wchar_t*>(ucImageName);
							if (wcscmp(wcImageName, wcModuleName) == 0)
							{
								if (pdwSize != nullptr)
									*pdwSize = (DWORD)pEntryData.SizeOfImage;
								return (uintptr_t)pEntryData.DllBase;
							}
							pCurrentList = (uintptr_t)pEntryData.InLoadOrderLinks.Flink;
							dwCount += 1;
						}
					}
				}
			}
		}
	}
	return 0;
}

uintptr_t Utilities::SearchSignature(DWORD dwProcessId, uintptr_t pStartAddress, SIZE_T szLength, BYTE* bPattern, const char* chMask)
{
	SIZE_T szPos = 0;
	auto maskLength = strlen(chMask) - 1;
	auto startAdress = pStartAddress;
	for (auto i = startAdress; i < startAdress + szLength; ++i)
	{
		if (Memory::ReadProcessMemory<unsigned char>(dwProcessId, i) == bPattern[szPos] || chMask[szPos] == '?')
		{
			if (chMask[szPos + 1] == '\0')
			{
				return i - maskLength;
			}
			szPos++;
		}
		else
		{
			szPos = 0;
		}
	}
	return 0;
}

uintptr_t Utilities::FindPattern(DWORD dwProcessId, uintptr_t pModuleAddress, SIZE_T szModuleSize, BYTE* bPattern, const char* chMask)
{
	return SearchSignature(dwProcessId, pModuleAddress, szModuleSize, bPattern, chMask);
}

uintptr_t Utilities::ExtractOffsetByPattern(DWORD dwProcessId, uintptr_t pModuleAddress, SIZE_T szModuleSize, ULONG ulAdditionalOffset, ULONG ulExtra, const char* chPattern, const char* chMask, BOOLEAN bRelative)
{
	bool bStatus = true;
	uintptr_t pResult = FindPattern(dwProcessId, pModuleAddress, szModuleSize, (BYTE*)chPattern, chMask);
	if (ulAdditionalOffset != NULL && pResult)
	{
		bStatus = Memory::ReadProcessMemory(dwProcessId, (uint64_t)(pResult + ulAdditionalOffset), &pResult, sizeof(pResult));
	}
	if (bStatus && pResult)
	{
		pResult += ulExtra;
		if (bRelative)
		{
			pResult -= pModuleAddress;
		}
		return pResult;
	}
	else
	{
		return 0;
	}
}

OSVERSIONINFOEXW Utilities::GetOSVersionInfo()
{
	NTSTATUS(WINAPI * RtlGetVersion)(LPOSVERSIONINFOEXW);
	OSVERSIONINFOEXW osInfo;
	*(FARPROC*)&RtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");
	if (RtlGetVersion != NULL)
	{
		osInfo.dwOSVersionInfoSize = sizeof(osInfo);
		RtlGetVersion(&osInfo);
		return osInfo;
	}
}