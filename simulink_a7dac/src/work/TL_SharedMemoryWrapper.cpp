

#include "TL_SharedMemoryWrapper.h"

#include <tchar.h>



TL_SharedMemoryWrapper::TL_SharedMemoryWrapper( char* fname, int sizeOfBytes )
{
	m_SizeOfBytes = sizeOfBytes;

	strcpy( m_PtrBuf[0].name, fname );

	m_PtrBuf[0].handle = CreateFileMappingA(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		m_SizeOfBytes,
		fname
		);

	if( NULL == m_PtrBuf[0].handle )
		throw( _T( "Ошибка вызова CreateFileMappingA" ) );


	m_PtrBuf[0].ptr = MapViewOfFile(
		m_PtrBuf[0].handle,	// HANDLE hFileMappingObject,
		FILE_MAP_WRITE,		// DWORD dwDesiredAccess,
		0,					// DWORD dwFileOffsetHigh,
		0,					// DWORD dwFileOffsetLow,
		m_SizeOfBytes		// SIZE_T dwNumberOfBytesToMap
		);

	if( NULL == m_PtrBuf[0].ptr )
		throw(_T( "Ошибка вызова MapViewOfFile" ) );


}

TL_SharedMemoryWrapper::~TL_SharedMemoryWrapper()
{


	FreeSharedMemory();

}



//! Освобождение памяти
void TL_SharedMemoryWrapper::FreeSharedMemory( void )
{
	int ret = 0;

	U32 cntBuf = 1;

	for( int ii = 0; ii < cntBuf; ii++ )
	{
		m_PtrBuf[ii].clear();
	}

}

//! Получение указателя на память
void* TL_SharedMemoryWrapper::GetPtr( void )
{
	return m_PtrBuf[0].ptr;
}
