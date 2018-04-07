/**
	\brief Работа с разделяемой памятью
*/


#include "brd.h"

//! Работа с общей памятью
class TL_SharedMemoryWrapper
{

	//! Параметры стрима
	//STREAM_PARAM m_Param;

	HANDLE	m_handleMemory;

	//! Структура указателя на один буфер 
	struct BufPtr
	{
		void* ptr;			//!< Указатель на буфер
		HANDLE  handle;		//!< дескриптор файла
		char    name[128];	//!< имя файла

		BufPtr()
		{
			ptr = NULL;
			handle = NULL;
		};

		~BufPtr()
		{
			clear();
		};

		void clear( void )
		{
			if( ptr )
			{
				UnmapViewOfFile( (void*)ptr );
				CloseHandle( handle );
			}
			ptr = NULL;
			handle = NULL;
		};
	};

	int m_SizeOfBytes;	//!< Размер области памяти в байтах

	//! Массив указателей на буфера 
	BufPtr m_PtrBuf[1];

	//! Освобождение памяти
	void FreeSharedMemory( void );

	public:


	TL_SharedMemoryWrapper( char* fname, int sizeOfBytes );

	~TL_SharedMemoryWrapper();

	//! Получение указателя на память
	void* GetPtr( void );


};