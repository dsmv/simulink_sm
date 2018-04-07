
#include "TF_Test.h"
#include "brd.h"

class CL_AMBPEX;
class TF_SimulinkShared;
class TF_BaseSource;

/**
	\brief Основной класс управления АЦП

	Класс запускает поток в котором происходит взаимодействие между Simulink и источником данных.

	m_pSource - указатель на источник данных, может быть имитатор, АЦП или что-то другое.

	Execute() - функция потока, в ней происходит опрос разделяемой памяти. 
	Через разделяемую память передаются команды на перезапуск сеанса сбора,
	сбор блока данных и передаётся собранный блок данных.


*/
class TF_SimulinkAdc : public TF_Test
{


	UINT ThreadId;

	HANDLE hThread;

	static UINT  WINAPI	ThreadFunc( LPVOID   lpvThreadParm );

	public:

		TF_SimulinkAdc( int argc, BRDCHAR** argv );
		virtual ~TF_SimulinkAdc();

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
		 1 - АЦП в однократном режиме
	*/
	U32 m_SourceMode;

	//! Источник данных 
	TF_BaseSource	*m_pSource;
};


