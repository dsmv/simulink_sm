#ifndef TF_PLL_H
#define TF_PLL_H

struct TF_PLL_TaskData;

class CL_Reg;

/**
 *  \brief Управление синтезатором
 *
 *
 *
 */

class TF_PLL
{
    //! Внутренние данные класса
    TF_PLL_TaskData *td;

public:

    TF_PLL( CL_Reg *reg );

    virtual ~TF_PLL();

    //! Подготовка PLL
    void Prepare( void );

    //! Установка тактовой частоты
    int SetPll( double freq, double *pFreqOut );

    //! Запись в регистр синтезатора
    void SpdWrite( unsigned int adr, unsigned int data );

    //! Установка делителей
    void SetDiv( int divA, int divB );

    //! Установленное значение частоты
    double  GetFreq( int n );

};

#endif // TF_PLL_H
