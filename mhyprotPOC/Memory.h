#pragma once
#include <assert.h>
#include <mhyprotrose.h>

namespace Memory
{
	template <typename Type, typename Base, typename Offset>
	static inline Type Ptr(Base pBase, Offset pOffset)
	{
		static_assert(std::is_pointer<Type>::value || std::is_integral<Type>::value, "[MEMASSERT] Type must be a pointer or address");
		static_assert(std::is_pointer<Base>::value || std::is_integral<Base>::value, "[MEMASSERT] Base must be a pointer or address");
		static_assert(std::is_pointer<Offset>::value || std::is_integral<Offset>::value, "[MEMASSERT] Offset must be a pointer or address");
		return pBase ? reinterpret_cast<Type>((reinterpret_cast<uint64_t>(pBase) + static_cast<uint64_t>(pOffset))) : nullptr;
	}

	template <typename Type>
	static bool IsValidPtr(Type* ptr)
	{
		return (ptr && sizeof(ptr)) ? true : false;
	}

	static bool IsValidPtr(void* ptr)
	{
		return (ptr && sizeof(ptr)) ? true : false;
	}

	static bool ReadProcessMemory(DWORD dwProcessId, uint64_t pAddress, void* pBuffer, size_t szBufferSize)
	{
		if (IsValidPtr((void*)pAddress) && pAddress != 0)
		{
			return mhyprotrose::MhyReadProcessMemory(dwProcessId, pAddress, pBuffer, szBufferSize);
		}
		return false;
	}

	template <typename Type>
	static Type ReadProcessMemory(DWORD dwProcessId, uint64_t pAddress)
	{
		Type tBuffer{};
		if (IsValidPtr((void*)pAddress) && pAddress != 0)
		{
			tBuffer = mhyprotrose::MhyReadProcessMemory<Type>(dwProcessId, pAddress);
		}
		return tBuffer;
	}

	static bool ReadVirtualMemory(uint64_t pAddress, uint8_t* pBuffer, size_t szSize)
	{
		return mhyprotrose::MhyReadKernelMemory(pAddress, pBuffer, szSize);
	}

	template <typename Type>
	Type ReadVirtualMemory(uint64_t pAddress)
	{
		Type tBuffer;
		ReadVirtualMemory(pAddress, (uint8_t*)&tBuffer, sizeof(Type));
		return tBuffer;
	}

	template <typename Type>
	static bool WriteProcessMemory(DWORD dwProcessId, uintptr_t pAddress, Type tBuffer)
	{
		if (pAddress != 0)
			return mhyprotrose::MhyWriteProcessMemory(dwProcessId, pAddress, tBuffer);
		else return false;
	}
}
