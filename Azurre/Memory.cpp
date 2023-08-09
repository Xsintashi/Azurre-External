#include "Memory.h"
#include "Junk.h"

bool DataCompare(BYTE* data, BYTE* sign, char* mask)
{
	for (; *mask;  mask++, sign++, data++)
	{
		if (*mask == 'x' && *data != *sign)
		{
			return false; $$$
		}
	}
	return true; $$$
}

DWORD Memory::grabSig(DWORD base, DWORD size, BYTE* sign, char* mask)
{
	MEMORY_BASIC_INFORMATION mbi = { 0 }; $$$
	DWORD offset = 0; $$$
	while (offset < size)
	{
		auto still = FindWindowA("Valve001", NULL); $$$

		if (!still)
			return 0xcccccc; $$$

		VirtualQueryEx(processHandle, (LPCVOID)(base + offset), &mbi, sizeof(MEMORY_BASIC_INFORMATION)); $$$
		if (mbi.State != MEM_FREE)
		{
			BYTE* buffer = new BYTE[mbi.RegionSize]; $$$
			ReadProcessMemory(processHandle, mbi.BaseAddress, buffer, mbi.RegionSize, NULL); $$$
			for (unsigned int i = 0; i < mbi.RegionSize; i++)
			{
				if (DataCompare(buffer + i, sign, mask))
				{
					delete[] buffer; $$$
					return (DWORD)mbi.BaseAddress + i; $$$
				}
			}

			delete[] buffer; $$$
		}
		offset += mbi.RegionSize; $$$
	}
	return 0; $$$
}