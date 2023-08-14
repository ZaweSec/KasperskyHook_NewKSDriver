#include "hooks.hpp"
#include "utils.hpp"

f_NtCreateFile o_NtCreateFile = nullptr;
f_NtReadFile o_NtReadFile = nullptr;

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

		/*if (wcsstr(name, L"SerseSkidda.txt"))
		{
			log("CHE CAZZO DICI CHE SKIDDA");
			return STATUS_ACCESS_DENIED;
		}
		*/
		if (wcsstr(name, (L"C:\\SerseSkidda.txt")))
		{

			PCWSTR filePath = L"\\??\\C:\\DioNegro.txt";
			UNICODE_STRING unicodeString;
			RtlInitUnicodeString(&unicodeString, filePath);
			ObjectAttributes->ObjectName = &unicodeString;
			return ZwCreateFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength);
		}


	}

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

/*   TESTING DO NOT USE THAT     */ 

NTSTATUS hooks::hk_NtReadFile
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
) {

	
	auto ret = ZwReadFile(
		FileHandle,
		Event,
		ApcRoutine,
		ApcContext,
		IoStatusBlock,
		Buffer,
		Length,
		ByteOffset,
		Key);

	if(NT_SUCCESS(ret))
		DbgPrint("[driver3] NameBefore %ws \n", Buffer);

	return ret;
}