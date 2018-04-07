
#include "brd.h"

class TL_SharedMemoryWrapper;


//! Обмен между Simulink и ADC через разделяемую память
class TF_SimulinkShared 
{

	TL_SharedMemoryWrapper	*m_pSharedMemory;

	//! Указатель на буфер параметров
	U32		*m_pBufParam;

	//! Указатель на буфер данных
	U32		*m_pBufData;

	//! Указатель на область параметров типа double
	double  *m_pBufDoubleParam;

public:

		TF_SimulinkShared( char* fname );
		~TF_SimulinkShared();

		//! Получение флага из разделяемой памяти
		U32	GetFlag( U32 id );

		//! Установка флага в разделяемой памяти
		U32 SetFlag( U32 id, U32 val );

		//! Получение указателя на буфер
		U32* GetBuf( void );

		//! Установка параметра типа double в разделяемой памяти
		double SetDoubleParam( U32 id, double val );

		//! Чтение параметра типа double из разделяемой памяти
		double GetDoubleParam( U32 id );

};


