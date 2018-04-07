//------------------------------------------------------------------------------
/* This file illustrates how to construct a simple C++ S-function with
 * easyLink.  The S-function has one scalar input, one scalar output and one
 * scalar parameter.  The output of the S-function is the sum of its input and
 * of the parameter,
 *
 *   y = u + p
 *
 * To compile this C++ S-function, enter the following command in MATLAB:
 *
 *   >>make sfun_offset.cpp
 *
 * Then open the file "test_offset.mdl" and start the simulation.
 */
//------------------------------------------------------------------------------
#define S_FUNCTION_NAME  sfun_offset

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
int     outputPortRows[OUTPUT_PORT_NUMBER] = {   1 };
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
enum    parameterName                        { OFFSET };
int     parameterType[PARAMETER_NUMBER] =    {      1 };
int     parameterRows[PARAMETER_NUMBER] =    {      1 };
int     parameterCols[PARAMETER_NUMBER] =    {      1 };
int     parameterTunable[PARAMETER_NUMBER] = {      1 };

//------------------------------------------------------------------------------
#include "easylink.h"

//------------------------------------------------------------------------------
class Block : public BaseBlock
{
public:

    void start()
    {
    }

    void outputs()
    {
        double in=getInputDouble(IN);
        double offset=getParameterDouble(OFFSET);

        setOutputDouble(OUT,in+offset);
    }

    void terminate()
    {
    }

};

//------------------------------------------------------------------------------
#include "mdldefinitions.h"

//------------------------------------------------------------------------------
