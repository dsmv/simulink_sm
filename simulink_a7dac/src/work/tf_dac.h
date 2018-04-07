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


};

#endif // TF_PLL_H
