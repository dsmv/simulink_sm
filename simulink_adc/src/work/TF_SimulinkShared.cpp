


#include "TF_SimulinkShared.h"
#include "TL_SharedMemoryWrapper.h"

TF_SimulinkShared::TF_SimulinkShared()
{

	m_pSharedMemory = new TL_SharedMemoryWrapper( "SimulinkAdc", 1024*1024 );

	m_pBufParam = (U32*)(m_pSharedMemory->GetPtr());
	m_pBufData = m_pBufParam+1024;

}

TF_SimulinkShared::~TF_SimulinkShared()
{
	delete m_pSharedMemory; m_pSharedMemory = NULL;
}

//! Получение флага из разделяемой памяти
U32	TF_SimulinkShared::GetFlag( U32 id )
{
	U32 ret = m_pBufParam[id];
	return ret;
}

//! Установка флага в разделяемой памяти
U32 TF_SimulinkShared::SetFlag( U32 id, U32 val )
{
	U32 ret = m_pBufParam[id];
	m_pBufParam[id]=val;
	return ret;
}

//! Получение указателя на буфер
U32* TF_SimulinkShared::GetBuf( void )
{
	return m_pBufData;
}
