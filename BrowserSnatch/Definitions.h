#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <Windows.h>

// Structures for ntdll APIs
typedef struct _LSA_UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING, * PUNICODE_STRING;

typedef struct _OBJECT_ATTRIBUTES {
	ULONG Length; HANDLE RootDirectory;
	PUNICODE_STRING ObjectName; ULONG Attributes;
	PVOID SecurityDescriptor;
	PVOID SecurityQualityOfService;
} OBJECT_ATTRIBUTES, * POBJECT_ATTRIBUTES;

typedef struct _CLIENT_ID {
	PVOID UniqueProcess;
	PVOID UniqueThread;
} CLIENT_ID, * PCLIENT_ID;

#define InitializeObjectAttributes(p, n, a, r, s) \
{ \
	(p)->Length = sizeof(OBJECT_ATTRIBUTES); \
	(p)->RootDirectory = r; \
	(p)->Attributes = a; \
	(p)->ObjectName = n; \
	(p)->SecurityDescriptor = s; \
	(p)->SecurityQualityOfService = NULL; \
}

// Undocumneted function definitions
using myNtCreateSection = NTSTATUS(NTAPI*)(OUT PHANDLE SectionHandle, IN ULONG DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL, IN PLARGE_INTEGER MaximumSize OPTIONAL, IN ULONG PageAttributess, IN ULONG SectionAttributes, IN HANDLE FileHandle OPTIONAL);
using myNtMapViewOfSection = NTSTATUS(NTAPI*)(HANDLE SectionHandle, HANDLE ProcessHandle, PVOID* BaseAddress, ULONG_PTR ZeroBits, SIZE_T CommitSize, PLARGE_INTEGER SectionOffset, PSIZE_T ViewSize, DWORD InheritDisposition, ULONG AllocationType, ULONG Win32Protect);
using myRtlCreateUserThread = NTSTATUS(NTAPI*)(IN HANDLE ProcessHandle, IN PSECURITY_DESCRIPTOR SecurityDescriptor OPTIONAL, IN BOOLEAN CreateSuspended, IN ULONG StackZeroBits, IN OUT PULONG StackReserved, IN OUT PULONG StackCommit, IN PVOID StartAddress, IN PVOID StartParameter OPTIONAL, OUT PHANDLE ThreadHandle, OUT PCLIENT_ID ClientID);
using myNtOpenProcess = NTSTATUS(NTAPI*)(OUT PHANDLE ProcessHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, IN PCLIENT_ID ClientId);
using myNtOpenThread = NTSTATUS(NTAPI*)(OUT PHANDLE ThreadHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, IN PCLIENT_ID ClientId);
using myNtSuspendThread = NTSTATUS(NTAPI*)(IN HANDLE ThreadHandle, OUT PULONG PreviousSuspendCount OPTIONAL);
using myNtGetContextThread = NTSTATUS(NTAPI*)(IN HANDLE ThreadHandle, IN OUT PCONTEXT ThreadContext);
using myNtSetContextThread = NTSTATUS(NTAPI*)(IN HANDLE ThreadHandle, IN PCONTEXT ThreadContext);
using myNtResumeThread = NTSTATUS(NTAPI*)(IN HANDLE ThreadHandle, OUT PULONG PreviousSuspendCount OPTIONAL);

#endif // DEFINITIONS_H
