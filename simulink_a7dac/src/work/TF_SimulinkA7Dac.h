
#include "TF_Test.h"
#include "brd.h"

class TF_SimulinkShared;
class TF_A7DacCtrl;

class TF_SimulinkA7Dac : public TF_Test
{


	UINT ThreadId;

	HANDLE hThread;

	static UINT  WINAPI	ThreadFunc( LPVOID   lpvThreadParm );

	public:

		TF_SimulinkA7Dac( int argc, BRDCHAR** argv );
		virtual ~TF_SimulinkA7Dac();

		virtual void Prepare( void );
		virtual void Start( void );
		virtual void Stop( void );
		virtual int isComplete( void );
		virtual void GetResult( void );
		virtual void Step( void );

		U32 Execute( void );

	U32	BlockWr;
	U32 BlockRd;

	U32 Terminate;

	U32 m_isComplete;

	//! Номер строки в таблице тестирования
	S32	m_RowNumber;

	S32	m_RowNumber2;

	////! Установка параметров по умолчанию
	//virtual void SetDefault( void );

	////! Расчёт параметров
	//virtual void CalculateParams( void );


	//! 1 - подключение к Simulink
	U32 m_isSimulink;

	//! Состояние обмена с Simulink
	U32 m_SimulinkState;

	//! Доступ к разделяемой памяти
	TF_SimulinkShared *m_pSimulinkShared;

	//! Индекс чтения RegRd
	U32 m_IndexRd;

	//! Индекс записи BufWr
	U32 m_IndexWr;

	U32 m_Flag2;
	U32 m_Flag10;

	//! Источник данных
	/**
		 0 - имитатор
		 1 - АЦП в непрерывном режиме
		 2 - АЦП в однократном режиме
	*/
	U32 m_SourceMode;

	//! Источник данных 
	TF_A7DacCtrl	*m_pSource;

	//! Параметры от Simulink
	double	m_SimulinkParam[8];
};


