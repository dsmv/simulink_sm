//------------------------------------------------------------------------------
/* This file does the same than sfun_size_change.cpp but illustrates
 * how to write directly in the output array without using setOutputArray.
 */
//------------------------------------------------------------------------------
#define S_FUNCTION_NAME sfun_direct_output_access

//------------------------------------------------------------------------------
// Specify the number and the sizes of the input ports of the block
//
// Use -1 to specify a dynamically dimensioned input signal
//------------------------------------------------------------------------------
#define INPUT_PORT_NUMBER                  1
enum    inputPortName      	               { IN };
int     inputPortRows[INPUT_PORT_NUMBER] = {  1 };
int     inputPortCols[INPUT_PORT_NUMBER] = {  1 };

//------------------------------------------------------------------------------
// Specify the number and the sizes of the output ports of the block
//
// Use -1 to specify a dynamically dimensioned output signal
//------------------------------------------------------------------------------
#define OUTPUT_PORT_NUMBER                   1
enum    outputPortName                       { OUT };
int     outputPortRows[OUTPUT_PORT_NUMBER] = {  -1 };
int     outputPortCols[OUTPUT_PORT_NUMBER] = {   1 };

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
#define PARAMETER_NUMBER                     1
enum    parameterName                        { OUTPUT_DIM };
int     parameterType[PARAMETER_NUMBER] =    {          1 };
int     parameterRows[PARAMETER_NUMBER] =    {          1 };
int     parameterCols[PARAMETER_NUMBER] =    {          1 };
int     parameterTunable[PARAMETER_NUMBER] = {          0 };

//------------------------------------------------------------------------------
#include "easylink.h"

//------------------------------------------------------------------------------
class Block : public BaseBlock
{
public:

    static void initializeOutputPortSizes()
    {
        BaseBlock::initializeOutputPortSizes();
        setOutputPortDimensions(OUT,getParameterInt(OUTPUT_DIM),1);
    }

    void outputs()
    {
        double in=getInputDouble(IN);
        Array out=getOutputArray(OUT);

        out.init(in);

    }

};

//------------------------------------------------------------------------------
#include "mdldefinitions.h"

//------------------------------------------------------------------------------
