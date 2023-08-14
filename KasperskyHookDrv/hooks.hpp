#pragma once

#include <ntifs.h>
#include <windef.h>

// Typedefs
//
using f_NtCreateFile = NTSTATUS( * )
(
	PHANDLE            FileHandle,
	ACCESS_MASK        DesiredAccess,
	POBJECT_ATTRIBUTES ObjectAttributes,
	PIO_STATUS_BLOCK   IoStatusBlock,
	PLARGE_INTEGER     AllocationSize,
	ULONG              FileAttributes,
	ULONG              ShareAccess,
	ULONG              CreateDisposition,
	ULONG              CreateOptions,
	PVOID              EaBuffer,
	ULONG              EaLength
);


using f_NtReadFile = NTSTATUS(*)
(
	HANDLE           FileHandle,
	HANDLE           Event,
	PIO_APC_ROUTINE  ApcRoutine,
	PVOID            ApcContext,
	PIO_STATUS_BLOCK IoStatusBlock,
	PVOID            Buffer,
	ULONG            Length,
	PLARGE_INTEGER   ByteOffset,
	PULONG           Key
);

// Functions (hooks)
//
namespace hooks
{
	NTSTATUS hk_NtCreateFile
	(
		PHANDLE            FileHandle,
		ACCESS_MASK        DesiredAccess,
		POBJECT_ATTRIBUTES ObjectAttributes,
		PIO_STATUS_BLOCK   IoStatusBlock,
		PLARGE_INTEGER     AllocationSize,
		ULONG              FileAttributes,
		ULONG              ShareAccess,
		ULONG              CreateDisposition,
		ULONG              CreateOptions,
		PVOID              EaBuffer,
		ULONG              EaLength
	);

	NTSTATUS hk_NtReadFile
	(
		HANDLE           FileHandle,
		HANDLE           Event,
		PIO_APC_ROUTINE  ApcRoutine,
		PVOID            ApcContext,
		PIO_STATUS_BLOCK IoStatusBlock,
		PVOID            Buffer,
		ULONG            Length,
		PLARGE_INTEGER   ByteOffset,
		PULONG           Key
	);
}

// Function pointers to original funcs
//
extern f_NtCreateFile o_NtCreateFile;
extern f_NtReadFile o_NtReadFile;