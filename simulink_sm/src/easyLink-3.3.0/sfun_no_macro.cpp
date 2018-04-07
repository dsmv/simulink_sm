//------------------------------------------------------------------------------
/* This file does the same than sfun_offset.cpp but illustrates
 * how to write a C++ S-function without using global variables that define
 * input, output and parameter port dimensions.
 */
//------------------------------------------------------------------------------
#define S_FUNCTION_NAME sfun_no_macro

//------------------------------------------------------------------------------
#include "easylink.h"

//------------------------------------------------------------------------------
class Block : public BaseBlock
{
public:

    static void initializeParameterPortSizes()
    {
        ssSetNumSFcnParams(simStruct,1);
    }

    static void initializeInputPortSizes()
    {
        ssSetNumInputPorts(simStruct,1);
        ssSetInputPortDirectFeedThrough(simStruct,0,1);
        ssSetInputPortRequiredContiguous(simStruct,0,1);
        setInputPortDimensions(0,1,1);
    }

    static void initializeOutputPortSizes()
    {
        ssSetNumOutputPorts(simStruct,1);
        setOutputPortDimensions(0,1,1);
    }

    void start()
    {
    }

    void outputs()
    {
        double in=getInputDouble(0);
        double offset=getParameterDouble(0);

        setOutputDouble(0,in+offset);
    }

    void terminate()
    {
    }

};

//------------------------------------------------------------------------------
#include "mdldefinitions.h"

//------------------------------------------------------------------------------
