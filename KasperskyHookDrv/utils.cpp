#include "utils.hpp"
#include "kernel_modules.hpp"
#include "pe.hpp"

bool data_compare( const char* pdata, const char* bmask, const char* szmask )
{
	for ( ; *szmask; ++szmask, ++pdata, ++bmask )
	{
		if ( *szmask == 'x' && *pdata != *bmask )
			return false;
	}

	return !*szmask;
}

uintptr_t utils::find_pattern( const uintptr_t base, const size_t size, const char* bmask, const char* szmask )
{
	for ( size_t i = 0; i < size; ++i )
		if ( data_compare( reinterpret_cast< const char* >( base + i ), bmask, szmask ) )
			return base + i;

	return 0;
}

uintptr_t utils::find_pattern_section( const uintptr_t base, const char* szsection, const char* bmask, const char* szmask )
{
	if ( !base || !szsection || !bmask || !szmask )
		return 0;

	const auto* psection = pe::get_section_header( base, szsection );
	return psection ? find_pattern( base + psection->VirtualAddress, psection->SizeOfRawData, bmask, szmask ) : 0;
}

uintptr_t utils::find_pattern_km( const wchar_t* szmodule, const char* szsection, const char* bmask, const char* szmask )
{
	if ( !szmodule || !szsection || !bmask || !szmask )
		return 0;

	const auto module_base = kernel_modules::get_kernel_module_base( szmodule );
	return module_base ? find_pattern_section( module_base, szsection, bmask, szmask ) : 0;
}

void* utils::get_system_routine( const wchar_t* szroutine )
{
	if ( !szroutine )
		return nullptr;

	UNICODE_STRING routine{ };
	RtlInitUnicodeString( &routine, szroutine );

	return MmGetSystemRoutineAddress( &routine );
}

uintptr_t utils::get_ntos_base()
{
	using f_RtlPcToFileHeader = PVOID( * )( PVOID PcValue, PVOID* BaseOfImage );
	const auto RtlPcToFileHeader = reinterpret_cast< f_RtlPcToFileHeader >( get_system_routine( L"RtlPcToFileHeader" ) );

	if ( !RtlPcToFileHeader )
		return 0;

	uintptr_t ntos_base = 0;
	RtlPcToFileHeader( RtlPcToFileHeader, reinterpret_cast< void** >( &ntos_base ) );

	return ntos_base;
}

bool utils::init()
{
	const auto ntos_base = get_ntos_base();

	if ( !ntos_base )
		return false;

	PsLoadedModuleList = reinterpret_cast< PLIST_ENTRY >( get_system_routine( L"PsLoadedModuleList" ) );

	if ( !PsLoadedModuleList )
	{
		auto result = find_pattern_section
		(
			ntos_base,
			".text",
			"\xC7\x43\x00\x00\x00\x00\x00\x48\x89\x43\x18\x48\x8D",
			"xx?????xxxxxx"
		);

		if ( !result )
			return false;

		result += 0xB;
		PsLoadedModuleList = reinterpret_cast< PLIST_ENTRY >( result + *reinterpret_cast< int* >( result + 0x3 ) + 0x7 );
	}

	PsLoadedModuleResource = reinterpret_cast< PERESOURCE >( get_system_routine( L"PsLoadedModuleResource" ) );

	if ( !PsLoadedModuleResource )
	{
		auto result = find_pattern_section
		(
			ntos_base,
			".text",
			"\x41\x23\xFF\x66",
			"xxxx"
		);

		if ( !result )
			return false;

		result += 0xA;
		PsLoadedModuleResource = reinterpret_cast< PERESOURCE >( result + *reinterpret_cast< int* >( result + 0x3 ) + 0x7 );
	}

	return true;
}