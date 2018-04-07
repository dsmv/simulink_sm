
#ifndef __SYSIPC_H__
#define __SYSIPC_H__

#ifndef __IPCLIB_H__
    #include "ipclib.h"
#endif

//----------------------------------------------------------------------
// Функции для работы с системными вызовами
//----------------------------------------------------------------------

extern "C" {

    //! Инициализирует буфер клавиатуры в неканоническое состояние
GIPCY_API    void IPC_initKeyboard(void);

    //! Восстанавливает буфер клавиатуры в каноническое состояние
GIPCY_API    void IPC_cleanupKeyboard(void);

    //! Ожидает нажатия клавиши (получает символ с консоли)
    /*!
      \return возвращает код нажатой клавиши
      */
GIPCY_API    int IPC_getch(void);

//! Ожидает нажатия клавиши (получает символ с консоли и отображает его)
/*!
  \return возвращает код нажатой клавиши
  */
GIPCY_API    int IPC_getche(void);

    //! Проверяет была ли нажата клавиша (проверяет консоль на ввод с клавиатуры)
    /*!
      \return в случае ошибки возвращает 0
      */
GIPCY_API    int IPC_kbhit(void);

	//! Выполняет задержку в миллисекундах
    /*!
      \param ms - время задержки, в мс
      */
GIPCY_API    void IPC_delay(int ms);

    //! Возвращает код последней ошибки
GIPCY_API    int IPC_sysError();

    //! Функция определяет полный путь у указанному файлу
    /*!
    \param name - имя файла
    \param path - полный путь
    \return код ошибки
    */
GIPCY_API    int IPC_getFullPath(const IPC_str *name, IPC_str *path);

    //! Функция определяет текущий рабочий каталог
    /*!
    \param buf - буфер для сохранения пути к текущему каталогу
    \param size - размер буфера
    \return путь к текущей директории
    */
GIPCY_API    const IPC_str* IPC_getCurrentDir(IPC_str *buf, int size);

GIPCY_API    int IPC_getPrivateProfileString( const IPC_str *lpAppName, const IPC_str *lpKeyName, const IPC_str *lpDefault,
                                     IPC_str *lpReturnedString, int nSize, const IPC_str *lpFileName );

GIPCY_API    int IPC_writePrivateProfileString( const IPC_str *lpAppName, const IPC_str *lpKeyName, const IPC_str *lpString, const IPC_str *lpFileName );

GIPCY_API    void* IPC_heapAlloc(int nSize);
GIPCY_API    int IPC_heapFree(void *ptr);

GIPCY_API    long IPC_interlockedDecrement(volatile long *val );
GIPCY_API    long IPC_interlockedIncrement(volatile long *val );
GIPCY_API    long IPC_interlockedCompareExchange(volatile long *dst, long val, long param );
GIPCY_API    long IPC_interlockedExchange(volatile long *dst, long val );
GIPCY_API    long IPC_interlockedExchangeAdd(volatile long *dst, long val );

	//! Конвертирует дескриптор IPC в файловый дескриптор
//GIPCY_API    int IPC_handleToFile(IPC_handle handle);

    //! Конвертирует дескриптор IPC в файловый дескриптор устройства
//GIPCY_API    int IPC_handleToDevice(IPC_handle handle);

GIPCY_API   IPC_str* IPC_itoa(int value, char* result, int base);
GIPCY_API   int IPC_strlwr(IPC_str *str);
GIPCY_API   unsigned int IPC_flushall();
GIPCY_API   long IPC_getTickCount();
GIPCY_API   long IPC_getTickPerSec();
}

#endif //__SYSIPC_H__
