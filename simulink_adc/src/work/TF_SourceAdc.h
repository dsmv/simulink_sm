
#ifndef TF_SourceAdc_H 

#define TF_SourceAdc_H 


#include "TF_BaseSource.h"

struct TF_SourceAdc_TaskData;

/**
		\brief Ввод данных от АЦП

		Класс принимает данные от АЦП с использованием библиотеки BARDY  
		Для работы с АЦП должен быть установлен пакет DaqTools 
		для выбранного сочетания базового модуля и субмодуля.
		http://insys.ru/downloads/daqtools64

		В каталоге simulink_adc\bin64 должен быть файл brd.ini настроенный 
		на сочетание базового модуля и субмодуля и файл exam_adc.ini в котором 
		должны быть установлены параметры АЦП.

		\section Main Основные функции:
		<list type="unnumbered">
		<item>PrepareAdc() - инициализация АЦП
		<item>ReStartSession() - перезапуск сеанса сбора, запускается при запуске моделирования в Simulink
		<item>EventStartCycle() - запуск сбора
		<item>GetData() -получение блока данных
		</list>

		Константа SIZE_DATA_OUT_ARRAY в файле global.h определяет размер собираемого блока данных.



*/
class TF_SourceAdc  : public TF_BaseSource
{

	//! Internal data of TF_SourceAdc
	TF_SourceAdc_TaskData	*td;

public:

	TF_SourceAdc( int argc, BRDCHAR** argv );
	virtual  ~TF_SourceAdc();

	//! Prepare function
	virtual int Prepare( int count );

	//! Start new session of process data 
	virtual void ReStartSession( void );

	//! Start of new data cycle
	virtual void EventStartCycle( void );

	//! Get buffer of data
	/**
		\param ptr	pointer of pointer of data, output
		\return		1 - set new pointer to ptr
	*/
	virtual int GetData( U32 **ptr );


	//! Подготовка FM214x250M
	void PrepareAdc( char* fname );

};


#endif


