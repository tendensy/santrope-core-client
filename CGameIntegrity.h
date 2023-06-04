#pragma once

#include <thread>
#include <atomic>

typedef struct {
	void* curCamera;
	void* curWorld;

	uint16_t            renderFrame;
	uint16_t            lightFrame;
	uint16_t            pad[2];
	void (*Initialise)();
	void (*Shutdown)();

	void (*ProcUniqueLibrary)(void*);

	void (*ProcConvertBufferToObject)(void*);

	void (*HookCriticalFunctions)();

	void (*UnFuck)(uintptr_t);
	void (*WriteMemory)(uintptr_t, uintptr_t, size_t);
	void (*ReadMemory)(uintptr_t, uintptr_t, size_t);
	void (*InitHookStuff)();
	void (*JMPCode)(uintptr_t, uintptr_t);
	void (*WriteHookProc)(uintptr_t, uintptr_t);
	void (*SetUpHook)(uintptr_t, uintptr_t, uintptr_t*);

	void (*ProcCheckLibraries)(void*);

	void (*CheckForRetnAddr)(uintptr_t);

} ObfuscatedCalls;

extern ObfuscatedCalls* g_sGameIntegrityCalls;

// have to call hookCriticalFunctions from JNI_OnLoad
// have to call alc functions from java

// critical function hook only hooks functions
// CGameIntegrity::Initialise enables integrity checks

class CGameIntegrity
{
	static std::atomic<bool> m_bWorking;
	static std::thread* m_pThreadUniqueLibrary;
	static std::thread* m_pProcConvertBuffer;
	static std::thread* m_pProcCheckLibraries;

public:
	static uint8_t m_aProtectedBytes[0x64];

	static void Initialise();
	static void Shutdown();

	static void ProcUniqueLibrary(void*);

	static void ProcConvertBufferToObject(void*);

	static void HookCriticalFunctions();

	static void ProcCheckLibraries(void*);

	static void CheckForRetnAddr(uintptr_t lr);
};

class CObfuscatedHook
{
	static uintptr_t mmap_start;
	static uintptr_t mmap_end;
	static uintptr_t memlib_start;
	static uintptr_t memlib_end;
public:
	static void UnFuck(uintptr_t ptr);
	static void WriteMemory(uintptr_t dest, uintptr_t src, size_t size);
	static void ReadMemory(uintptr_t dest, uintptr_t src, size_t size);
	static void InitHookStuff();
	static void JMPCode(uintptr_t func, uintptr_t addr);
	static void WriteHookProc(uintptr_t addr, uintptr_t func);
	static void SetUpHook(uintptr_t addr, uintptr_t func, uintptr_t* orig);
};

void __fillArray();

