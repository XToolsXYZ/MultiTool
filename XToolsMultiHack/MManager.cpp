﻿/* Made by ZR2 Please Don't FUCK ME */

#include "MManager.h"

Memory::Memory() {}
Memory::~Memory() {}

bool Memory::Attach(char* pName, DWORD rights) 
{
	HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(entry);

	do 
	{
		if (!strcmp(entry.szExeFile, pName)) 
		{
			pID = entry.th32ProcessID;
			CloseHandle(handle);

			_process = OpenProcess(rights, false, pID);

			Attached = true;

			return true;
		}
	}
	while (Process32Next(handle, &entry));
	
	return false;
}

void Memory::Detach() 
{
	Attached = false;
	CloseHandle(_process);
}

Module Memory::GetModule(char* moduleName) 
{
	HANDLE module = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(mEntry);

	do 
	{
		if (!strcmp(mEntry.szModule, (LPSTR)moduleName)) 
		{
			CloseHandle(module);

			Module mod = { (DWORD)mEntry.hModule, mEntry.modBaseSize };
			return mod;
		}
	} 
	while (Module32Next(module, &mEntry));

	Module mod = { (DWORD)false, (DWORD)false };
	return mod;
}

HMODULE Memory::LoadModule(const char* moduleName) 
{
	HANDLE module = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(mEntry);

	do 
	{
		if (!strcmp(mEntry.szModule, (LPSTR)moduleName)) 
		{

			CloseHandle(module);

			char szPath[MAX_PATH] = { NULL };
			GetModuleFileNameEx(_process, mEntry.hModule, szPath, MAX_PATH);

			return LoadLibrary(szPath);
		}
	} 
	while (Module32Next(module, &mEntry));

	return NULL;
}

bool Memory::DataCompare(const BYTE* pData, const BYTE* pMask, const char* pszMask) 
{
	for (; *pszMask; ++pszMask, ++pData, ++pMask) 
	{
		if (*pszMask == 'x' && *pData != *pMask)
		{
			return false;
		}
	}

	return (*pszMask == NULL);
}

DWORD Memory::FindPattern(DWORD start, DWORD size, const char* sig, const char* mask) 
{
	BYTE* data = new BYTE[size];

	unsigned long bytesRead;

	if (!ReadProcessMemory(_process, (LPVOID)start, data, size, &bytesRead))
	{
		delete[] sig;
		delete[] data;
		return NULL;
	}

	for (DWORD i = 0; i < size; i++) 
	{
		if (DataCompare((const BYTE*)(data + i), (const BYTE*)sig, mask))
		{
			delete[] sig;
			delete[] data;
			return start + i;
		}
	}
	delete[] sig;
	delete[] data;
	return NULL;
}

DWORD Memory::FindPatternArr(DWORD start, DWORD size, const char* mask, int count, ...) 
{
	char* sig = new char[count + 1];
	va_list ap;
	va_start(ap, count);
	for (int i = 0; i < count; i++) 
	{
		char read = va_arg(ap, char);
		sig[i] = read;
	}
	va_end(ap);
	sig[count] = '\0';
	return FindPattern(start, size, sig, mask);
}

Vector Memory::NormalizeAngles(Vector angle) 
{
	if (!std::isfinite(angle.x)) {
		angle.x = 0;
	}
	if (!std::isfinite(angle.y)) {
		angle.y = 0;
	}
	while (angle.y < -180.0f) angle.y += 360.0f;
	while (angle.y > 180.0f) angle.y -= 360.0f;
	if (angle.x > 89.0f) angle.x = 89.0f;
	if (angle.x < -89.0f) angle.x = -89.0f;
	angle.z = 0;
	return angle;
}