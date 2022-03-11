#include "hooks.hpp"
#include "utils.hpp"

f_NtCreateFile o_NtCreateFile = nullptr;

NTSTATUS hooks::hk_NtCreateFile
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
	ULONG              EaLength )
{
	if ( ObjectAttributes && ObjectAttributes->ObjectName && ObjectAttributes->ObjectName->Buffer )
	{
		const auto name = ObjectAttributes->ObjectName->Buffer;

		if (wcsstr(name, L"Speedyyy.txt"))
		{
			//return STATUS_ACCESS_DENIED;
			return o_NtCreateFile(FileHandle,DesiredAccess,ObjectAttributes,IoStatusBlock,AllocationSize,FileAttributes,ShareAccess,CreateDisposition,CreateOptions,EaBuffer,EaLength);
		}

	}

	// Call the original function
	//
	return o_NtCreateFile( FileHandle, 
						   DesiredAccess, 
						   ObjectAttributes, 
						   IoStatusBlock, 
						   AllocationSize, 
						   FileAttributes, 
						   ShareAccess, 
						   CreateDisposition, 
						   CreateOptions, 
						   EaBuffer, 
						   EaLength );
}