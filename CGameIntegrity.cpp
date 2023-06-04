#include "CGameIntegrity.h"
#include "main.h"
#include "game/common.h"
#include "str_obfuscator_no_template.hpp"
#include <sys/mman.h>
#include <unistd.h>
#include <assert.h>
#include <dlfcn.h>
#include <dirent.h>

#define HOOK_PROC "\x01\xB4\x01\xB4\x01\x48\x01\x90\x01\xBD\x00\xBF\x00\x00\x00\x00"
const uint8_t g_szTrueBytes[5] = "\x09\x68\x49\x6C";

const cryptor::string_encryptor encLib = cryptor::create("libsamp.so", 11);

const cryptor::string_encryptor allowedLibs[] = { 
cryptor::create("libsamp.so", 11),
cryptor::create("libGTASA.so", 23),
cryptor::create("libImmEmulatorJ.so", 23),
cryptor::create("libSCAnd.so", 23),
cryptor::create("libcrashlytics.so", 23),
cryptor::create("libbass.so", 23)
};

extern char(*CStreaming__ConvertBufferToObject)(int, int, int);
extern char(*CFileMgr__ReadLine)(int, int, int);

std::atomic<bool> CGameIntegrity::m_bWorking;
std::thread* CGameIntegrity::m_pThreadUniqueLibrary;
std::thread* CGameIntegrity::m_pProcConvertBuffer;
std::thread* CGameIntegrity::m_pProcCheckLibraries;
uint8_t CGameIntegrity::m_aProtectedBytes[0x64];

ObfuscatedCalls* g_sGameIntegrityCalls = nullptr;


void CGameIntegrity::Initialise()
{
	//START_GAME_INTEGRITY1(1);
	//END_GAME_INTEGRITY1(1);

	static bool once = false;
	if (once)
	{
		return;
	}

	once = true;
	m_bWorking = true;

	g_sGameIntegrityCalls->HookCriticalFunctions();

	m_pThreadUniqueLibrary = new std::thread(ProcUniqueLibrary, nullptr);
	m_pThreadUniqueLibrary = new std::thread(ProcConvertBufferToObject, nullptr);
	m_pProcCheckLibraries = new std::thread(ProcCheckLibraries, nullptr);
}

void CGameIntegrity::Shutdown()
{
#ifdef _CDEBUG
	Log("Shutdowning game integrity module");
#endif
	m_bWorking = false;

	m_pProcConvertBuffer->join();
	m_pThreadUniqueLibrary->join();
	m_pProcCheckLibraries->join();

	delete m_pThreadUniqueLibrary;
	delete m_pProcConvertBuffer;
	delete m_pProcCheckLibraries;
#ifdef _CDEBUG
	Log("CGameIntegrity Shutdowned");
#endif
}

void CGameIntegrity::ProcUniqueLibrary(void*)
{
	const char* library = encLib.decrypt();
	while (m_bWorking)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		Dl_info info;
		if (dladdr(__builtin_return_address(0), &info) != 0)
		{
			void* current_library_addr = dlopen(info.dli_fname, RTLD_NOW);
			if (!current_library_addr)
			{
#ifdef _CDEBUG
				Log("LIBRARY CRASH");
#endif
				LIB_CRASH_OFFSET(11, 1);
				g_sGameIntegrityCalls->WriteMemory(g_libGTASA + 0x0029110E, (uintptr_t)"\x20\x25\x2D\x78", 4); // TRAP 3
			}

			//Log("%p | %p", current_library_addr, dlopen(library, RTLD_NOW));

			if (dlopen(library, RTLD_NOW) != current_library_addr)
			{
#ifdef _CDEBUG
				Log("LIBRARY CRASH");
#endif
				LIB_CRASH_OFFSET(12, 1);
				g_sGameIntegrityCalls->WriteMemory(g_libGTASA + 0x0029110E, (uintptr_t)"\x00\x25\x22\x78", 4);
			}
		}
#ifdef _CDEBUG
		Log("LIBRARY OK");
#endif
	}
}

#include "cryptors/GAME_INTEGRITY2_result.h"

void CGameIntegrity::ProcConvertBufferToObject(void*)
{
	PROTECT_CODE_GAME_INTEGRITY2;

	while (m_bWorking)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		//memcpy((void*)& CGameIntegrity::m_aProtectedBytes[0], (const void*)CStreaming__ConvertBufferToObject_hook, 0x64);
		bool bCond = false;
		for (int i = 0; i < 0x64 - 4; i++)
		{
			if (!memcmp(&m_aProtectedBytes[i], &g_szTrueBytes[0], 4))
			{
#ifdef _CDEBUG
				Log("CONVERT BUFFER OK");
				Log("orig %x %x %x %x", m_aProtectedBytes[i], m_aProtectedBytes[i + 1], m_aProtectedBytes[i + 2], m_aProtectedBytes[i + 3]);
				Log("true %x %x %x %x", g_szTrueBytes[0], g_szTrueBytes[1], g_szTrueBytes[2], g_szTrueBytes[3]);
#endif
				bCond = true;
				break;
			}
		}
		
		if (!bCond)
		{
#ifdef _CDEBUG
			Log("CRASH CONVERT BUFFER");
#endif
			LIB_CRASH_OFFSET(13, 1);
			g_sGameIntegrityCalls->WriteMemory(g_libGTASA + 0x0029110E, (uintptr_t)"\x00\x25\x2D\x78", 4);
		}
	}
}

#include "cryptors/GAME_INTEGRITY1_result.h"
bool IsPMHere();
bool DumpLibraries(std::vector<std::string>& buff);
void CGameIntegrity::ProcCheckLibraries(void*)
{
	PROTECT_CODE_GAME_INTEGRITY1;

	while (m_bWorking)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		if (IsPMHere())
		{
#ifdef _CDEBUG
			Log("CRASH FALSE LIBRARIES");
#endif
			LIB_CRASH_OFFSET(14, 1);
			g_sGameIntegrityCalls->WriteMemory(g_libGTASA + 0x0029110E, (uintptr_t)"\x00\x25\x2D\x78", 4);
		}

		std::vector<std::string> szLibs;
		if (!DumpLibraries(szLibs))
		{
#ifdef _CDEBUG
			Log("CRASH FALSE LIBRARIES");
#endif
			LIB_CRASH_OFFSET(14, 1);
			g_sGameIntegrityCalls->WriteMemory(g_libGTASA + 0x0029110E, (uintptr_t)"\x00\x25\x2D\x78", 4);
		}

		for (size_t i = 0; i < szLibs.size(); i++)
		{
			bool bCrash = true;
			for (size_t j = 0; j < sizeof(allowedLibs) / sizeof(cryptor::string_encryptor); j++)
			{
				if (!strcmp(szLibs[i].c_str(), allowedLibs[j].decrypt()))
				{
					bCrash = false;
				}
			}
			if (bCrash)
			{
#ifdef _CDEBUG
				Log("CRASH LIBRARIES");
#endif
				LIB_CRASH_OFFSET(15, 1);
				g_sGameIntegrityCalls->WriteMemory(g_libGTASA + 0x0029110E, (uintptr_t)"\x00\x25\x2D\x78", 4);
			}
		}

	}
}


void CGameIntegrity::HookCriticalFunctions()
{
#ifdef _CDEBUG
	Log("Hooking critical functions");
#endif
	g_sGameIntegrityCalls->InitHookStuff();

	//g_sGameIntegrityCalls->SetUpHook(g_libGTASA + 0x00291104, (uintptr_t)CStreaming__ConvertBufferToObject_hook, (uintptr_t*)& CStreaming__ConvertBufferToObject);

#ifdef _CDEBUG
	Log("Hooked critical functions");
#endif
}
void CGameIntegrity::CheckForRetnAddr(uintptr_t lr)
{
#ifdef _CDEBUG
	Log("Checking for retn addr");
#endif
	//if(true)
	if (lr != (g_libGTASA + 0x0029446C) && lr != (g_libGTASA + 0x29446d))
	{
#ifdef _CDEBUG
		Log("INVALID ADDR");
		Log("LR WAS: %x", lr - g_libGTASA);
#endif
		LIB_CRASH_OFFSET(16, 1);
		g_sGameIntegrityCalls->WriteMemory(g_libGTASA + 0x0029110E, (uintptr_t)"\x00\x25\x2D\x78", 4);
	}
#ifdef _CDEBUG
	Log("RETN OK!");
#endif
}

void __fillArray()
{
#ifdef _CDEBUG
	Log("fillArray");
#endif

	if (g_sGameIntegrityCalls)
	{
		return;
	}


	g_sGameIntegrityCalls = new ObfuscatedCalls();
	g_sGameIntegrityCalls->curCamera = nullptr;
	g_sGameIntegrityCalls->curWorld = (void*)(g_libGTASA + 0x180044);
	g_sGameIntegrityCalls->Initialise = CGameIntegrity::Initialise;
	g_sGameIntegrityCalls->Shutdown = CGameIntegrity::Shutdown;

	g_sGameIntegrityCalls->ProcUniqueLibrary = CGameIntegrity::ProcUniqueLibrary;
	g_sGameIntegrityCalls->lightFrame = 2;
	g_sGameIntegrityCalls->ProcConvertBufferToObject = CGameIntegrity::ProcConvertBufferToObject;

	g_sGameIntegrityCalls->HookCriticalFunctions = CGameIntegrity::HookCriticalFunctions;
	g_sGameIntegrityCalls->pad[0] = 1;
	g_sGameIntegrityCalls->UnFuck = CObfuscatedHook::UnFuck;
	g_sGameIntegrityCalls->WriteMemory = CObfuscatedHook::WriteMemory;
	g_sGameIntegrityCalls->ReadMemory = CObfuscatedHook::ReadMemory;
	g_sGameIntegrityCalls->InitHookStuff = CObfuscatedHook::InitHookStuff;
	g_sGameIntegrityCalls->JMPCode = CObfuscatedHook::JMPCode;
	g_sGameIntegrityCalls->WriteHookProc = CObfuscatedHook::WriteHookProc;
	g_sGameIntegrityCalls->SetUpHook = CObfuscatedHook::SetUpHook;

	g_sGameIntegrityCalls->ProcCheckLibraries = CGameIntegrity::ProcCheckLibraries;
	g_sGameIntegrityCalls->CheckForRetnAddr = CGameIntegrity::CheckForRetnAddr;
}


uintptr_t CObfuscatedHook::mmap_start = 0;
uintptr_t CObfuscatedHook::mmap_end = 0;
uintptr_t CObfuscatedHook::memlib_start = 0;
uintptr_t CObfuscatedHook::memlib_end = 0;

void CObfuscatedHook::UnFuck(uintptr_t ptr)
{
	mprotect((void*)(ptr & 0xFFFFF000), PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC);
}

void CObfuscatedHook::WriteMemory(uintptr_t dest, uintptr_t src, size_t size)
{
	g_sGameIntegrityCalls->UnFuck(dest);
	memcpy((void*)dest, (void*)src, size);
	cacheflush(dest, dest + size, 0);
}

void CObfuscatedHook::ReadMemory(uintptr_t dest, uintptr_t src, size_t size)
{
	g_sGameIntegrityCalls->UnFuck(src);
	memcpy((void*)dest, (void*)src, size);
}

void CObfuscatedHook::InitHookStuff()
{
	memlib_start = g_libGTASA + 0x180044;
	memlib_end = memlib_start + 0x50;

	mmap_start = (uintptr_t)mmap(0, PAGE_SIZE, PROT_WRITE | PROT_READ | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	mprotect((void*)(mmap_start & 0xFFFFF000), PAGE_SIZE, PROT_READ | PROT_EXEC | PROT_WRITE);
	mmap_end = (mmap_start + PAGE_SIZE);
}

void CObfuscatedHook::JMPCode(uintptr_t func, uintptr_t addr)
{
	uint32_t code = ((addr - func - 4) >> 12) & 0x7FF | 0xF000 | ((((addr - func - 4) >> 1) & 0x7FF | 0xB800) << 16);
	g_sGameIntegrityCalls->WriteMemory(func, (uintptr_t)& code, 4);
}

void CObfuscatedHook::WriteHookProc(uintptr_t addr, uintptr_t func)
{
	char code[16];
	memcpy(code, HOOK_PROC, 16);
	*(uint32_t*)& code[12] = (func | 1);
	g_sGameIntegrityCalls->WriteMemory(addr, (uintptr_t)code, 16);
}

void CObfuscatedHook::SetUpHook(uintptr_t addr, uintptr_t func, uintptr_t* orig)
{
	if (memlib_end < (memlib_start + 0x10) || mmap_end < (mmap_start + 0x20))
	{
		return;
	}

	g_sGameIntegrityCalls->ReadMemory(mmap_start, addr, 4);
	g_sGameIntegrityCalls->WriteHookProc(mmap_start + 4, addr + 4);
	*orig = mmap_start + 1;
	mmap_start += 32;

	g_sGameIntegrityCalls->JMPCode(addr, memlib_start);
	g_sGameIntegrityCalls->WriteHookProc(memlib_start, func);
	memlib_start += 16;
}