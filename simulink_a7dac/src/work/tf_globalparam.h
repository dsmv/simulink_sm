#ifndef TF_GLOBALPARAM_H
#define TF_GLOBALPARAM_H

struct TF_GlobalParam
{

    double  freqSynt;   //!< Частота синтезатора

    int     divA;       //!< Коэффициент деления тактовой частоты синтезатора для ЦАП
    int     divB;       //!< Коэффициент деления тактовой частоты синтезатора для внешних разъёмов

    double  dac0Freq;   //!< Частота ЦАП0
    double  dac0Phase;  //!< Начальная фаза ЦАП0
    double  dac0Ampl;   //!< Амлитуда ЦАП0

    double  dac1Freq;   //!< Частота ЦАП1
    double  dac1Phase;  //!< Начальная фаза ЦАП1
    double  dac1Ampl;   //!< Амлитуда ЦАП1

    int     is_freqSynt;    //!< 1 - изменилась частота синтезатора
    int     is_divA;        //!< 1 - изменился коэффициент divA
    int     is_divB;        //!< 1 - изменился коэффициент divB

//    int  is_dac0Freq;   //!< 1 - изменилась частота ЦАП0
//    int  is_dac0Phase;  //!< 1 - изменилась начальная фаза ЦАП0
//    int  is_dac0Ampl;   //!< 1 - изменилась амлитуда ЦАП0
//    int  is_dac1Freq;   //!< 1 - изменилась частота ЦАП1
//    int  is_dac1Phase;  //!< 1 - изменилась начальная фаза ЦАП1
//    int  is_dac1Ampl;   //!< 1 - изменилась амлитуда ЦАП1

    int  is_dacChange;  //!< 1 - изменились параметры ЦАС

    double freqExternal;    //!< Частота на внешних разъёмах W7, W8
    double freqDac;         //!< Частота дискретизации ЦАП
    int    is_chageFreq;    //!< 1 -изменение freqExternal, freqDac;

    TF_GlobalParam()
    {
        freqSynt=1000.000000;

        divA=2;
        divB=2;

        dac0Freq=10;
        dac0Phase=0;
        dac0Ampl=4000;

        dac1Freq=10;
        dac1Phase=90;
        dac1Ampl=4000;

        is_freqSynt=0;
        is_divA=0;
        is_divB=0;



//        is_dac0Freq=0;
//        is_dac0Phase=0;
//        is_dac0Ampl=0;
//        is_dac1Freq=0;
//        is_dac1Phase=0;
//        is_dac1Ampl=0;

        freqExternal=0;
        freqDac=0;
        is_chageFreq=0;

        is_dacChange=0;

    }

    ~TF_GlobalParam()
    {

    }
};


#ifdef GLOBAL_PARAM

#define ext

#else

#define ext extern

#endif


ext  TF_GlobalParam     g_GlobalParam;


#endif // TF_GLOBALPARAM_H
