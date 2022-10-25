#pragma once
#include <Windows.h>
#include <iostream>
#include <Psapi.h>
#include <TlHelp32.h>
enum DRV_CTLCODE
{
	CODE_WRITE_MEMORY_Physical = 1,
	CODE_READ_MEMORY_Physical,
	CODE_WRITE_MEMORY,
	CODE_READ_MEMORY,
	CODE_GET_MODULE,
	CODE_AllocUserMemory,
	CODE_FreeUserMemory,
	CODE_CreateUserThread,
	CODE_GET_VERSION
};

typedef struct _INFO_STRUCT_
{
	ULONG64 ProcessID;
	ULONG64 Address;
	ULONG64 buffer;
	ULONG64 Length;
	_INFO_STRUCT_(ULONG64 pid, ULONG64 addr, ULONG64 buf, ULONG64 len) :ProcessID(pid), Address(addr), buffer(buf), Length(len) {}
}INFO_STRUCT, * PINFO_STRUCT;

typedef struct _MODULE_INFO_ {
	ULONG64 StartBase;
	LPCWSTR Name;
}MODULE_INFO, * PMODULE_INFO;

class Driver
{
public:
	Driver() {}
	~Driver() {}

	bool InitializeInterface() {
		LoadLibraryA("user32.dll");
		LoadLibraryA("win32u.dll");
		auto base = GetModuleHandleA("win32u.dll");
		void* hooked_fn = GetProcAddress(base, "NtQueryCompositionSurfaceStatistics");
		syscall = reinterpret_cast<decltype(syscall)>(hooked_fn);
		return true;
	}

	void ReadRaw(uint64_t ProcessId, uint64_t Address, void* Buffer, size_t len) {
		INFO_STRUCT buffer = INFO_STRUCT(ProcessId, Address, (ULONG64)Buffer, len);
		syscall(&buffer, CODE_READ_MEMORY);
	}

	void WriteRaw(uint64_t ProcessId, uint64_t Address, void* Buffer, size_t len) {
		INFO_STRUCT buffer = INFO_STRUCT(ProcessId, Address, (ULONG64)Buffer, len);
		syscall(&buffer, CODE_WRITE_MEMORY);
	}

	template<typename T>
	T Read(uint64_t ProcessId, uint64_t Address) {
		T ret;
		INFO_STRUCT buffer = INFO_STRUCT(ProcessId, Address, (ULONG64)&ret, sizeof(T));
		syscall(&buffer, CODE_READ_MEMORY_Physical);
		return ret;
	}

	template<typename T>
	void Read(uint64_t ProcessId, uint64_t Address, T* Buffer) {
		INFO_STRUCT buffer = INFO_STRUCT(ProcessId, Address, (ULONG64)Buffer, sizeof(T));
		syscall(&buffer, CODE_READ_MEMORY_Physical);
	}

	template<typename T>
	VOID Write(uint64_t ProcessId, uint64_t Address, T& Value) {
		INFO_STRUCT buffer = INFO_STRUCT(ProcessId, Address, ULONG64(&Value), sizeof(T));
		syscall(&buffer, CODE_WRITE_MEMORY_Physical);
	}

	template<typename T>
	T Read_fast(uint64_t ProcessId, uint64_t Address) {
		T ret;
		INFO_STRUCT buffer = INFO_STRUCT(ProcessId, Address, (ULONG64)&ret, sizeof(T));
		syscall(&buffer, CODE_READ_MEMORY);
		return ret;
	}

	template<typename T>
	VOID Write_fast(uint64_t ProcessId, uint64_t Address, T Value) {
		INFO_STRUCT buffer = INFO_STRUCT(ProcessId, Address, ULONG64(&Value), sizeof(T));
		syscall(&buffer, CODE_WRITE_MEMORY);
	}

	ULONG64 GetModule(int pid, LPCWSTR Name)
	{
		MODULE_INFO module_info;
		module_info.Name = Name;
		INFO_STRUCT buffer = INFO_STRUCT(pid, NULL, (ULONG64)&module_info, NULL);
		syscall(&buffer, CODE_GET_MODULE);
		return (uintptr_t)module_info.StartBase;
	}

private:

	PVOID(NTAPI* syscall)(void*, INT64);
};


class MemoryToolsWrapper {
public:
	MemoryToolsWrapper(ULONG64 PId, Driver* DriverInstance) {
		driver = DriverInstance;
		ProcessId = PId;
	}
	template<typename T>
	inline auto read(uint64_t Address) -> T {
		return driver->Read_fast<T>(ProcessId, Address);
	}

	template<typename T>
	inline auto write(uint64_t Address, T Value) -> void {
		return driver->Write_fast<T>(ProcessId, Address, Value);
	}

	inline void write(uint64_t Address, void* Buffer, size_t size) {
		return driver->WriteRaw(ProcessId, Address, Buffer, size);
	}

	inline void read(uint64_t Address, void* Buffer, size_t size) {
		return driver->ReadRaw(ProcessId, Address, Buffer, size);
	}
private:
	Driver* driver;
	ULONG64 ProcessId;
};

