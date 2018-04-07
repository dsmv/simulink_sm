/** 

	\mainpage

	<h2>
	C++ S-function для приёма данных от АЦП через разделяемую память
	</h2>
 		
	####Входные порты
	<list type="unnumbered" >
	<item> START_IN - вход сигнала старта
	<item> GET_DATA - получение данных
	</list>

	###Выходные порты
	<list type="unnumbered" >
	<item> DATA - выход данных, массив 16384 отсчёта, double
	<item> START_OUT - выход сигнала старта
	</list>

	По фронту START_IN происходит передача через разделяемую память сигнала на начало сбора данных,
	после подтверждения начала сбора данных производится формирование фронта на выходе START_OUT

	По фронту GET_DATA из разделяемой памяти принимается блок данных и передаётся на выход GET_DATA

	### Основные функции и классы:
	<list type="unnumbered" >
	<item> BaseBlock - базовый класс для создания S-Function из библиотеки easyLink
	<item> Block - класс S-Function sm_adc
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

#define S_FUNCTION_NAME  sm_adc


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
#define PARAMETER_NUMBER                     2
//enum    parameterName                        { PAR1, PAR2, PAR3, PAR4, PAR5, PAR6 };
//int     parameterType[PARAMETER_NUMBER] =    {    1,    1,    1,    1,    2,    3 };
//int     parameterRows[PARAMETER_NUMBER] =    {    1,    2,    1,   -1,   -1,   -1 };
//int     parameterCols[PARAMETER_NUMBER] =    {    1,    3,   -1,   -1,    1,    1 };
//int     parameterTunable[PARAMETER_NUMBER] = {    1,    0,    1,    1,    1,    1 };

enum    parameterName                        { FNAME, DATA_SIZE };
int     parameterType[PARAMETER_NUMBER] =    {    2,    1 };
int     parameterRows[PARAMETER_NUMBER] =    {    1,    1 };
int     parameterCols[PARAMETER_NUMBER] =    {    1,    1 };
int     parameterTunable[PARAMETER_NUMBER] = {    1,    1 };


#define INPUT_PORT_NUMBER                  2
enum    inputPortName      	               { START_IN, GET_DATA };
int     inputPortRows[INPUT_PORT_NUMBER] = { 1, 1 };
int     inputPortCols[INPUT_PORT_NUMBER] = { 1, 1 };

const int blockSize = 16384;

//------------------------------------------------------------------------------
// Specify the number and the sizes of the output ports of the block
//
// Use -1 to specify a dynamically dimensioned output signal
//------------------------------------------------------------------------------
#define OUTPUT_PORT_NUMBER                   2
enum    outputPortName                       { DATA, START_OUT };
int     outputPortRows[OUTPUT_PORT_NUMBER] = {   blockSize, 1 };
int     outputPortCols[OUTPUT_PORT_NUMBER] = {   1, 1 };

//------------------------------------------------------------------------------
#include "easylink.h"

class TF_SimulinkShared;


//! S-Function sm_adc.mex64w
class Block : public BaseBlock
{

private:


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
