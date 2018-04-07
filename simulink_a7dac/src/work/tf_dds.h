#ifndef TF_DDS_H
#define TF_DDS_H

//#include <QtGlobal>

struct TF_DDS_TaskData;

class CL_Reg;

/**
 *  \brief Формирование сигнала
 *
 *
 *
 */

class TF_DDS
{
    //! Внутренние данные класса
    TF_DDS_TaskData *td;

public:

    TF_DDS( CL_Reg *reg );

    virtual ~TF_DDS();

    //! Подготовка
    void Prepare(int flagLoadSinTable );

    //! Установка частоты
    void SetDacFreq( double syntFreq, double Freq0, double Phase0, double Ampl0,  double Freq1, double Phase1, double Ampl1 );

    void SetPhase( double syntFreq, double Freq, double Phase, __int64& phase_start, __int64& phase_inc );

    //! Установка счётчиков старта
    void SetCntStart( int regCntEnable, int regCntDisable, int regPeriod, int isContinuePeriod, int isCntEnable );

    //! Завершение вывода
    int isPeriodComplete( void );

    //! Запуск и останов
    void DacStart( int enable, int start );
};

#endif // TF_DDS_H
