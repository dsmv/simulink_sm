
#include <stdio.h>
#include "utypes.h"

#include "tf_a7dac.h"

#include "ftd2xx.h"

struct CMD_Item
{
    int dev;    //!< узел
    int adr;    //!< адрес
    int data;   //!< данные для записи
    int *pData; //!< указатель для чтения
    int cmd;    //!< команда, 1 - запись, 0 - чтение
};

//! Внутренние данные класса
struct TF_A7Dac_TaskData
{
    FT_HANDLE ftHandle; //!< дескриптор устройства

    CMD_Item        items[256];     //!< массив команд
    unsigned int    cmd_index;      //!< номер текущей команды
    unsigned int    cmd_status;     //!< состояние команды

    TF_A7Dac_TaskData()
    {
       cmd_status=0;
       cmd_index=0;
    }
};

TF_A7Dac::TF_A7Dac()
{
    td = new TF_A7Dac_TaskData();
}

TF_A7Dac::~TF_A7Dac()
{
    delete td; td=NULL;
}

//! Начало работы с модулем
int TF_A7Dac::Open()
{

    printf( "Доступ к FTDI\n");
    FT_STATUS ftStatus;
    DWORD numDevs=-1;

    //ftStatus=FT_SetVIDPID( 0x0403, 0x6015 );
    //ftStatus=FT_CreateDeviceInfoList( numDevs );

    ftStatus = FT_ListDevices(&numDevs,NULL,FT_LIST_NUMBER_ONLY);
    if (ftStatus == FT_OK) {
        // FT_ListDevices OK, number of devices connected is in numDevs
        printf( "Число устройств=%d\n", numDevs);
    }
    else {
        // FT_ListDevices failed
        printf( "Устройство не подключено\n" );
        return 1;
    }

    ftStatus=FT_Open( 0, &(td->ftHandle));
    if( FT_OK==ftStatus )
    {
        printf( "A7_Dac - устройство открыто\n" );
    } else
    {
        printf( "A7_Dac - ошибка доступа к устройству\n");
        return 2;
    }

/*
    FT_SetBitMode(  td->ftHandle, 0xF, 1 );

        DWORD bytesWritten;
        int buf=1;
        for( int ii=0; ii<16; ii++ )
        {
         buf=ii;
         ftStatus=FT_Write( td->ftHandle, &buf, 4, &bytesWritten );
         ftStatus=FT_Read( td->ftHandle,  &buf, 4, &bytesWritten );
         break;
        }

    FT_SetBitMode(  td->ftHandle, 0, 0 );


    {
        DWORD bytesWritten;
        int buf=0;
        for( int ii=0; ii<16; ii++ )
        {
         buf=0;
         ftStatus=FT_Write( td->ftHandle, &buf, 4, &bytesWritten );
         //ftStatus=FT_Read( td->ftHandle,  &buf, 4, &bytesWritten );
        }

    }
*/

    //PldLoad( "a7dac_m2.bit" );

    int kk=0;

//return 0;
    for( ; ; )
    {
        int pldDone = GetPldDone();
        //pldDone=0;
        if( pldDone )
        {
            printf( "\nПЛИС загружена\n");
            break;
        } else
        {
            printf( "\nПЛИС не загружена\n");
            char* fname = "a7dac_m2.bit";
            printf( "\nЗагрузка прошивки %s ", fname );
            int ret=PldLoad( fname );
            if( 1==ret )
            {
                printf( " - Ok\n\n");
                break;
            }
            else
            {
                printf( " - Error\n\n");
                kk++;
                if( kk==5 )
                    return 1;
            }

        }
    }

    FT_Close( td->ftHandle );

    FT_Open( 0, &(td->ftHandle));

    {
         int data;
         for( int ii=0; ; ii++ )
         {
             PkgCmdStart();
             PkgCmdRead( 0, 0, &data );
             PkgCmdExecute();

             if( data!=0xFFFFFFFF )
                 break;
         }
    }
     //FT_ResetPort(  td->ftHandle );
     //FT_CyclePort(  td->ftHandle );
     //FT_SetUSBParameters( td->ftHandle, 2048, 2048 );
      //FT_StopInTask( td->ftHandle );
      //FT_RestartInTask( td->ftHandle );
     //FT_Open( 0, &(td->ftHandle));

    return 0;
}

//! Проверка наличия прошивки
int TF_A7Dac::GetPldDone( void )
{
    int ret=0;

    DWORD bytesWritten;
    unsigned char buf=1;

    FT_SetBitMode(  td->ftHandle, 0, 1 );

//    buf=0xFA;
//    FT_Write( td->ftHandle, &buf, 4, &bytesWritten );
//    FT_Read( td->ftHandle,  &buf, 4, &bytesWritten );


//    buf=0x7C;
//    FT_Write( td->ftHandle, &buf, 4, &bytesWritten );
//    FT_Read( td->ftHandle,  &buf, 4, &bytesWritten );

//    buf=0xFF;
//    FT_Write( td->ftHandle, &buf, 4, &bytesWritten );
//    FT_Read( td->ftHandle,  &buf, 4, &bytesWritten );

    char data_in[4096];
//    for( int ii=0; ii<512; ii++ )
//    {
//        FT_Read( td->ftHandle,  &data_in[0], 4096, &bytesWritten );
//    }
//    buf=data_in[0];
    FT_GetBitMode(td->ftHandle, &buf );


    if( buf&0x10)
        ret=1;

    FT_SetBitMode(  td->ftHandle, 0, 0 );

    return ret;
}

//! Загрузка прошивки ПЛИС
int TF_A7Dac::PldLoad( char* fname )
{
    int ret=0;
    DWORD bytesWritten;
    unsigned char buf=1;

    FILE *fl = fopen( fname, "rb" );
    if( NULL==fl )
    {
        printf( "Ошибка доступа к файлу %s\n", fname );
        return 0;
    }

    FT_SetBitMode(  td->ftHandle, 0x80, 1 );

    {

        //FT_Read( td->ftHandle,  &buf, 1, &bytesWritten );
        FT_GetBitMode(td->ftHandle, &buf );

        // Сброс прошивки ПЛИС
        buf=0x60;
        FT_Write( td->ftHandle, &buf, 1, &bytesWritten );

        buf=0xE0;
        FT_Write( td->ftHandle, &buf, 1, &bytesWritten );

        FT_SetBitMode(  td->ftHandle, 0, 0 );
        FT_SetBitMode(  td->ftHandle, 0x40, 1 );

        for( int jj=0; ; jj++ )
        {
            //FT_Read( td->ftHandle,  &buf, 4, &bytesWritten );
            FT_GetBitMode(td->ftHandle, &buf );
            if( 0==(buf & 0x10) )
                break;
        }
    }

    char data_in[512];
    char data_out[512*8];
    int index;
    for( ; ; )
    {
            index=fread( &data_in[0], 1, 512, fl );
            if( 0==index )
                break;

            for( int kk=0; kk<index; kk++ )
            {
                // формирование массива
                for( int ii=0; ii<8; ii++ )
                {
                    data_out[kk*8+ii]=0x80;
                    if( data_in[kk] & (1<<(7-ii)) )
                    {
                        data_out[kk*8+ii]|= 0xC0;
                    }
                }
            }
        FT_Write( td->ftHandle, &data_out[0], index*8, &bytesWritten );
    }



    for( int ii=0; ii<100; ii++ )
    {
        FT_Write( td->ftHandle, &data_out[0], 8, &bytesWritten );
    }

    fclose( fl );

    FT_SetBitMode(  td->ftHandle, 0, 0 );
    FT_SetBitMode(  td->ftHandle, 0, 1 );
    ret=0;
    for( int ii=0; ii<5000000 ; ii++ )
    {

        // Проверка загрузки прошивки
        //FT_Read( td->ftHandle,  &data_out[0], 4096, &bytesWritten );
        FT_GetBitMode(td->ftHandle, &buf );
        //buf=data_out[0];
        if( buf&0x10)
        {
            ret=1;
            break;
        }
    }

    FT_SetBitMode(  td->ftHandle, 0, 0 );
    return ret;
}


//! Начало выполнения команды
void TF_A7Dac::PkgCmdStart( void )
{
    td->cmd_status=1;
    td->cmd_index=0;
}

//! Запись в регистр
void TF_A7Dac::PkgCmdWrite( int dev, int adr, int data )
{
    if( td->cmd_index<63 )
    {
        td->items[td->cmd_index].adr=adr;
        td->items[td->cmd_index].dev=dev;
        td->items[td->cmd_index].data=data;
        td->items[td->cmd_index].pData=NULL;
        td->items[td->cmd_index].cmd=1;
        td->cmd_index++;
    }
}

//! Чтение из регистра
void TF_A7Dac::PkgCmdRead( int dev, int adr, int *pData )
{
    if( td->cmd_index<63 )
    {
        td->items[td->cmd_index].adr=adr;
        td->items[td->cmd_index].dev=dev;
        td->items[td->cmd_index].data=0;
        td->items[td->cmd_index].pData=pData;
        td->items[td->cmd_index].cmd=0;
        td->cmd_index++;
    }
}

//! Выполнение команды
void TF_A7Dac::PkgCmdExecute( void )
{
    // Формирование массива
    int BufOut[128];
    int BufIn[128];
    int Buf4BitOut[256];
    int Buf4BitIn[256];

    for( int ii=0; ii<128; ii++ )
    {
       BufOut[ii]=0;
       BufIn[ii]=0;
    }

    int cmd;
    CMD_Item *pItem;
    for( int ii=0; ii<td->cmd_index; ii++)
    {
        pItem = td->items + ii;
        cmd = 7;
        if( pItem->cmd )
            cmd |=0x10; // команда записи
        cmd |= (pItem->adr&0xFFFF) << 8; // адрес
        cmd |= pItem->dev << 24;       // узел

        BufOut[2*ii]=cmd;
        BufOut[2*ii+1]=pItem->data;
    }

    // Формирование промежуточного массива

    {
        char *src=(char*)BufOut;
        char *dst=(char*)Buf4BitOut;
        int val;
        for( int ii=0; ii<td->cmd_index*8; ii++ )
        {
            val = *src++;
            *dst++ = 0xF0 | (val & 0xF);
            *dst++ = 0xF0 | ((val>>4) & 0xF);
        }
    }

    // Передача массива
    char *ptr = (char*)Buf4BitOut;


    DWORD bytesWritten;
    int size=td->cmd_index*16;
    FT_STATUS ftStatus;
    for( ; ; )
    {
       ftStatus=FT_Write( td->ftHandle, ptr, size, &bytesWritten );
       size-=bytesWritten;
       ptr+=bytesWritten;
       if( 0==size )
           break;
    }

    // Приём ответа
    ptr = (char*)Buf4BitIn;
    size=td->cmd_index*16;
    for( ; ; )
    {
       ftStatus=FT_Read( td->ftHandle, ptr, size, &bytesWritten );
       size-=bytesWritten;
       ptr+=bytesWritten;
       if( 0==size )
           break;
    }

    // Формирование ответного массива из промежуточного
    {
        char val0, val1, val;
        char *src=(char*)Buf4BitIn;
        char *dst=(char*)BufIn;
        for( int ii=0; ii<td->cmd_index*8; ii++ )
        {
            val0 = *src++;
            val1 = *src++;
            val = (val1<<4) | (val0&0x0F);
            *dst++=val;
        }


    }


//    for( int ii=0; ii<td->cmd_index; ii++ )
//    {
//        printf( "Write: %.8X %.8X\n", BufOut[2*ii], BufOut[2*ii+1] );
//    }


//    for( int ii=0; ii<td->cmd_index; ii++ )
//    {
//        printf( "Read: %.8X %.8X\n", BufIn[2*ii], BufIn[2*ii+1] );
//    }

    for( int ii=0; ii<td->cmd_index; ii++ )
    {
        pItem = td->items + ii;
        if( 0==pItem->cmd && pItem->pData )
        {
            *(pItem->pData)=BufIn[2*ii+1];
        }

    }

}
