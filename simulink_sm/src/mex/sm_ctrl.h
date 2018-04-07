/** 

	\mainpage

	<h2>
	C++ S-function для управления генератором сигналов
	</h2>
 		
	####Входные порты
	<list type="unnumbered" >
	<item> START_IN - вход сигнала старта
	<item> D0..D7 - произвольные параметры типа double
	</list>

	###Выходные порты
	<list type="unnumbered" >
	<item> START_OUT - выход сигнала старта
	</list>

	По фронту START_IN происходит запись параметров D0..D7 в разделяемую память 
	и формирование сигнала на сбор данных, после подтверждения начала сбора данных 
	производится формирование фронта на выходе START_OUT

	При работе с приложением <b>simulink_a7dac</b> назначение параметров следующее:
	<list type="unnumbered" >
	<item> D0 - канал ЦАП 0 - частота сигнала
	<item> D1 - канал ЦАП 0 - начальная фаза сигнала
	<item> D2 - канал ЦАП 0 - амплитуда сигнала
	<item> D3 - канал ЦАП 1 - частота сигнала
	<item> D4 - канал ЦАП 1 - начальная фаза сигнала
	<item> D5 - канал ЦАП 1 - амплитуда сигнала
	<item> D6 - длина импульса в отсчётах тактовой частоты
	<item> D7 - резерв
	</list>
	При работе с другими приложениями назначение параметров может быть другим.

	### Основные функции и классы:
	<list type="unnumbered" >
	<item> BaseBlock - базовый класс для создания S-Function из библиотеки easyLink
	<item> Block - класс S-Function sm_ctrl
	<item> Block::start() - начало сеанса моделирования
	<item> Block::outputs() - формирование выходных сигналов
	<item> TF_SimulinkShared - обмен с приложением через разделяемую память
	<item> TL_SharedMemoryWrapper - управление разделяемой памятью
	</list>


	Для реализации S-Function используется библиотека <b>easyLink-3.3.0</b>

	### About easyLink-3.3.0

	easyLink is developed and maintained within the FEMTO-ST Institute located in
	Besanзon, France and hosted by Renater.

	easyLink official site is: http://sourcesup.renater.fr/easylink/

	contact: Guillaume J. Laurent, http://www.femto-st.fr/~guillaume.laurent/



 */

#define S_FUNCTION_NAME  sm_ctrl


//------------------------------------------------------------------------------
// Specify the number, the type and the sizes of the parameters of the block
// A parameter is set to tunable if its values can change during the simulation
//
// The possible types of the paramaters are:
//     * 1 for real parameters (scalar or array, use -1 as size to specify
//          a dynamically dimensioned array)
//     * 2 for literal string parameters
//     * 3 for literal identifiers of MATLAB variables
//
//------------------------------------------------------------------------------
#define PARAMETER_NUMBER                     1
//enum    parameterName                        { PAR1, PAR2, PAR3, PAR4, PAR5, PAR6 };
//int     parameterType[PARAMETER_NUMBER] =    {    1,    1,    1,    1,    2,    3 };
//int     parameterRows[PARAMETER_NUMBER] =    {    1,    2,    1,   -1,   -1,   -1 };
//int     parameterCols[PARAMETER_NUMBER] =    {    1,    3,   -1,   -1,    1,    1 };
//int     parameterTunable[PARAMETER_NUMBER] = {    1,    0,    1,    1,    1,    1 };

enum    parameterName                        { FNAME };
int     parameterType[PARAMETER_NUMBER] =    {    2  };
int     parameterRows[PARAMETER_NUMBER] =    {    1  };
int     parameterCols[PARAMETER_NUMBER] =    {    1  };
int     parameterTunable[PARAMETER_NUMBER] = {    1  };


#define INPUT_PORT_NUMBER                  9
enum    inputPortName      	               { START_IN, D0, D1, D2, D3, D4, D5, D6, D7 };
int     inputPortRows[INPUT_PORT_NUMBER] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
int     inputPortCols[INPUT_PORT_NUMBER] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };

const int blockSize = 16384;

//------------------------------------------------------------------------------
// Specify the number and the sizes of the output ports of the block
//
// Use -1 to specify a dynamically dimensioned output signal
//------------------------------------------------------------------------------
#define OUTPUT_PORT_NUMBER                   1
enum    outputPortName                       { START_OUT };
int     outputPortRows[OUTPUT_PORT_NUMBER] = { 1 };
int     outputPortCols[OUTPUT_PORT_NUMBER] = { 1 };

//------------------------------------------------------------------------------
#include "easylink.h"

class TF_SimulinkShared;


//------------------------------------------------------------------------------
class Block : public BaseBlock
{

private:

	//double		*waveform;
	//double		data;
	//int			cnt;
	//int			flag;
	int			flagLowVal_0;
	int			flagLowVal_1;

	int			flagHighVal_0;
	int			flagHighVal_1;

	int			flagError;

	unsigned	indexReq;
	unsigned	indexWr;

	int			m_DataSize;
	string		m_SharedFileName;

	TF_SimulinkShared	*m_pSimulinkShared;

public:

    void start();

    void outputs();

    void terminate();
};

//------------------------------------------------------------------------------
#include "mdldefinitions.h"

//------------------------------------------------------------------------------
