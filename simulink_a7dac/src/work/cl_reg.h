#ifndef CL_Reg_H
#define CL_Reg_H




/**
     \brief Доступ к регистраи



*/
class CL_Reg
{


public:
    CL_Reg(){};
    virtual ~CL_Reg(){};

    //! Начало выполнения команды
    virtual void PkgCmdStart( void ) {};

    //! Запись в регистр
    virtual void PkgCmdWrite( int dev, int adr, int data ) {};

    //! Чтение из регистра
    virtual void PkgCmdRead( int dev, int adr, int *pData ) {};

    //! Выполнение команды
    virtual void PkgCmdExecute( void ) {};

};

#endif // CL_Reg_H
