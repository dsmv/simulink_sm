#ifndef TF_A7DAC_H
#define TF_A7DAC_H

#include "cl_reg.h"

struct TF_A7Dac_TaskData;

/**
     \brief Управление модулем A7_Dac



*/
class TF_A7Dac : public CL_Reg
{

    //! Внутренние данные класса
    TF_A7Dac_TaskData  *td;

public:
    TF_A7Dac();
    virtual ~TF_A7Dac();

    //! Начало работы с модулем
    int Open( void );

    //! Начало выполнения команды
    virtual void PkgCmdStart( void );

    //! Запись в регистр
    virtual void PkgCmdWrite( int dev, int adr, int data );

    //! Чтение из регистра
    virtual void PkgCmdRead( int dev, int adr, int *pData );

    //! Выполнение команды
    virtual void PkgCmdExecute( void );

    //! Проверка наличия прошивки
    int GetPldDone( void );

    //! Загрузка прошивки ПЛИС
    int PldLoad( char* fname );

};

#endif // TF_A7DAC_H
