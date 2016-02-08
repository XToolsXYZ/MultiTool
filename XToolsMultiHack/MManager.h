/* Made by ZR2 Please Don't FUCK ME */

#ifndef MMANAGER_H
#define MMANAGER_H

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <TlHelp32.h>
#include <string>
#include <Psapi.h>
#include <stdarg.h>
#include <stdint.h>

struct GlowObjectPlayer {
	DWORD pEntity;
	float r;
	float g;
	float b;
	float a;
	uint8_t unk1[16];
	bool RenderWhenOccluded;
	bool RenderWhenUnoccluded;
	bool FullBloom;
	uint8_t unk2[14];
};

struct Vector {
	float x, y, z;
};

struct Module {
	DWORD dwBase;
	DWORD dwSize;
};

class Memory {
public:
	Memory();
	~Memory();

	bool Attached;
	bool Attach(char* pName, DWORD rights = PROCESS_ALL_ACCESS);
	void Detach();

	Module GetModule(char* moduleName);
	HMODULE LoadModule(const char* moduleName);
	Vector NormalizeAngles(Vector angle);

	template <class T>
	T Read(DWORD addr) {
		T _read;
		ReadProcessMemory(_process, (LPVOID)addr, &_read, sizeof(T), NULL);
		return _read;
	}
	template <class T>
	void Write(DWORD addr, T val) {
		WriteProcessMemory(_process, (LPVOID)addr, &val, sizeof(T), NULL);
	}

	DWORD FindPattern(DWORD start, DWORD size, const char* sig, const char* mask);
	DWORD FindPatternArr(DWORD start, DWORD size, const char* mask, int count, ...);

private:
	HANDLE _process;
	DWORD pID;
	bool DataCompare(const BYTE* pData, const BYTE* pMask, const char* pszMask);
};
#endif