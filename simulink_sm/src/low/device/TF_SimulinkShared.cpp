


#include "TF_SimulinkShared.h"
#include "TL_SharedMemoryWrapper.h"

TF_SimulinkShared::TF_SimulinkShared( char *fname )
{

	m_pSharedMemory = new TL_SharedMemoryWrapper( fname, 1024*1024 ); // "SimulinkAdc"

	m_pBufParam = (U32*)(m_pSharedMemory->GetPtr());
	m_pBufDoubleParam = (double*)(m_pBufParam+512);
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


//! Установка параметра типа double в разделяемой памяти
double TF_SimulinkShared::SetDoubleParam( U32 id, double val )
{
	double ret = m_pBufDoubleParam[id];
	m_pBufDoubleParam[id]=val;
	return ret;
}

//! Чтение параметра типа double из разделяемой памяти
double TF_SimulinkShared::GetDoubleParam( U32 id )
{
	double ret = m_pBufDoubleParam[id];
	return ret;
}
